@echo off
cmake -B build -G Ninja
cmake --build build
copy build\compile_commands.json .
