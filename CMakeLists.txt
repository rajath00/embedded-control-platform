cmake_minimum_required(VERSION 3.10)
project(EmbeddedControlPlatform)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_CLANG_TIDY "Run clang-tidy during builds" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXECUTABLE clang-tidy REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
endif()

if(ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

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
    ${PROJECT_SOURCE_DIR}/tests
)

add_test(
    NAME SensorLayerTests
    COMMAND SensorLayerTests
)



add_executable(FilterLayerTests
    tests/filter_layer_tests.cpp
    src/filters/filter_layer.cpp
    )


target_include_directories(FilterLayerTests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests
)

add_test(
    NAME FilterLayerTests
    COMMAND FilterLayerTests
)