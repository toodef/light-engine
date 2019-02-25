@echo off

if not exist "artifacts" mkdir artifacts && cd artifacts
if not exist "win" mkdir win && cd win
if not exist "lib" mkdir lib && cd lib

call ..\..\..\tools\install_deps_lib.bat

git clone https://github.com/pybind/pybind11.git
set PYBIND11_DIR=%CD%\pybind11

curl -OL https://www.python.org/ftp/python/3.7.2/python-3.7.2.post1-embed-amd64.zip -o python-3.6.8-embed-amd64.zip
7z x python-3.6.8-embed-amd64.zip

cmake ..\..\.. -G "Visual Studio 15 2017 Win64" -DBUILD_EXAMPLES="OFF" -DBUILD_PYTHON="ON" -DVERSION="0.0.2" || exit /b 1
msbuild /nologo /verbosity:quiet /p:Configuration=Release Project.sln || exit /b 1

cd ..\..\..
