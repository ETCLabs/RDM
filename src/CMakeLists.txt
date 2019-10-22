set(RDM_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(RDM_INCLUDE ${RDM_ROOT}/include)

################################# Dependencies ################################

include(RdmAddDependency)

rdm_add_dependency(EtcPal etcpal_DIR)

################################# RDM library #################################

add_library(RDMInterface INTERFACE)
target_include_directories(RDMInterface INTERFACE ${RDM_INCLUDE})

add_library(RDM
  # RDM headers
  ${RDM_INCLUDE}/rdm/controller.h
  ${RDM_INCLUDE}/rdm/defs.h
  ${RDM_INCLUDE}/rdm/message.h
  ${RDM_INCLUDE}/rdm/param_data.h
  ${RDM_INCLUDE}/rdm/responder.h
  ${RDM_INCLUDE}/rdm/uid.h

  # RDM sources
  rdm/controller.c
  rdm/message.c
  rdm/param_data.c
  rdm/responder.c
  rdm/uid.c
)
target_link_libraries(RDM PUBLIC RDMInterface EtcPal)

# Installation
install(TARGETS RDM ARCHIVE DESTINATION lib)
install(DIRECTORY ${RDM_ROOT}/include/rdm DESTINATION include FILES_MATCHING PATTERN "*.h")

############################### RDM mock library ##############################

if(RDM_BUILD_MOCK_LIB)
  add_library(RDMMock
    # RDM headers
    ${RDM_INCLUDE}/rdm/controller.h
    ${RDM_INCLUDE}/rdm/defs.h
    ${RDM_INCLUDE}/rdm/message.h
    ${RDM_INCLUDE}/rdm/responder.h
    ${RDM_INCLUDE}/rdm/uid.h

    # RDM sources
    # We will gradually substitute these with mocks as needed
    rdm/controller.c
    rdm/message.c
    rdm/responder.c
    rdm/uid.c
  )
  target_link_libraries(RDMMock PUBLIC RDMInterface EtcPalInterface)
endif()
