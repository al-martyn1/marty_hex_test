cmake_minimum_required(VERSION 3.21.0)
project(umba-hex VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# add_compile_definitions(MARTY_BIGINT_FORCE_NUMBER_UNDERLYING_TYPE=std::uint8_t)


set(UMBA_STATIC_RUNTIME ON)
set(UMBA_USE_BOOST      ON)

include(${CMAKE_CURRENT_LIST_DIR}/.cmake/umba.cmake)


include_directories(${LIB_ROOT})

# Import libraries here
add_subdirectory(${LIB_ROOT}/encoding)
add_subdirectory(${LIB_ROOT}/marty_bigint)
add_subdirectory(${LIB_ROOT}/marty_cpp)
add_subdirectory(${LIB_ROOT}/marty_crc)
add_subdirectory(${LIB_ROOT}/marty_decimal)
add_subdirectory(${LIB_ROOT}/marty_hex)
add_subdirectory(${LIB_ROOT}/marty_mem)
add_subdirectory(${LIB_ROOT}/marty_svg)
add_subdirectory(${LIB_ROOT}/marty_utf)
add_subdirectory(${LIB_ROOT}/marty_format)
add_subdirectory(${LIB_ROOT}/marty_expressions)
# add_subdirectory(${LIB_ROOT}/marty_pugixml)
# add_subdirectory(${LIB_ROOT}/marty_tr)
# add_subdirectory(${LIB_ROOT}/marty_yaml_toml_json)
# add_subdirectory(${LIB_ROOT}/nlohmann)    # https://json.nlohmann.me/integration/cmake/#external
# add_subdirectory(${LIB_ROOT}/pugixml)
add_subdirectory(${LIB_ROOT}/sfmt)
add_subdirectory(${LIB_ROOT}/umba)
add_subdirectory(${LIB_ROOT}/umba_tokenizer)
# add_subdirectory(${LIB_ROOT}/yaml-cpp)

# umba_configure_boost("STATIC_LIBS" "MULTITHREADED" "${STATIC_RUNTIME}")

# add_compile_definitions("YAML_CPP_STATIC_DEFINE")
# add_compile_definitions("USE_UMBA_LOUT_COUT")
# add_compile_definitions("UMBA_TOKENIZER_DISABLE_UMBA_TOKENIZER_GET_CHAR_CLASS_FUNCTION")
# add_compile_definitions("UMBA_TOKEN_TRIE_FIND_NEXT_BINARY_SEARCH_DISABLED")


# Configure libraries here
set(COMMON_LIBS encoding::encoding sfmt::sfmt umba
    marty::bigint
    marty::cpp
    marty::crc
    marty::decimal
    marty::expressions
    marty::hex
    marty::mem
    marty::svg
    umba::tokenizer
    marty::utf
    marty::format
   )
# set(PUGIXML_LIB pugixml::pugixml marty_pugixml::marty_pugixml)
# set(JSON_YAML_LIB nlohmann_json::nlohmann_json yaml-cpp::yaml-cpp)


file(GLOB_RECURSE sources   "${SRC_ROOT}/*.cpp")
file(GLOB_RECURSE headers   "${SRC_ROOT}/*.h")
file(GLOB_RECURSE resources "${SRC_ROOT}/*.rc")
umba_make_sources_tree("${SRC_ROOT}" "${sources}" "${headers}" "${resources}")


# Docs

file(GLOB_RECURSE md_docs  "${CMAKE_CURRENT_LIST_DIR}/doc*/*.md" "${CMAKE_CURRENT_LIST_DIR}/_src/*.md" "${CMAKE_CURRENT_LIST_DIR}/src/*.md" "${CMAKE_CURRENT_LIST_DIR}/doc*/*.txt")
file(GLOB         md2_docs "${CMAKE_CURRENT_LIST_DIR}/*.md")
set(all_docs ${md_docs} ${md2_docs})
list(FILTER all_docs EXCLUDE REGEX ".*CMakeLists\.txt$")
list(FILTER all_docs EXCLUDE REGEX ".*_generators/.*")
list(FILTER all_docs EXCLUDE REGEX ".*examples/.*")
list(FILTER all_docs EXCLUDE REGEX ".*md_/.*$")
list(FILTER all_docs EXCLUDE REGEX ".*_md/.*$")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Docs" FILES ${all_docs})

# Doc sources

file(GLOB_RECURSE md_src1  "${CMAKE_CURRENT_LIST_DIR}/doc*/*.md_" "${CMAKE_CURRENT_LIST_DIR}/doc*/*._md" "${CMAKE_CURRENT_LIST_DIR}/_src/*.md_" "${CMAKE_CURRENT_LIST_DIR}/_src/*._md" "${CMAKE_CURRENT_LIST_DIR}/src/*.md_" "${CMAKE_CURRENT_LIST_DIR}/src/*._md")
file(GLOB         md_src2 "${CMAKE_CURRENT_LIST_DIR}/*.md_" "${CMAKE_CURRENT_LIST_DIR}/*._md")
set(all_docs_src ${md_src1} ${md_src2})


list(FILTER all_docs_src EXCLUDE REGEX ".*_generators/.*")
list(FILTER all_docs_src EXCLUDE REGEX ".*examples/.*")
list(FILTER all_docs_src EXCLUDE REGEX ".*md_/.*")
list(FILTER all_docs_src EXCLUDE REGEX ".*_md/.*")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Docs.src" FILES ${all_docs_src})



set(PLATFORM_LIBS)
if(WIN32) # Всё нормально, это про таргет
    list(APPEND PLATFORM_LIBS ws2_32 iphlpapi shell32)
endif()


set(COMMON_UMBA_OPTIONS "BIGOBJ" "UTF8" "PEDANTIC" "WERR") # "PEDANTIC"/"WALL" "WERR"


# if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

set(TESTS "ALL")

if (TESTS STREQUAL "ALL")


set(TARGET marty-hex-test-001)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-002)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-003)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-004)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-005)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-006)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-007)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-008)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-009)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-010)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-011)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-012)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-013)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-014)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-015)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}")
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-016)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-017)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-018)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-019)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-020)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")

endif()


set(TARGET marty-hex-test-021)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-022)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-023)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-024)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-025)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-026)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-027)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-028)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-029)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-030)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-031)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-032)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-033)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET marty-hex-test-034)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")





#------------------------------------------------------------------------------------------------
if (TESTS STREQUAL "ALL")

set(TARGET mfmt_example_01)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_02)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_03_generate)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_03)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_04_generate)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_04)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_05)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_06)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_07)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_08)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET mfmt_example_09)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/marty_format_examples/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


set(TARGET std_to_chars_tests)
add_executable(${TARGET} "${SRC_ROOT}/umba-hex/std_format_tests/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")


endif()



if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(TARGET gcvt_test)
  add_executable(${TARGET} "${SRC_ROOT}/umba-hex/std_format_tests/${TARGET}.cpp" "${headers}" ${all_docs} ${all_docs_src})
  umba_add_target_options(${TARGET} ${COMMON_UMBA_OPTIONS} "CONSOLE") 
  target_link_libraries(${TARGET} PRIVATE "${PLATFORM_LIBS}" "${COMMON_LIBS}")
endif()




