set(kelmandga_SRCS
  kelmandga.cpp
)

add_library(kelmandga SHARED ${kelmandga_SRCS})
install(TARGETS kelmandga DESTINATION lib)