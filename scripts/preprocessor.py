# This file is part of ArduRGB, an app to control lighting devices.
# Copyright (C) 2021 Milan Sorak.
#
# ArduRGB is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ArduRGB is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ArduRGB.  If not, see <https://www.gnu.org/licenses/>.

import sys
from configparser import ConfigParser
from os import scandir, makedirs
from os.path import abspath, join, isfile, isdir
from shutil import copyfile
Import('env')  # type: ignore

# Global vars
virtual_strips = 0
physical_strips = 0
effects_data = []
effects_args = {}
pio_env = env['PIOENV']  # type: ignore
# Get relevant paths
base_path = env['PROJECT_DIR']  # type: ignore
include_path = env['PROJECT_INCLUDE_DIR']  # type: ignore
src_path = env['PROJECT_SRC_DIR']  # type: ignore
lib_path = abspath(join(base_path, 'lib'))
# Store import paths
main_path = abspath(join(src_path, 'main.cpp'))
patch_dir = abspath(join(base_path, 'patched'))
patched_path = abspath(join(patch_dir, 'main.cpp'))
# Setup config reading
env_config = env.GetProjectConfig()  # type: ignore
config = ConfigParser()
config_name = 'user_config.ini'
config_path = abspath(join(base_path, config_name))
config.read(config_path)


def getLeds():
    global virtual_strips
    global physical_strips
    global out_args
    leds_output = {}
    # Get all physical LED strips
    for key in config['leds']:
        if 'physical_strip_' in key:
            strip_id = config['leds'][key]
            if int(strip_id) < 0:
                sys.exit(
                    f'Physical strip identifier {strip_id} for "{key}" invalid'
                    f' (identifier must be >= 0)')
            leds_output[f'STRIP_PHYSICAL_{strip_id}'] = strip_id
            physical_strips += 1
    # Attributes that should not be defined in uppercase
    reg_attr = [
        'physical_correction']
    # Attributes that should be defined as char arrays
    char_attr = [
        'led_effect']
    # Get attributes for all physical LED strips
    physical_attr = {
        'physical_pin': 'STRIP_PHYSICAL_PIN',
        'physical_type': 'STRIP_PHYSICAL_TYPE',
        'physical_order': 'STRIP_PHYSICAL_ORDER',
        'physical_correction': 'STRIP_PHYSICAL_CORRECTION',
        'physical_leds': 'STRIP_PHYSICAL_LEDS'}
    for i in range(physical_strips):
        for key, value in physical_attr.items():
            out_str = config['leds'][f'{key}_{i}']
            # Format user definition
            if key in reg_attr:
                leds_output[f'{value}_{i}'] = out_str
            elif key in char_attr:
                leds_output[f'{value}_{i}'] = f'"{out_str}"'
            else:
                leds_output[f'{value}_{i}'] = out_str.upper()
            # Handle empty input
            if not out_str:
                leds_output[f'{value}_{i}'] = 'NULL'
    # Get all virtual LED strips
    for key in config['leds']:
        if 'led_strip_' in key:
            strip_id = config['leds'][key]
            if int(strip_id) > physical_strips or int(strip_id) < 0:
                sys.exit(
                    f'Virtual strip identifier {strip_id} for {key} invalid'
                    f' (identifier must match physical strip)')
            leds_output[f'LED_STRIP_{virtual_strips}'] = strip_id
            virtual_strips += 1
    virtual_attr = {
        'led_strip': 'LED_STRIP',
        'led_num': 'LED_NUM',
        'led_delay': 'LED_DELAY',
        'led_effect': 'LED_EFFECT',
        'led_reversed': 'LED_REVERSED'}
    for i in range(virtual_strips):
        for key, value in virtual_attr.items():
            out_str = config['leds'][f'{key}_{i}']
            # Handle additional args
            if len(out_str.split(',')) > 1:
                out_args = []
                for arg in out_str.split(',')[1:]:
                    if arg:
                        out_args.append(arg)
                effects_args[i] = out_args
                out_str = out_str.split(',')[0]
            # Format user definition
            if key in reg_attr:
                leds_output[f'{value}_{i}'] = out_str
            elif key in char_attr:
                leds_output[f'{value}_{i}'] = f'"{out_str}"'
            else:
                leds_output[f'{value}_{i}'] = out_str.upper()
            # Handle empty input
            if not out_str:
                leds_output[f'{value}_{i}'] = 'NULL'
    return(leds_output)


