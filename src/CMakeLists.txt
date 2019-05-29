set(RDM_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(RDM_INCLUDE ${RDM_ROOT}/include)

################################# Dependencies ################################

include(RdmAddDependency)

# option(RDM_BUILD_MOCK_LIB "Build the RDM_mock library" OFF)
#
# if(RDM_BUILD_TESTS OR RDM_BUILD_MOCK_LIB)
#   set(LWPA_BUILD_MOCK_LIB ON)
# endif()
rdm_add_dependency(lwpa LWPA_DIR)

################################# RDM library #################################

add_library(RDM
  # RDM headers
  ${RDM_INCLUDE}/rdm/controller.h
  ${RDM_INCLUDE}/rdm/defs.h
  ${RDM_INCLUDE}/rdm/message.h
  ${RDM_INCLUDE}/rdm/responder.h
  ${RDM_INCLUDE}/rdm/uid.h

  # RDM sources
  rdm/controller.c
  rdm/message.c
  rdm/responder.c
  rdm/uid.c
)
target_include_directories(RDM PUBLIC ${RDM_INCLUDE})
target_link_libraries(RDM PUBLIC lwpa)

############################### RDM mock library ##############################

# if(RDM_BUILD_MOCK_LIB)
#   add_library(RDM_mock
#     # RDM headers
#     ${RDM_INCLUDE}/rdm/controller.h
#     ${RDM_INCLUDE}/rdm/defs.h
#     ${RDM_INCLUDE}/rdm/message.h
#     ${RDM_INCLUDE}/rdm/responder.h
#     ${RDM_INCLUDE}/rdm/uid.h
# 
#     # RDM sources
#     # We will gradually substitute these with mocks as needed
#     rdm/controller.c
#     rdm/message.c
#     rdm/responder.c
#     rdm/uid.c
#   )
#   target_include_directories(RDM_mock PUBLIC ${RDM_INCLUDE})
#   target_link_libraries(RDM_mock PUBLIC lwpa_mock)
# endif()
