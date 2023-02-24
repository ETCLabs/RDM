# GHRDM source manifest

set(RDM_HEADERS
  ${RDM_INCLUDE}/rdm/cpp/message_types/command_header.h
  ${RDM_INCLUDE}/rdm/cpp/message_types/command.h
  ${RDM_INCLUDE}/rdm/cpp/message_types/nack_reason.h
  ${RDM_INCLUDE}/rdm/cpp/message_types/response_header.h
  ${RDM_INCLUDE}/rdm/cpp/message_types/response.h
  ${RDM_INCLUDE}/rdm/cpp/message.h
  ${RDM_INCLUDE}/rdm/cpp/uid.h
  ${RDM_INCLUDE}/rdm/controller.h
  ${RDM_INCLUDE}/rdm/defs.h
  ${RDM_INCLUDE}/rdm/message.h
  ${RDM_INCLUDE}/rdm/responder.h
  ${RDM_INCLUDE}/rdm/uid.h
  ${RDM_INCLUDE}/rdm/version.h
)

set(RDM_SOURCES
  ${RDM_SRC}/rdm/controller.c
  ${RDM_SRC}/rdm/message.c
  ${RDM_SRC}/rdm/responder.c
  ${RDM_SRC}/rdm/uid.c
)
