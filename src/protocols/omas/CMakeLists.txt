set(omas_SRCS
  omas.cpp
)

add_library(omas SHARED ${omas_SRCS})
install(TARGETS omas DESTINATION lib)