def writeHeader(lines):
    pio_platform = env['PIOPLATFORM']  # type: ignore
    communication_type = config['common']['communication_type']
    monitor_speed = env_config.get(f'env:{pio_env}', 'monitor_speed')
    wifi_ssid = config['common']['wifi_ssid']
    wifi_password = config['common']['wifi_password']
    ota_password = config['common']['ota_password']
    leds_path = abspath(join(include_path, 'user_definitions.h'))
    with open(leds_path, 'w') as file:
        file.write(
            f'// AUTOGENERATED\n'
            f'// Global definitions\n\n'
            f'#define {pio_platform.upper()}\n'
            f'#define _{communication_type.upper()}\n\n')
        for key, value in lines.items():
            file.write(f'#define {key} {value}\n')
        file.write(f'\n#define STRIP_NUM {virtual_strips}\n'
                   f'#define P_STRIP_NUM {physical_strips}\n'
                   f'#define V_STRIP_NUM {virtual_strips}\n\n'
                   f'#define BAUDRATE {monitor_speed}\n\n'
                   f'#define WIFI_SSID "{wifi_ssid}"\n'
                   f'#define WIFI_PASSWORD "{wifi_password}"\n'
                   f'#define OTA_PASSWORD "{ota_password}"\n')


def createIdentifiers():
    # Generate string literals to send to UI client
    default_brightness = config['common']['default_brightness']
    device_name = config['common']['device_name']
    prog_version = env_config.get('project', 'version')
    output_items = [device_name,
                    pio_env,
                    prog_version,
                    physical_strips,
                    virtual_strips]
    # Formatters for message output types

    def serialOutput():
        serial_output = str(len(output_items) + 1)
        for item in output_items:
            serial_output += f',{item}'
        serial_output += ','
        return serial_output

    outputters = {
        'OUT_SERIAL_MESSAGE': serialOutput}
    # Write identifiers to file
    identifiers_path = abspath(join(include_path, 'identifiers.h'))
    with open(identifiers_path, 'w') as file:
        file.write(
            '// AUTOGENERATED\n// Output string literals for device identification\n\n')
        for out_name, out_format in outputters.items():
            file.write(f'#define {out_name} "{out_format()}"\n')


