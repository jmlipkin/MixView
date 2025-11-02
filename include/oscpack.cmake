set(BASE ${CMAKE_CURRENT_LIST_DIR}/oscpack)

add_library(oscpack)

target_sources(oscpack
  PRIVATE
  ${BASE}/osc/OscTypes.h
  ${BASE}/osc/OscTypes.cpp
  ${BASE}/osc/OscHostEndianness.h
  ${BASE}/osc/OscException.h
  ${BASE}/osc/OscPacketListener.h
  ${BASE}/osc/MessageMappingOscPacketListener.h
  ${BASE}/osc/OscReceivedElements.h
  ${BASE}/osc/OscReceivedElements.cpp
  ${BASE}/osc/OscPrintReceivedElements.h
  ${BASE}/osc/OscPrintReceivedElements.cpp
  ${BASE}/osc/OscOutboundPacketStream.h
  ${BASE}/osc/OscOutboundPacketStream.cpp
)

target_include_directories(oscpack PRIVATE ${BASE})

target_include_directories(oscpack SYSTEM INTERFACE ${BASE})