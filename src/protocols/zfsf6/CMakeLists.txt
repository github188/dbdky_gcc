set(zfsf6_SRCS
  zfsf6.cpp
)

add_library(zfsf6 SHARED ${zfsf6_SRCS})
install(TARGETS zfsf6 DESTINATION lib)