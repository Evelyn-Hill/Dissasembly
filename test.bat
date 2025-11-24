@echo off

IF EXIST .\build\tests.exe (
	.\build\tests.exe
) ELSE (
	call "build.bat" test
)
