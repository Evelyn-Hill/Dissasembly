@echo off
cmake -B build -G Ninja
cmake --build build
copy build\compile_commands.json .

if "%1" == "test" (
	echo == TEST OUT == 
	.\build\tests.exe
)

if "%1" == "run" (
	echo == PROGRAM OUT ==
	.\build\game.exe
)
