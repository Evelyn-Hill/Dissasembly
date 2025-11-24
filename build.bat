@echo off
cmake -B build -G Ninja
cmake --build build
copy build\compile_commands.json .

if "%1" == "test" (
	.\build\tests.exe
)
