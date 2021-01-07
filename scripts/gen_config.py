import sys
Import("env")  # type: ignore

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