def setDefinitions(file_path):
    effects_path = abspath(join(src_path, 'effects'))
    effects_files = []
    effects_names = []
    global effects_data
    # Find all effects files
    for file in scandir(effects_path):
        if file.path.endswith('.h'):
            effects_files.append(file.path)
            effects_names.append(file.name)
    # Get all effect data
    i = 0
    for effect_file in effects_files:
        if isfile(effect_file):
            with open(effect_file, 'r') as file:
                for line in file:
                    # Get char array that is used to run effect
                    try:
                        if line.split()[0] == '#define' and line.split()[2] == 'EFFECT':
                            effect_name = line.split()[1]
                            effects_data.append({effect_name: []})
                    except:
                        pass
                    # Get effect class name
                    if 'class ' in line:
                        class_name = line.split()[1].strip()
                        list(effects_data[i].values())[0].append(class_name)
                    # Get effect args
                    elif 'static void run(' in line:
                        args = line.split('static void run')[
                            1].strip('();\n').split(', ')
                        args_list = list(effects_data[i].values())[0]
                        for arg in args:
                            arg = arg.split()[1].strip('&*[]')
                            args_list.append(arg)
            i += 1

    leds_path = abspath(join(include_path, 'user_definitions.h'))

    # Include effects in main
    def effectsInjector(file):
        for effect in effects_names:
            file.write(f'#include "effects/{effect}" // AUTOGENERATED\n')

    injectLines(file_path, '// ---------- Effects ---------- //\n',
                effectsInjector)

    # Additional definitions
    def definesInjector(file):
        default_brightness = config['common']['default_brightness']
        file.write(
            f'#define DEFAULT_BRIGHTNESS {default_brightness} // AUTOGENERATED\n')

    injectLines(file_path, '// ---------- Defines ---------- //\n',
                definesInjector)

    # Add LED definitions to main
    def ledInjector(file):
        file.write(
            '// Physical LED strip arrays from which virtual CRGBSets are created\n')
        # Generate CRGBArrays for each physical strip
        for i in range(physical_strips):
            file.write(
                f'CRGBArray<STRIP_PHYSICAL_LEDS_{i}> strip_{i}; // AUTOGENERATED\n')
        file.write(
            '\n// Array for each led and associated hue values for use in effects\n')
        strip_defines = []
        with open(leds_path, 'r') as file_:
            # strip_defines = [
            #     {'LED_STRIP': 0,
            #      'LED_NUM': 10,
            #      'LED_DELAY': 25,
            #      'LED_EFFECT': NULL,
            #      'LED_REVERSED': FALSE}]
            # Initialise empty structure for each virtual strip
            # Position in array corresponds to strip identifier
            for i in range(virtual_strips):
                strip_defines.append({})
            for line in file_:
                if '#define LED_' in line:
                    name = line.split()[1]
                    attr = name[:-2]
                    strip_id = int(name.split('_')[2])
                    attr_val = line.split()[2]
                    try:
                        attr_val = int(attr_val)
                    except:
                        pass
                    strip_defines[strip_id][attr] = attr_val
        # Track virtual strips within their physical strips
        p_strips_checked = []
        for i in range(physical_strips):
            p_strips_checked.append(0)
        # Generate start and end index based on other virtual strips within a physical strip
        for strip in strip_defines:
            parent = strip['LED_STRIP']
            start_index = 0
            # If another virtual strip already exists on this physical strip
            if p_strips_checked[parent]:
                start_index = 'LED_NUM_0'
                # Sum all previous start indices
                for i in range(p_strips_checked[parent] - 1):
                    start_index += f' + LED_NUM_{i + 1}'
            end_index = 'LED_NUM_0'
            # Sum all previous end indices
            for i in range(p_strips_checked[parent]):
                end_index += f' + LED_NUM_{i + 1}'
            end_index += ' - 1'
            # Add number of virtual strips assigned to its parent
            p_strips_checked[parent] += 1
            # Set start and end index as start and end of virtual strip
            if strip['LED_REVERSED'] == 'FALSE':
                strip['START_INDEX'] = start_index
                strip['END_INDEX'] = end_index
            # Invert start and end positions of virtual strips to register reversed led order
            else:
                strip['START_INDEX'] = end_index
                strip['END_INDEX'] = start_index
        # Generate CRGBSet, hue array, and effect args array for each virtual strip
        for i, strip in enumerate(strip_defines):
            parent = strip['LED_STRIP']
            start_index = strip['START_INDEX']
            end_index = strip['END_INDEX']
            file.write(
                f'CRGBSet led_array_{i}(strip_{parent}({start_index}, {end_index})); // AUTOGENERATED\n'
                f'uint8_t led_hues_{i}[LED_NUM_{i}]; // AUTOGENERATED\n')
            # Predefine existing args
            if i in effects_args:
                args_array = '{'
                for arg in effects_args[i]:
                    args_array += f'{arg}, '
                args_array += "'\\0'}"
                file.write(
                    f'uint8_t led_args_{i}[ARGS_NUM] = {args_array}; // AUTOGENERATED\n\n')
            else:
                file.write(
                    f'uint8_t led_args_{i}[ARGS_NUM]; // AUTOGENERATED\n\n')
        # Generate LEDDict to store information for all virtual strips
        file.write('// Store all relevant information about each LED strip\n'
                   'LEDDict strips[]{\n')
        for i in range(virtual_strips):
            default_effect = strip_defines[i]['LED_EFFECT']
            if default_effect != 'NULL':
                out_line = '    {'f'led_array_{i}, led_hues_{i}, led_args_{i}, LED_NUM_{i}, hash({default_effect}), 0, 0, LED_DELAY_{i}, true''}'
            else:
                out_line = '    {'f'led_array_{i}, led_hues_{i}, led_args_{i}, LED_NUM_{i}, 0, 0, 0, LED_DELAY_{i}, true''}'
            file.write(out_line)
            # Not last struct element
            if i < virtual_strips - 1:
                file.write(',')
            # End of struct
            else:
                file.write('};')
            file.write(' // AUTOGENERATED\n')

    injectLines(
        file_path, '// ---------- LED Definitions ---------- //\n', ledInjector)

    # Init physical strips
    def stripInitInjector(file):
        for i in range(physical_strips):
            file.write(f'  FastLED.addLeds<STRIP_PHYSICAL_TYPE_{i}, STRIP_PHYSICAL_PIN_{i}, STRIP_PHYSICAL_ORDER_{i}>'
                       f'(strip_{i}, STRIP_PHYSICAL_LEDS_{i})')
            # Set strip corrections
            correction = config['leds'][f'physical_correction_{i}']
            if correction:
                file.write(f'.setCorrection({correction})')
            file.write('; // AUTOGENERATED\n')

    injectLines(
        file_path, '  // ---------- Strip Init ---------- //\n', stripInitInjector)

    # Switch-case for running effects
    def runEffectInjector(file):
        file.write('// AUTOGENERATED\nvoid runEffect()\n{\n'
                   '  for (uint8_t i = 0; i < STRIP_NUM; i++)\n  {\n'
                   '    switch (strips[i].current_mode)\n    {\n')
        # Set effect data
        for effect in effects_data:
            for key, value in effect.items():
                # Input that will be switched over to run associated effect
                case_str = key.lower()
                case_class = value[0]
                # Arguments that the effect expects
                case_args = ''
                # New array without class name
                args = value
                del args[0]
                for i, arg in enumerate(args):
                    if i:
                        case_args += ', '
                    # Effect requires whole LEDDict
                    if 'strips' in arg:
                        case_args += 'strips'
                    # Effect requires LEDDict member
                    else:
                        case_args += f'strips[i].{arg}'
                file.write(f'    case hash("{case_str}"):\n'
                           f'      {case_class}::run({case_args});\n'
                           f'      break;\n')
        file.write('    }\n  }\n}\n')

    injectLines(
        file_path, '// ---------- Run Effects ---------- //\n', runEffectInjector)


