# Build Requirments

Sootlib requires C++17

Sootlib can be built without any optional dependencies by turning off Cantera compatability and not building tests.

Google Test: The unit tests in `test/` require Google Test to be built. Google Test is currently included in this
directory. https://github.com/google/googletest

Cantera: To build the Cantera compat components of the library and any tests or examples that require Cantera, you need
Cantera installed as a system library. https://cantera.org/install/index.html

# Build Instructions

Run `cmake .` to generate the sootlib build files in the current directory while inside of the top level sootlib directory.

This will generate the necessary Makefiles and you can now run `make` to build the project.

Use the options described below to customize your build. If you run `cmake .` and then change your build options, you will
need to either delete `CMakeCache.txt` or ensure that you have removed the cached values you changed.

# Build options

In the top level `CMakeLists.txt` you can enable/disable `BUILD_CANTERA_COMPAT`. This will:
* Determine if Cantera examples will build (if examples are set to be built)
* Determine if Cantera tests will be built (if tests are set to be built)
* Determine if Cantera integration elements of sootlib are built and included in the library

In the top level `CMakeLists.txt` you can enable/disable `BUILD_TESTS`. This will:
* Determine if gtest tests are built
* To build tests on Mac you need Google Test installed system wide

In the top level `CMakeLists.txt` you can enable/disable `BUILD_EXAMPLES`. This will:
* Determine if examples are built

You can add .cpp source files to `example/standard/` and `example/cantera/`. Add these files to either
`example/standard/CMakeLists.txt` or `example/cantera/CMakeLists.txt` depending on if they require Cantera. Add
`example_src.cpp` as `example_src`. These files will be built as standalone executables.

You can add .cpp source files to `test/standard` and `test/cantera`. Add these files to either
`test/standard/CMakeLists.txt` or `test/cantera/CMakeLists.txt` depngind on if they required Cantera. Add
`example_test.cpp` as `example_test`. These files will be built as standalone executables with Google Test. Files in
`test/standard/` will be added to the test list `standardTests`. Files in `test/cantera` will be added to the test list
`canteraTests`.

We should roughly (or closely) follow the [Cantera contributions stype guide](https://github.com/Cantera/cantera/blob/main/CONTRIBUTING.md)
in this project.

# TODO

### Short Term Design

* Create some common code for duplicate gas source term code
* Optimize all models by saving intermediate calculations
* Perhaps add warnings when running into 0 or having illegal calculations to guide people in NAN situations
  
### Long Term Design

* Optimized higher dimentional models
* Determine needed state constructor / generator features
* Determine needed cantera interface features

### Broken / Not Building

* SECT model was highly broken in old code so current interpretation is probably not accurate

### Untested

* `InputState`
* `GrowthModel_LIN`
* `GrowthModel_LL`
* `GrowthModel_HACA`
* `NucleationModel_LIN`
* `NucleationModel_LL`
* `NucleationModel_PAH`
* `OxidationModel_LL`
* `OxidationModel_LEE_NEOH`
* `OxidationModel_NSC_NEOH`
* `OxidationModel_HACA`
* `CoagulationModel_LL`
* `CoagulationModel_Frenk`
* `CoagulationModel_Fuchs`
* `SootModel_MONO`
* `SootModel_MOMIC`
* `SootModel_QMOM`
* `SootModel_LOGN`
* `SootModel_SECT`

### Not Implemented

*

### Missing Features

* `cantera_compat`
