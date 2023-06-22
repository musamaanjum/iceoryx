cmake_minimum_required(VERSION 3.18)
set(PREFIX iceoryx/v${CMAKE_PROJECT_VERSION})
# set(CMAKE_CXX_STANDARD 14)
set(IOX_VERSION_STRING "2.90.0")

project(iceoryx_binding_python)

find_package(iceoryx_platform REQUIRED)
find_package(iceoryx_hoofs REQUIRED)
find_package(iceoryx_posh REQUIRED)
find_package(iceoryx_dust REQUIRED)

include(IceoryxPackageHelper)
include(IceoryxPlatform)
include(IceoryxPlatformSettings)
include(IceoryxPoshDeployment)

if(CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME MATCHES Darwin)
    option(BUILD_SHARED_LIBS "Create shared libraries by default" ON)
endif()

find_package(pybind11 REQUIRED CONFIG)
pybind11_add_module(iceoryx_binding_python wrapper.cpp)

target_include_directories(iceoryx_binding_python
			   PUBLIC
			   ${CMAKE_CURRENT_SOURCE_DIR}/include 
			   ${CMAKE_BINARY_DIR}/generated/iceoryx/include
			   ../iceoryx_hoofs/include
			   ../iceoryx_platform/linux/include
			   ../iceoryx_posh/include
			   ../iceoryx_dust/include
			   ../iceoryx_hoofs/primitives/include
			   ../iceoryx_hoofs/vocabulary/include
			   ../iceoryx_hoofs/design/include
			   ../iceoryx_hoofs/functional/include
			   ../iceoryx_hoofs/reporting/include
			   ../iceoryx_hoofs/utility/include
			   ../generated/iceoryx_hoofs/include
			   ../iceoryx_hoofs/legacy/include
			   ../iceoryx_hoofs/container/include
			   ../iceoryx_hoofs/time/include
			   ../iceoryx_hoofs/memory/include
			   ../iceoryx_hoofs/posix/time/include
			   ../iceoryx_hoofs/filesystem/include
			   ../iceoryx_hoofs/posix/design/include
			   ../iceoryx_hoofs/posix/vocabulary/include/
			   )

# iox_add_library(
#     TARGET              ${PROJECT_NAME}
#     NAMESPACE           ${PROJECT_NAME}
#     PROJECT_PREFIX      ${PREFIX}
#     BUILD_INTERFACE     ${CMAKE_CURRENT_SOURCE_DIR}/include
#                         ${CMAKE_BINARY_DIR}/generated/iceoryx/include
#     INSTALL_INTERFACE   include/${PREFIX}
#     PUBLIC_LIBS_LINUX   stdc++ pthread
#     PRIVATE_LIBS        iceoryx_posh::iceoryx_posh
#     FILES
#         wrapper.cpp
# )

# set(TEST_LINK_LIBS
#     iceoryx_hoofs::iceoryx_hoofs
#     iceoryx_posh::iceoryx_posh
#     iceoryx_posh::iceoryx_posh_config
#     iceoryx_posh::iceoryx_posh_roudi
# )

# include_directories(..)

