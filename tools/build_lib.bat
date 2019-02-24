@echo off

if not exist "artifacts" mkdir && cd artifacts
if not exist "win" mkdir win && cd win
if not exist "python" mkdir python && cd python

cmake ..\..\.. -G "Visual Studio 15 2017 Win64" -DBUILD_EXAMPLES="ON" -DBUILD_PYTHON="OFF" -DVERSION="0.0.2" || exit /b 1

msbuild /nologo /verbosity:quiet /p:Configuration=Release cpp-template.sln || exit /b 1

cd ..\..\..
