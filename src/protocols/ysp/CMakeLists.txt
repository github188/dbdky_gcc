set(ysp_SRCS
  ysp.cpp
)

add_library(ysp SHARED ${ysp_SRCS})
install(TARGETS ysp DESTINATION lib)