cmake_minimum_required(VERSION 3.18)

project(pyiceoryx)

find_package(iceoryx_platform REQUIRED)
find_package(iceoryx_hoofs REQUIRED)
find_package(iceoryx_posh REQUIRED)

include(IceoryxPackageHelper)
include(IceoryxPlatform)
include(IceoryxPlatformSettings)
include(IceoryxPoshDeployment)

if(CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME MATCHES Darwin)
    option(BUILD_SHARED_LIBS "Create shared libraries by default" ON)
endif()

set(CMAKE_CXX_STANDARD 14)

find_package(pybind11 REQUIRED CONFIG)
pybind11_add_module(pyiceoryx wrapper.cpp)
