set(tmpwia_SRCS
  tmpwia.cpp
)

add_library(tmpwia SHARED ${tmpwia_SRCS})
install(TARGETS tmpwia DESTINATION lib)