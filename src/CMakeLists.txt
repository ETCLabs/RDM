set(RDM_INCLUDE ${RDM_ROOT}/include)

################################# RDM library #################################

set(RDM_LIB_TARGET_NAME RDM)

add_library(${RDM_LIB_TARGET_NAME}
  # RDM headers
  # ${RDM_INCLUDE}/rdm/controller.h
  ${RDM_INCLUDE}/rdm/defs.h
  ${RDM_INCLUDE}/rdm/message.h
  # ${RDM_INCLUDE}/rdm/responder.h
  ${RDM_INCLUDE}/rdm/uid.h

  # RDM sources
  # rdm/controller.c
  rdm/message.c
  # rdm/responder.c
  rdm/uid.c
)
target_include_directories(${RDM_LIB_TARGET_NAME} PUBLIC ${RDM_INCLUDE})
target_link_libraries(${RDM_LIB_TARGET_NAME} PUBLIC EtcPal)

# Write a pkg-config file
set(RDM_PKGCONFIG_INTERMEDIATE_FILE ${CMAKE_BINARY_DIR}/${RDM_LIB_TARGET_NAME}.pc)
configure_file(${RDM_ROOT}/cmake/RDM.pc.in ${RDM_PKGCONFIG_INTERMEDIATE_FILE} @ONLY)

# Installation
install(TARGETS ${RDM_LIB_TARGET_NAME} ARCHIVE DESTINATION lib)
install(DIRECTORY ${RDM_ROOT}/include/rdm DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${RDM_ROOT}/include/rdm/cpp DESTINATION include/rdm FILES_MATCHING PATTERN "*.h")
install(FILES ${RDM_PKGCONFIG_INTERMEDIATE_FILE} DESTINATION lib/pkgconfig)

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
    # rdm/controller.c
    # rdm/message.c
    # rdm/responder.c
    rdm/uid.c
  )
  target_include_directories(RDMMock PUBLIC ${RDM_INCLUDE})
endif()
