@echo off

if not exist "artifacts" mkdir && cd artifacts
if not exist "win" mkdir win && cd win
if not exist "lib" mkdir lib && cd lib

cmake ..\.. -G "Visual Studio 15 2017 Win64" -DBUILD_EXAMPLES="OFF" -DBUILD_PYTHON="ON" -DVERSION="0.0.2" || exit /b 1

msbuild /nologo /verbosity:quiet /p:Configuration=Release light-engine.sln || exit /b 1

cd ..\..\..