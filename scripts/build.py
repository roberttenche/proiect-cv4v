from sys import argv
from os import getcwd, mkdir, remove, rename, chdir, path, environ
from platform import system
from subprocess import run as system_run
from genericpath import isdir, isfile
from shutil import rmtree, copy
from colorama import Fore, Back, Style

cwd = getcwd()
platform = system()

args = argv
args_len = len(argv)

def set_env_vars():
    # environ['CUDA_MODULE_LOADING'] = 'LAZY' # less memory used at runtime
    print('',end='') # avoids errors
def info(message: str):
    print(Fore.BLUE + '[INFO] ' + Style.RESET_ALL + message)
def success(message: str):
    print(Fore.GREEN + '[SUCCESS] ' + Style.RESET_ALL + message)
def warning(message: str):
    print(Fore.YELLOW + '[WARNING] ' + Style.RESET_ALL + message)
def error(message: str):
    print(Fore.RED + '[ERROR] ' + Style.RESET_ALL + message)
    raise Exception(message)

# Arguments shall be executed in this order if provided
arg_struct = {
    'clean': False,                 # -c --clean
    'CLEAN': False,                 # -C --CLEAN
    'create-build-folders': False,  # -g --create-build-folders
    'build': False,                 # -b --build
    'run': False                    # -r --run
}

def parse_args():
    for arg in args:
        if arg == '--clean' or (arg.count('-') == 1 and 'c' in arg):
            arg_struct['clean'] = True
            continue
        if arg == '--CLEAN' or (arg.count('-') == 1 and 'C' in arg):
            arg_struct['CLEAN'] = True
            continue
        if arg == '--create-build-folders' or (arg.count('-') == 1 and 'g' in arg):
            arg_struct['create-build-folders'] = True
            continue
        if arg == '--build' or (arg.count('-') == 1 and 'b' in arg):
            arg_struct['build'] = True
            continue
        if arg == '--run' or (arg.count('-') == 1 and 'r' in arg):
            arg_struct['run'] = True
            continue


def create_build_folders():
    info('Creating build and bin folders')
    if not isdir(cwd + '/build'):
        mkdir(cwd + '/build')


def move(old, new):
    if isfile(old):
        if isfile(new):
            remove(new)
        rename(old, new)
    else:
        print(old + ' does not exist')


def clean():
    if arg_struct['clean'] is True:
        info('Cleaning build folder')
    elif arg_struct['CLEAN'] is True:
        info('Cleaning all build folders')
    else:
        error('Something went wrong with arg_struct')

    # Clean bin
    if isdir(cwd + '/bin') and arg_struct['CLEAN'] is True:
        rmtree(cwd + '/bin')
    elif not isdir(cwd + '/bin') and arg_struct['CLEAN'] is True:
        info('Bin folder does not exist.')

    # Clean build
    if isdir(cwd + '/build'):
        rmtree(cwd + '/build')
    else:
        info('Build folder does not exist')


def build():
    info('Running build...')

    # info('Updating submodules...')
    # p = system_run('git submodule update --init --recursive')
    # if p.returncode != 0:
    #     error('Updating submodules failed!')

    info('Generating Makefiles...')
    p = system_run('cmake -S . -B build -G "Visual Studio 16 2019"')
    if p.returncode != 0:
        error('Building failed! Unable to generate Makefile')

    success('Project sucessfully built')


def appl_run():
    info('Running application...')
    if isfile('bin/appl') or isfile('bin/appl.exe'):
        system_run('bin/appl')
    else:
        error('Build application first!')


def main():
    set_env_vars()

    parse_args()

    if arg_struct['clean'] == True or arg_struct['CLEAN'] == True:
        clean()

    if arg_struct['create-build-folders'] == True:
        create_build_folders()

    if arg_struct['build'] == True:
        build()

    if arg_struct['run'] == True:
        appl_run()


if __name__ == '__main__':
    main()
