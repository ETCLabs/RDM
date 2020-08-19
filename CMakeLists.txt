cmake_minimum_required(VERSION 3.3)

########################## Global Project Attributes ##########################

project(RDM VERSION 0.3.0)

if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  if (MSVC)
    add_compile_options(/W4)
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall)
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      add_compile_options(-Wno-sign-conversion)
    else()
      add_compile_options(-Wno-sign-compare)
    endif()
  endif()

  # Allow folder grouping in generated IDE projects
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Add a target to run Clang Format
  file(GLOB_RECURSE FILES_TO_REFORMAT 
    include/*.h
    src/*.c
    src/*.cpp
    src/*.h
    tests/*.c
    tests/*.cpp
    tests/*.h
  )
  add_custom_target(reformat_all COMMAND clang-format -i ${FILES_TO_REFORMAT})
  set_target_properties(reformat_all PROPERTIES FOLDER tools)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(RDM_ROOT ${CMAKE_CURRENT_LIST_DIR})

option(RDM_BUILD_MOCK_LIB "Build the RDMMock library" OFF)
option(RDM_BUILD_TESTS "Build the RDM unit tests" OFF)

# Main library

add_subdirectory(src)

# Tests

if(RDM_BUILD_TESTS)
  enable_testing()
  include(AddGoogleTest)
  add_subdirectory(tests)
endif()
