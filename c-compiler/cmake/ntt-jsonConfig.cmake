if (ntt-json_FOUND)
    return()
endif()

set(NTT_JSON_NAME ntt-json)
set(NTT_JSON_DIR ${CMAKE_SOURCE_DIR}/vendors/json)

add_subdirectory(${NTT_JSON_DIR})

add_library(${NTT_JSON_NAME} INTERFACE)

target_link_libraries(
    ${NTT_JSON_NAME}
    INTERFACE 
    nlohmann_json::nlohmann_json
)

set(ntt-json_FOUND TRUE)