# Build System

NIKA's build system is based on the principles of the [sc-machine](https://github.com/ostis-ai/sc-machine) build system. To learn more about them, read [sc-machine build system documentation](https://ostis-ai.github.io/sc-machine/build/build_system/).

## Compilation steps

This project uses CMake presets. These presets help manage different build configurations, making it easier to work with dependencies and streamline the build process.

If you wish to use CLI instead of a graphical interface like Visual Studio Code, you can list all the available configurations in the `CMakePresets.json` file and pick one of the presets:

```sh
conan install . -s build_type=<Debug|Release> --build=missing
cmake --list-presets
```

### Configure presets

Configure presets define how the project is configured before building. They specify various settings, including whether to include tests and which dependencies to use.

| **Name**                     | **Display Name**                        | **Description**                                      | **Build location** |
|------------------------------|-----------------------------------------|------------------------------------------------------|--------------------|
| `debug-conan`                | Debug config (Conan)                    | Debug config with tests (Conan dependencies used)    | build/Debug        |
| `release-conan`              | Release config (Conan)                  | Release config (Conan dependencies used)             | build/Release      |
| `release-with-tests-conan`   | Release config with tests (Conan)       | Release config with tests (Conan dependencies used)  | build/Release      |

## Build presets

Build presets link to the configure presets and specify how to build the project. They determine the output directories based on the selected configuration.

| **Name**   | **Configure Preset**                        | **Build location** |
|------------|---------------------------------------------|--------------------|
| `debug`    | `debug-conan`                               | build/Debug        |
| `release`  | `release-conan`, `release-with-tests-conan` | build/Release      |

## Recommendations

- For development and debugging, use the **Debug config (Conan)** (`debug-conan`) preset. This configuration includes options for testing and benchmarking.
- For production builds, choose the **Release config (Conan)** (`release-conan`) preset to create optimized builds without debugging information.
- If you need to run tests in a production build, select the **Release config with tests (Conan)** (`release-with-tests-conan`) preset.

## Example usage

To build your project using these presets, you can use the following commands:

```sh
# Use pipx to install Conan if not already installed
pipx install conan
```

```sh
# Install dependencies using Conan with Debug build type
conan install . -s build_type=Debug --build=missing
# Configure using debug-conan preset
cmake --preset debug-conan
# Build using debug preset
cmake --build --preset debug
```

To build your project for release, you can build with tests to ensure everything works as expected:

```sh
# Install dependencies using Conan with Debug build type
conan install . -s build_type=Release --build=missing
# Configure using release-with-tests preset
cmake --preset release-with-tests-conan
# Build using release preset
cmake --build --preset release
```

We also define `INSTALL` instructions in our CMake. This routine can be launched by appending `--target install` to the `cmake --build --preset <preset>` command. The result of this command will reside in `build/<Release|Debug>/install` (our default install folder). Install folder is basically a pack of portable executable files and shared libraries required to get NIKA up and running, this command is intended to create clutter-free archives/folders with our code which you can then use in your own projects.
