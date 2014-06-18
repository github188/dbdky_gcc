set(CodecMMXN_SRCS
  CodecMMXN.cpp
)

add_library(MMXN SHARED ${CodecMMXN_SRCS})
target_link_libraries(MMXN pthread m)
include_directories(..)
install(TARGETS MMXN DESTINATION lib)