[![Ubuntu linux build workflow](https://github.com/Abdul-AZ/mgfield/actions/workflows/ubuntu_build.yml/badge.svg?branch=master&event=push)](https://github.com/Abdul-AZ/mgfield/actions/workflows/ubuntu_build.yml) [![Windows build workflow](https://github.com/Abdul-AZ/mgfield/actions/workflows/windows_build.yml/badge.svg?branch=master&event=push)](https://github.com/Abdul-AZ/mgfield/actions/workflows/windows_build.yml) [![MacOS build workflow](https://github.com/Abdul-AZ/mgfield/actions/workflows/macos_build.yml/badge.svg?branch=master&event=push)](https://github.com/Abdul-AZ/mgfield/actions/workflows/macos_build.yml)
# MGFIELD
MgField is a desktop application used to visualize magnetic fields of various objects. This application is mostly made for educational purposes to build intuition for people getting into electricity and magnetism. The application is developed in C++ with the help of the [Qt](https://www.qt.io/) framework.

# Building
### Requirements
- Qt version 6.6.1 or newer
- CMake
### Using Qt Creator 
- Open ```mgfield.pro```
- Configure environment to 6.6.1 (use MinGW on windows)
- Build and run (F5)
### Manual Building 
- ```mkdir build```
- ```cmake -B ./build -S .```
- ```cd build```
- ```cmake --build .```

# Contributing
All contributions are welcome including feature requests.

# License
Project is licensed with the [MIT](https://opensource.org/license/mit) license.
