cmake_minimum_required(VERSION 3.13)
project(EmbeddedControlPlatform LANGUAGES CXX)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_CLANG_TIDY "Run clang-tidy during builds" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXECUTABLE clang-tidy REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
endif()


if(ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()


# Sensor abstraction, SensorLayer, and simulated sensor headers.
# The current implementation is header-only.

add_library(sensor-library INTERFACE)

target_include_directories(sensor-library INTERFACE
        ${PROJECT_SOURCE_DIR}/include
    )

#Signal-filter implementation

add_library(filter-library
    src/filters/filter_layer.cpp
    )

target_link_libraries(filter-library PUBLIC
    sensor-library)

target_include_directories(filter-library PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    )

# Runnable simulator application.

add_executable(thermal-simulator
    src/main.cpp
)

target_link_libraries(thermal-simulator PRIVATE
    sensor-library
    filter-library
    )

#Tests

enable_testing()

add_executable(SensorLayerTests
    tests/sensor_layer_tests.cpp
)

target_include_directories(SensorLayerTests PRIVATE
    ${PROJECT_SOURCE_DIR}/tests
)

target_link_libraries(SensorLayerTests PRIVATE
    sensor-library
)

add_test(
    NAME SensorLayerTests
    COMMAND SensorLayerTests
)

add_executable(FilterLayerTests
    tests/filter_layer_tests.cpp
    )


target_include_directories(FilterLayerTests PRIVATE
    ${PROJECT_SOURCE_DIR}/tests
)

target_link_libraries(FilterLayerTests PRIVATE
    filter-library
)

add_test(
    NAME FilterLayerTests
    COMMAND FilterLayerTests
)

add_executable(ControllerTests
    tests/controller_tests.cpp
)

target_link_libraries(ControllerTests PRIVATE
    sensor-library
)

add_test(
    NAME ControllerTests
    COMMAND ControllerTests
)

add_executable(HeaterControllerTests
    tests/heater_controller_tests.cpp
)

target_link_libraries(HeaterControllerTests PRIVATE
    sensor-library
)

add_test(
    NAME HeaterControllerTests
    COMMAND HeaterControllerTests
)


add_executable(ThermalPlantTests
    tests/thermal_plant_tests.cpp
)

target_link_libraries(ThermalPlantTests PRIVATE
    sensor-library
)

add_test(
    NAME ThermalPlantTests
    COMMAND ThermalPlantTests
)
