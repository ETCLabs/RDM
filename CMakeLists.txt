cmake_minimum_required(VERSION 3.15)

########################## Global Project Attributes ##########################

project(RDM)

set(RDM_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(RDM_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(RDM_SRC ${CMAKE_CURRENT_LIST_DIR}/src)
set(RDM_CMAKE ${CMAKE_CURRENT_LIST_DIR}/cmake)

###################### Compile Options and Configuration ######################

option(RDM_BUILD_MOCK_LIB "Build the RDMMock library" OFF)
option(RDM_BUILD_TESTS "Build the RDM unit tests" OFF)

################################# Dependencies ################################

include(${RDM_CMAKE}/OssDependencyTools.cmake)
determine_compile_environment()

if(NOT COMPILING_AS_OSS)
  include(${RDM_CMAKE}/AddCMakeTools.cmake)
endif()

include(${RDM_CMAKE}/ResolveDependencies.cmake)

############################## Standalone support #############################

if(NOT COMPILING_AS_OSS)
  include(${CMAKE_TOOLS_MODULES}/QualityGateHelpers.cmake)

  setup_standalone_compile()
  setup_clang_format()
  # TODO: setup_clang_tidy()
  setup_address_sanitizer()
endif()

################################ Main libraries ###############################

add_subdirectory(src)

#################################### Tests ####################################

if(RDM_BUILD_TESTS)
  include(GoogleTest)
  enable_testing()
  add_subdirectory(tests)
endif()
