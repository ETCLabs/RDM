set(RDM_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(RDM_INCLUDE ${RDM_ROOT}/include)

################################# Dependencies ################################

if(NOT TARGET lwpa)
  if(NOT DEFINED LWPA_DIR)
    # Set a default for the lwpa location; assume it is named 'lwpa' and at the same level as RDM.
    set(LWPA_DIR ${RDM_ROOT}/../lwpa)
  endif()
  add_subdirectory(${LWPA_DIR} lwpa)
endif()

add_library(RDM
  # RDM headers
  ${RDM_INCLUDE}/rdm/controller.h
  ${RDM_INCLUDE}/rdm/defs.h
  ${RDM_INCLUDE}/rdm/message.h
  ${RDM_INCLUDE}/rdm/responder.h
  ${RDM_INCLUDE}/rdm/uid.h

  # RDM sources
  rdm_controller.c
  rdm_message.c
  rdm_responder.c
  rdm_uid.c
)
target_include_directories(RDM PUBLIC ${RDM_INCLUDE})
target_link_libraries(RDM PUBLIC lwpa)