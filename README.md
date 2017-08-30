# NOTICE
This project is suspended and replaced by ZheQuant-brain-python

# Usage
The ZheQuant worker for time-comsuming calculation work and written by C++

# TODO
- A list of default database table and its fields. These fields should be the standard for plugins.
- Plugin System for more commands&calculators. It should has the following features:
    - the plugin system can auto-detect the plugins, at least during compiling
- every plugin should provide
    - what name of its command is, so the plugin system knows which plugin to use for a input command line
    - the names of those standard database objects which are used
- a way to map customized database object names to standard ones

# Tips
Unless otherwise noted, this document assumes you are under the project root directory.

# Build
```
1. makedir build && cd build
2. cmake ..
3. make
```

This is called out-of-source build.

# Run

```
./build/cbrain
```
If you are not the super user, you may need run it with sudo.

# Test
The project uses CTest and Google Test.
To run the test, enter the command:
```
cd build && ctest -V
```
The option -V means "verbose" so that the output from Google Test will be displayed.

# Develop
This project uses [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

If you found anything not sticking to it, please raise an issue and let me know. Thanks!
