
set(target "gtest")

file(GLOB GTEST_SRCS "src/*.cc")
file(GLOB GTEST_HDRS "include/gtest/*.h" "include/gtest/internal/*.h")

add_library(${target} STATIC ${GTEST_SRCS} ${GTEST_HDRS})

target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(${target} PRIVATE /W0)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${target} pthread)
endif()