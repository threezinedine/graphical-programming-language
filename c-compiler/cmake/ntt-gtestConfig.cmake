if (ntt-gtest_FOUND)
    return()
endif()

set(NTT_GTEST_NAME ntt-gtest)
set(NTT_GTEST_DIR ${CMAKE_CURRENT_LIST_DIR}/../vendors/gtest)

set(gtest_force_shared_crt ON)
add_subdirectory(${NTT_GTEST_DIR})
if (MSVC)
endif()

add_library(${NTT_GTEST_NAME} INTERFACE)
target_link_libraries(
    ${NTT_GTEST_NAME} 
    INTERFACE
    gtest
    gtest_main
    gmock
    gmock_main
)

set(ntt-gtest_FOUND TRUE)