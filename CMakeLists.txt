cmake_minimum_required(VERSION 3.10)
project(EmbeddedControlPlatform)

set(CMAKE_CXX_STANDARD 17)



add_executable( EmbeddedControl
    src/main.cpp
    src/filters/filter_layer.cpp
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

# add_test(
#     NAME SensorLayerTests
#     COMMAND SensorLayerTests
# )



add_executable(FilterLayerTests
    tests/filter_layer_tests.cpp
    src/filters/filter_layer.cpp
    )


target_include_directories(FilterLayerTests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME FilterLayerTests
    COMMAND FilterLayerTests
)