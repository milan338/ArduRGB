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

Import("env")  # type: ignore

config = env.GetProjectConfig()  # type: ignore
save_patched = config.get('build', 'save_patched')


def removePatched(source, target, env):
    from os import scandir, remove, listdir, rmdir
    from os.path import abspath, join, isdir
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


if save_patched.lower() == 'false':
    env.AddPostAction("checkprogsize", removePatched)  # type: ignore
