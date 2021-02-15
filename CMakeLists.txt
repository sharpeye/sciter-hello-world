cmake_minimum_required(VERSION 3.19)

project(
    sciter-hello-world
    VERSION 0.1
    LANGUAGES CXX
)

include(FetchContent)

FetchContent_Declare(
    sciter-sdk
    GIT_REPOSITORY https://github.com/c-smile/sciter-sdk.git
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(sciter-sdk)

set(SCITER_SDK_PLATFORM_PATH ${sciter-sdk_SOURCE_DIR}/bin.lnx)
set(SCITER_SDK_PACKFOLDER_PATH ${SCITER_SDK_PLATFORM_PATH}/packfolder)
set(SCITER_SDK_DLL_NAME libsciter-gtk.so)
set(SCITER_SDK_DLL_PATH ${SCITER_SDK_PLATFORM_PATH}/x64/${SCITER_SDK_DLL_NAME})

file(CHMOD ${SCITER_SDK_PACKFOLDER_PATH} PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ WORLD_READ)
file(CREATE_LINK ${SCITER_SDK_DLL_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${SCITER_SDK_DLL_NAME})

add_executable(hello
    main.cpp
    ${sciter-sdk_SOURCE_DIR}/include/sciter-gtk-main.cpp
)

add_custom_target(
    ui
    COMMAND "${SCITER_SDK_PACKFOLDER_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/ui" "${CMAKE_CURRENT_SOURCE_DIR}/resources.cpp" -v "resources"
    COMMENT "Generating resources.cpp file"
)

add_dependencies(hello ui)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

link_directories(${GTK3_LIBRARY_DIRS})

include_directories(
    ${GTK3_INCLUDE_DIRS}
    ${sciter-sdk_SOURCE_DIR}/include
)

target_link_libraries(hello
    ${GTK3_LIBRARIES}
    ${CMAKE_DL_LIBS}
)
