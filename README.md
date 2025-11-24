# Dissasembly Line
Is a short game built originally for the Mini Jame Gam #50.

## Dependencies
- CMake v4.0 or higher
- Ninja

## Building
Dissasembly Line uses CMake as it's build system with Ninja as it's preferred generator. <br>

We have premade build scripts setup for both windows `build.bat` and bash `build.sh`.

### Build options
`build test` will build and test the program.

## Testing
We have premade test scripts setup for both windows `test.bat` and bash `test.sh`. <br>

If no `build/tests` binary exists, the test script will trigger a new build of the project.
