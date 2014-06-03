set(modbus_SRCS
  modbus.cpp
)

add_library(modbus SHARED ${modbus_SRCS})
install(TARGETS modbus DESTINATION lib)