# Cpp template application and unit tests

This is a C++ template project to create a C++ App and test its code with googletest.

## Submodules

This project uses git submodules.
After cloning this repository, the submodules directories will be there, but empty. You must run two commands: `git submodule init` to initialize your local configuration file, and `git submodule update` to fetch all the data from those projects and check out the appropriate commits listed in the superproject.

If a new submodule is needed, it can be added as follows:

```sh
git submodule add <repo_address> <3rdParty/name_of_repo_folder>
```

The repository referenced by *repo_address* will be cloned into the folder *name_of_folder*, which should not exist yet.

After adding the submodule with the command above, the desired tag of the submodule repository should be checked out and the changes must be commited.

## Needed software

### CMake

CMake is a free, cross-platform, software development tool for building applications via compiler-independent instructions. It also can automate testing, packaging and installation. It runs on a variety of platforms and supports many programming languages. And more importantly, it is a tool used in this project. Currently the minimum version required to build this project is the 3.28.

To install CMake there are at least two options.

#### Option 1

```sh
sudo apt install cmake
cmake --version
```

If the version installed is equal or greater than 3.28 you´re good to go.

#### Option 2

```sh
sudo apt remove --purge --auto-remove cmake
version=3.28
build=5
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/
./bootstrap
make -j4
sudo make install
cmake --version
```

## Compilation

### VS Code

A build task was defined, to start the building process just press "Ctrl + Shift + B".

### Release version

The *cmake_build_release.sh* script is intended to build a release version of the application.

## Debugging

*gdb* must be installed in order to debug. If *gdb* is not installed execute the following command:

```sh
sudo apt install gdb
```

To debug the unit tests a VS Code configuration was added (*launch.json*). To start debugging just set a breakpoint in a test or source file and press **F5**. The unit tests will be launched and the execution will stop at the breakpoint. By default *online* tests are excluded. Modify the *args* parameter of the *launch.json* file to include them.

**Cheers mate!**
