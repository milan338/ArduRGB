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

from os.path import abspath, join
Import('env')  # type: ignore

config = env.GetProjectConfig()  # type: ignore
base_path = env['PROJECT_SRC_DIR']  # type: ignore
main_path = abspath(join(base_path, 'main.cpp'))


def copyFile():
    import sys
    from shutil import copyfile
    target_path = abspath(join(base_path, 'main_ld.cpp'))
    # Copy file
    try:
        copyfile(main_path, target_path)
    except:
        sys.exit('Failed to copy "src/main.cpp" to "src/main_ld.cpp"')
    # Disable file
    with open(target_path, 'r') as file:
        data = file.readlines()
    with open(target_path, 'w') as file:
        file.write(
            '// This file contains all code injected by the preprocessor\n\n#ifdef MAIN_LD\n\n')
        for line in data:
            file.write(line)
        file.write('\n#endif // MAIN_LD\n')


def removeInjections(source, target, env):
    separators = [
        '// ---------- Effects ---------- //\n',
        '// ---------- LED Definitions ---------- //\n',
        '  // ---------- Strip Init ---------- //\n',
        '// ---------- Run Effects ---------- //\n']
    # Get current data
    with open(main_path, 'r') as file:
        data = file.readlines()
    # Write new data
    with open(main_path, 'w') as file:
        in_section = False
        i = 0
        inject = [False, True, False, True]
        to_inject = [
            None,
            '// Store all relevant information about each LED strip\nLEDDict strips[]{}; // TEMPORARY\n',
            None,
            'void runEffect() { ; }; // TEMPORARY\n']
        for line in data:
            # Transition between separators
            if line in separators:
                in_section = not in_section
                file.write(line)
                # Declare placeholders to prevent linter errors
                if in_section:
                    if inject[i]:
                        file.write(to_inject[i])
                    i += 1
            # Write original code
            elif not in_section:
                file.write(line)


# Copy 'main.cpp' with injections
if config.get('build', 'save_injected_main').lower() == 'true':
    copyFile()
# Remove injections after build
env.AddPostAction('buildprog', removeInjections)  # type: ignore
