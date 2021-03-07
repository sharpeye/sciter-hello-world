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

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    target_include_directories(hello PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(hello PRIVATE ${GTK3_LIBRARIES})

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

target_include_directories(hello PRIVATE
    ${sciter-sdk_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(hello PRIVATE ${CMAKE_DL_LIBS})

add_custom_command(TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SCITER_SDK_DLL_PATH}
        $<TARGET_FILE_DIR:hello>
)
