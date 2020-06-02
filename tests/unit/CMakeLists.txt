################################### testing ###################################

include(GoogleTest)

add_executable(test_rdm
  # RDM unit test sources
  test_cpp_command.cpp
  test_cpp_nack_reason.cpp
  test_cpp_response.cpp
  test_cpp_uid.cpp

  test_message.cpp
  test_responder.cpp
  test_uid.cpp
)
set_target_properties(test_rdm PROPERTIES
  FOLDER tests
  CXX_STANDARD 11
)
target_link_libraries(test_rdm RDM gtest_main)
gtest_add_tests(TARGET test_rdm SKIP_DEPENDENCY)
