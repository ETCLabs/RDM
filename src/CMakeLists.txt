set(RDM_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)
set(RDM_INCLUDE ${RDM_ROOT}/include)

################################# Dependencies ################################

function(rdm_add_dependency target loc_variable)
  # Step 1 - see if the target has already been included
  if(NOT TARGET ${target})
    message(STATUS "Attempting to locate dependency ${target}...")
    message(STATUS "Checking user-provided variable ${loc_variable}...")
    # Step 2 - if the user has provided a location for the dependency, use that
    if(DEFINED ${loc_variable})
      message(STATUS "Found. Adding dependency from ${${loc_variable}}.")
      add_subdirectory(${${loc_variable}} ${target})
    else()
      message(STATUS "Not defined. Checking if already downloaded at the same level as this directory...")
      # Step 3 - look for the dependency in a folder with the same name as the
      # target, at the same level as this folder
      if (EXISTS ${RDM_ROOT}/../${target})
        message(STATUS "Found. Adding dependency from RDM_DIR/../${target}.")
        add_subdirectory(${RDM_ROOT}/../${target} ${target})
      else()
        # TODO: add package manager step
        message(FATAL_ERROR "Dependency ${target} not satisfied.")
      endif()
    endif()
  endif()
endfunction()

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
  rdm_controller.c
  rdm_message.c
  rdm_responder.c
  rdm_uid.c
)
target_include_directories(RDM PUBLIC ${RDM_INCLUDE})
target_link_libraries(RDM PUBLIC lwpa)