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

Import('env')  # type: ignore


def callback(source, target, env):
    print('\033[96m' 'Task completed successfully')


env.AddCustomTarget(  # type: ignore
    name='led_config',
    dependencies=None,
    actions=[
        'python "$PROJECT_DIR/scripts/ui/led_config.py"',
        callback],
    title='Led Configuration',
    description='Graphical interface to configure LED strips.',
    always_build=True
)