def injectLines(path, separator, injector):
    # Get current data
    with open(path, 'r') as file:
        data = file.readlines()
    # Write new data
    with open(path, 'w') as file:
        in_section = False
        for line in data:
            # Transition between separators
            if line == separator:
                in_section = not in_section
                file.write(separator)
                # Inject new code in between separators
                if in_section:
                    injector(file)
            # Write original code
            elif not in_section:
                file.write(line)


def copyMain():
    if not isdir(patch_dir):
        try:
            makedirs(patch_dir)
        except:
            sys.exit('Failed to create directory "src/patched"')
    try:
        copyfile(main_path, patched_path)
    except:
        sys.exit('Failed to copy "src/main.cpp" to "src/patched/main.cpp"')


def preprocessor(node, run=[]):
    if not run:
        # Copy unpatched 'main.cpp' to temporary file for injection
        copyMain()
        # Get user LED definitions
        led_data = getLeds()
        # Write LED definitions to header files
        writeHeader(led_data)
        createIdentifiers()
        # Modify main file to contain relevant definitions
        setDefinitions(patched_path)
        # Only run once
        run.append(1)
    return node


def replaceMainPatched(node):
    # Pass patched 'main.cpp' as 'main.cpp'
    print('\033[96m' f'Using {patched_path} in place of {main_path}')
    return env.File(patched_path)  # type: ignore


def skipAsmFromBuild(node):
    # Ignore file from build process
    return None


# Run main preprocessor
env.AddBuildMiddleware(preprocessor, main_path)  # type: ignore
# Pass patched 'main.cpp' as 'main.cpp'
env.AddBuildMiddleware(replaceMainPatched, main_path)  # type: ignore
