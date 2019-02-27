import sys

import shutil
from setuptools import setup, Distribution
import argparse
import os


class BinaryDistribution(Distribution):
   def is_pure(self):
      return False


DISTNAME = "lepy"

parser = argparse.ArgumentParser(description='Build ' + DISTNAME + ' whl file. Script get bin and source directories and create whl in working directory')
parser.add_argument('--bin-path', help='Root dir to find compiled *.pyd file', required=True)
parser.add_argument('--source-path', help='Directory, that contains source files (__init__.py and others)',
                     required=True)
parser.add_argument('--version', help='Version of whl package (as example 1.0.0)')

args = parser.parse_args()

print('Start processing with following params:')
print('  bin-path: ' + args.bin_path)
print('  source-path: ' + args.source_path)
print('  version: ' + args.version)
print('Working directory: ' + str(sys.argv))

root_dir = os.path.abspath(os.path.join(sys.argv[0], os.pardir, os.pardir))

if not os.path.exists(args.bin_path):
   print('bin-path options points to nonexistent directory', file=sys.stderr)
   sys.exit(1)

if not os.path.exists(args.source_path):
   print('source-path options points to nonexistent directory', file=sys.stderr)
   sys.exit(1)

platform_info = {'platform_name': None, 'py_dll_ext': None}
if sys.platform == "linux":
   platform_info['py_dll_ext'] = '.so'
   platform_info['platform_name'] = 'linux_amd64'
elif sys.platform == "win32":
   platform_info['py_dll_ext'] = '.pyd'
   platform_info['platform_name'] = 'win_amd64'
else:
   print('Unknown platform!', file=sys.stderr)
   sys.exit(1)

dll_files_pathes = [os.path.join(dir_path, file) for dir_path, dir_name, filenames in os.walk(args.bin_path) for file in
                    filenames if os.path.splitext(file)[1] in [platform_info['py_dll_ext'], ".dll"]]

if len(dll_files_pathes) < 1:
   print('No one *' + platform_info + ' file was found!', file=sys.stderr)
   sys.exit(1)

#if len(dll_path) > 1:
#    print('Find more then one ' + platform_info + ' files:', file=sys.stderr)
#    print('  '.join(dll_path), file=sys.stderr)
#    sys.exit(1)

#dll_path = dll_path[0]

#if args.version is None:
#    args.version = 'unversioned'

print('Build whl from:')
print('  ' + str(dll_files_pathes))
print('Version: ' + args.version)

if os.path.exists(DISTNAME):
   shutil.rmtree(DISTNAME)

os.makedirs(DISTNAME)
dll_names = [os.path.basename(n) for n in dll_files_pathes]
for dll in dll_files_pathes:
   shutil.copyfile(dll, os.path.join(DISTNAME, os.path.basename(dll)))
shutil.copyfile(os.path.join(args.source_path, '__init__.py'), os.path.join(DISTNAME, '__init__.py'))

sys.argc = 1
sys.argv = [sys.argv[0], 'bdist_wheel']

try:
   from wheel.bdist_wheel import bdist_wheel as _bdist_wheel


   class bdist_wheel(_bdist_wheel):
      def finalize_options(self):
         _bdist_wheel.finalize_options(self)
         self.root_is_pure = False

except ImportError:
   bdist_wheel = None

files_for_setup = dll_names + ["__init__.py"]

print(files_for_setup)

with open(os.path.join(root_dir, "README.md"), "r") as fh:
   long_description = fh.read()

setup(
   name=DISTNAME,
   version=args.version,
   author="Anton Fedotov",
   author_email="anton.fedotov.af@gmail.com.com",
   description='Lightweight and fast 3D visualisation engine writen on C++ with Python bindings',
   long_description=long_description,
   long_description_content_type="text/markdown",
   url="https://github.com/toodef/light-engine",
   packages=[DISTNAME],
   install_requires=[],
   package_data={ DISTNAME: files_for_setup },
   classifiers=[
        'Development Status :: 3 - Alpha',
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'Operating System :: OS Independent',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules',
   ]#,
#   distclass=BinaryDistribution,
#   cmdclass={'bdist_wheel': bdist_wheel}
)
