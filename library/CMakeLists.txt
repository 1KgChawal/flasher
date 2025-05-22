add_library(
    lprint SHARED
    src/print/print.c++
    src/print/print.h++
)

target_include_directories(lprint PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(
    lfile SHARED
    src/file/file.c++
    src/file/file.h++
)

target_include_directories(lfile PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")