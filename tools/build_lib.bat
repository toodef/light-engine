@echo off

if not exist "artifacts" mkdir artifacts & cd artifacts
if not exist "win" mkdir win & cd win
if not exist "lib" mkdir lib & cd lib

call ..\..\..\tools\install_deps_lib.bat

cmake ..\..\.. -G "Visual Studio 15 2017 Win64" -DBUILD_EXAMPLES="OFF" -DBUILD_PYTHON="OFF" -DVERSION="0.0.2" || exit /b 1
msbuild /nologo /verbosity:quiet /p:Configuration=Release Project.sln || exit /b 1

cd ..\..\..
