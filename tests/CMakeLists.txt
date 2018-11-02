################################### testing ###################################

enable_testing()

# Get and build the Googletest library at configure time: 
include(../tools/cmake/get_googletest.cmake)
get_googletest(${CMAKE_CURRENT_LIST_DIR}/..)

set(TEST_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/unit)

add_executable(test_rdm
  # lwpa unit test sources
  ${TEST_SRC_DIR}/main.cpp
  ${TEST_SRC_DIR}/test_uid.cpp
)

target_link_libraries(test_rdm RDM gtest)