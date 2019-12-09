################################### testing ###################################

include(GoogleTest)

add_executable(test_rdm
  # RDM unit test sources
  main.cpp
  test_responder.cpp
  test_uid.cpp
  test_cpp_uid.cpp
)
set_target_properties(test_rdm PROPERTIES
  FOLDER tests
  CXX_STANDARD 14
)
target_link_libraries(test_rdm RDM gtest)
gtest_add_tests(TARGET test_rdm SKIP_DEPENDENCY)
