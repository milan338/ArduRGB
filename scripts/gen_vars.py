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
