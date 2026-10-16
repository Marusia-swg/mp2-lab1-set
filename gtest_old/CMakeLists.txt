set(target "gtest")

add_library(${target} STATIC gtest-all.cc)

if(MSVC)
    target_compile_options(${target} PRIVATE /permissive /W0)
endif()

if(((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR
     ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang") AND
     (${CMAKE_SYSTEM_NAME} MATCHES "Linux")))
    set(pthread "-pthread")
endif()

target_link_libraries(${target} ${pthread})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)