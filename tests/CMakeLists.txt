################################### testing ###################################

include(GoogleTest)

set(TEST_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/unit)

add_executable(test_rdm
  # RDM unit test sources
  ${TEST_SRC_DIR}/main.cpp
  ${TEST_SRC_DIR}/test_responder.cpp
  ${TEST_SRC_DIR}/test_uid.cpp
)
set_target_properties(test_rdm PROPERTIES
  FOLDER tests
  CXX_STANDARD 14
)
target_link_libraries(test_rdm RDM gtest)
gtest_add_tests(TARGET test_rdm SKIP_DEPENDENCY)
