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
Import('env')  # type: ignore

config = env.GetProjectConfig()  # type: ignore


def configExists():
    return config.has_section('user_config')


def copyConfig():
    from os.path import abspath, join
    from shutil import copyfile
    base_path = env['PROJECT_DIR']  # type: ignore
    src_path = abspath(join(base_path, 'scripts', 'user_config_TMP.ini'))
    target_path = abspath(join(base_path, 'user_config.ini'))
    try:
        copyfile(src_path, target_path)
    except:
        sys.exit('Failed to copy "scripts/user_config.ini" to "user_config.ini"')


if not configExists():
    copyConfig()
    sys.exit(
        '"user_config.ini" has been generated in the project root,'
        ' please configure for your hardware.')

elif len(config.get('platformio', 'default_envs')) != 1:
    sys.exit('Option "default_envs" must contain exactly one environment')
