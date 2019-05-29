################################### testing ###################################

enable_testing()

message(STATUS "Adding GoogleTest dependency...")
include(AddGoogleTest)

set(TEST_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/unit)

add_executable(test_rdm
  # RDM unit test sources
  ${TEST_SRC_DIR}/main.cpp
  ${TEST_SRC_DIR}/test_responder.cpp
  ${TEST_SRC_DIR}/test_uid.cpp
)

target_link_libraries(test_rdm RDM gtest)
