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

from os import scandir, remove, listdir, rmdir
from os.path import abspath, join, isdir
Import("env")  # type: ignore

config = env.GetProjectConfig()  # type: ignore
save_patched = config.get('build', 'save_patched')


def removePatched(source, target, env):
    base_path = env['PROJECT_DIR']
    patch_dir = abspath(join(base_path, 'patched'))
    patch_files = [
        'main.cpp',
        'main.cpp.o']
    # Remove files that will be inside directory
    if isdir(patch_dir):
        for file in scandir(patch_dir):
            if file.name in patch_files:
                try:
                    remove(file)
                except:
                    print('\033[93m' f'Failed to remove file {file}')
    # Remove directory only if empty
    if not len(listdir(patch_dir)):
        try:
            rmdir(patch_dir)
            print(
                '\033[96m' f'Removed patch directory {patch_dir} successfully')
        except:
            print('\033[93m' f'Failed to remove patch directory {patch_dir}')


def defaultDefinitions(source, target, env):
    include_path = env['PROJECT_INCLUDE_DIR']
    definitions_path = abspath(join(include_path, 'definitions.h'))
    with open(definitions_path, 'r') as file:
        data = file.readlines()
    with open(definitions_path, 'w') as file:
        for line in data:
            if '#define MESSAGE_TYPE ' in line:
                file.write(
                    '#define MESSAGE_TYPE _SERIAL // Communication interface\n')
            elif '#define BAUDRATE ' in line:
                file.write(
                    '#define BAUDRATE 9600        // Serial communication speed\n')
            else:
                file.write(line)


if save_patched.lower() == 'false':
    env.AddPostAction('checkprogsize', removePatched)  # type: ignore
env.AddPostAction('checkprogsize', defaultDefinitions)  # type: ignore
