set(hdsf6_SRCS
  hdsf6.cpp
)

add_library(hdsf6 SHARED ${hdsf6_SRCS})
install(TARGETS hdsf6 DESTINATION lib)