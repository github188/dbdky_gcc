set(vaisala_SRCS
  vaisala.cpp
)

add_library(vaisala SHARED ${vaisala_SRCS})
install(TARGETS vaisala DESTINATION lib)