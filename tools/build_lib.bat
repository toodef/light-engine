@echo off

if not exist "artifacts" mkdir artifacts

cd artifacts

if not exist "win64" mkdir win64

cd win64

cmake ..\.. -G "Visual Studio 15 2017 Win64" -DBUILD_EXAMPLES="ON" -DBUILD_PYTHON="OFF" -DVERSION="0.0.2" || exit /b 1

msbuild /nologo /verbosity:quiet /p:Configuration=Release cpp-template.sln || exit /b 1

test\Release\tests.exe || exit /b 1

cd ..\..