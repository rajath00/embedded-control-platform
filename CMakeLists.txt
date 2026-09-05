cmake_minimum_required(VERSION 3.10)
project(EmbeddedControlPlatform)

set(CMAKE_CXX_STANDARD 17)



add_executable( EmbeddedControl
src/main.cpp
)


target_include_directories(EmbeddedControl PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

link_directories(/usr/local/lib)

enable_testing()

add_executable(SensorLayerTests
    tests/sensor_layer_tests.cpp
)

target_include_directories(SensorLayerTests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME SensorLayerTests
    COMMAND SensorLayerTests
)
