cmake_minimum_required(VERSION 3.19)

project(
    sciter-hello-world
    VERSION 0.1
    LANGUAGES CXX
)

include(FetchContent)

FetchContent_Declare(
    sciter-sdk
    URL https://sciter.com/sdk/sciter-sdk.zip
)

FetchContent_MakeAvailable(sciter-sdk)

if (UNIX)
    set(SCITER_SDK_PLATFORM_PATH ${sciter-sdk_SOURCE_DIR}/bin.lnx)
    set(SCITER_SDK_PACKFOLDER_PATH ${SCITER_SDK_PLATFORM_PATH}/packfolder)
    set(SCITER_SDK_DLL_NAME libsciter-gtk.so)
    set(SCITER_SDK_DLL_PATH ${SCITER_SDK_PLATFORM_PATH}/x64/${SCITER_SDK_DLL_NAME})

    file(CHMOD ${SCITER_SDK_PACKFOLDER_PATH} PERMISSIONS
        OWNER_WRITE
        OWNER_READ
        OWNER_EXECUTE
        GROUP_READ
        WORLD_READ)

    add_executable(hello
        main.cpp
        ${sciter-sdk_SOURCE_DIR}/include/sciter-gtk-main.cpp
    )
else() # WIN32
    set(SCITER_SDK_PLATFORM_PATH ${sciter-sdk_SOURCE_DIR}/bin.win)
    set(SCITER_SDK_PACKFOLDER_PATH ${SCITER_SDK_PLATFORM_PATH}/packfolder.exe)
    set(SCITER_SDK_DLL_NAME sciter.dll)
    set(SCITER_SDK_DLL_PATH ${SCITER_SDK_PLATFORM_PATH}/x64/${SCITER_SDK_DLL_NAME})

    add_compile_definitions(UNICODE)
    add_executable(hello WIN32
        main.cpp
        ${sciter-sdk_SOURCE_DIR}/include/sciter-win-main.cpp
    )
endif() # TODO: MacOS

add_custom_target(
    ui
    COMMAND "${SCITER_SDK_PACKFOLDER_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/ui" resources.cpp -v resources
    COMMENT "Generating resources.cpp file"
)

add_dependencies(hello ui)

set(HELLO_INCLUDES ${sciter-sdk_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
set(HELLO_LIBS ${CMAKE_DL_LIBS})

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    link_directories(${GTK3_LIBRARY_DIRS})

    list(APPEND HELLO_INCLUDES ${GTK3_INCLUDE_DIRS})
    list(APPEND HELLO_LIBS ${GTK3_LIBRARIES})
endif()

include_directories(${HELLO_INCLUDES})

target_link_libraries(hello ${HELLO_LIBS})

add_custom_command(TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SCITER_SDK_DLL_PATH}
        $<TARGET_FILE_DIR:hello>
)
