set(lamodbus_SRCS
  lamodbus.cpp
)

add_library(lamodbus SHARED ${lamodbus_SRCS})
install(TARGETS lamodbus DESTINATION lib)