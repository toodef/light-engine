@echo off

if not exist "artifacts" mkdir artifacts & cd artifacts
if not exist "win" mkdir win & cd win
if not exist "lib" mkdir lib & cd lib

call ..\..\..\tools\install_deps_lib.bat

curl -OL https://www.transmissionzero.co.uk/files/software/development/GLUT/freeglut-MSVC.zip -o freeglut-MSVC.zip
7z x freeglut-MSVC.zip
set FREEGLUT_ROOT=%CD%\freeglut

cmake ..\..\.. -G "Visual Studio 15 2017 Win64" -DBUILD_EXAMPLES="ON" -DBUILD_PYTHON="OFF" -DVERSION="0.0.2" || exit /b 1
msbuild /nologo /verbosity:quiet /p:Configuration=Release Project.sln || exit /b 1

cd ..\..\..
