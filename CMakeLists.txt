cmake_minimum_required(VERSION 3.16)
project(StarWallpaper C)

if(NOT WIN32)
    message(FATAL_ERROR
        "This initial native backend targets Windows. Linux (X11/Wayland) and macOS "
        "require separate desktop-window backends."
    )
endif()

add_executable(StarWallpaper WIN32 main.c)
set_property(TARGET StarWallpaper APPEND PROPERTY LINK_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/StarWallpaper.ico"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/StarWallpaperAbout.bmp"
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/Set-ExecutableIcon.ps1"
)
target_compile_features(StarWallpaper PRIVATE c_std_11)
target_compile_definitions(StarWallpaper PRIVATE
    UNICODE
    _UNICODE
)
find_program(POWERSHELL_EXECUTABLE NAMES powershell.exe)
if(NOT POWERSHELL_EXECUTABLE)
    message(FATAL_ERROR "Windows PowerShell is required to embed the application icon.")
endif()
add_custom_command(TARGET StarWallpaper POST_BUILD
    COMMAND "${POWERSHELL_EXECUTABLE}"
        -NoProfile
        -NonInteractive
        -ExecutionPolicy Bypass
        -File "${CMAKE_CURRENT_SOURCE_DIR}/scripts/Set-ExecutableIcon.ps1"
        -Executable "$<TARGET_FILE:StarWallpaper>"
        -Icon "${CMAKE_CURRENT_SOURCE_DIR}/assets/StarWallpaper.ico"
        -AboutBitmap "${CMAKE_CURRENT_SOURCE_DIR}/assets/StarWallpaperAbout.bmp"
    VERBATIM
)
target_link_libraries(StarWallpaper PRIVATE
    user32
    gdi32
    comdlg32
    advapi32
    shell32
)

if(MSVC)
    target_compile_options(StarWallpaper PRIVATE /W4 /O2)
    target_link_options(StarWallpaper PRIVATE /SUBSYSTEM:WINDOWS,5.01)
else()
    target_compile_options(StarWallpaper PRIVATE -Wall -Wextra -O2)
endif()

if(MINGW OR CMAKE_C_COMPILER_TARGET MATCHES "windows-gnu")
    target_link_options(StarWallpaper PRIVATE
        -municode
        -Wl,--subsystem,windows
        -Wl,--major-subsystem-version,5
        -Wl,--minor-subsystem-version,1
    )
endif()
