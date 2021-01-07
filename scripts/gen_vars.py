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
Import("env")  # type: ignore


def setOTA():
    config = env.GetProjectConfig()  # type: ignore
    # Should use OTA or serial upload
    use_ota = config.get('common', 'use_ota')
    # Set upload port as ip address
    if use_ota.lower() == 'true':
        ota_port = config.get('common', 'ota_port')
        ota_password = config.get('common', 'ota_password')
        env.Append(UPLOADERFLAGS=['--auth', ota_password])  # type: ignore
        env['UPLOAD_PROTOCOL'] = 'espota'  # type: ignore
        env['UPLOAD_PORT'] = ota_port  # type: ignore
    # Set upload port as serial port
    elif use_ota.lower() == 'false':
        serial_port = config.get('common', 'serial_port')
        # Handle empty serial ports
        if serial_port:
            env['UPLOAD_PORT'] = serial_port  # type: ignore
    # Invalid user input
    else:
        sys.exit('"use_ota" option in "user_config.ini" must be boolean value')


setOTA()
