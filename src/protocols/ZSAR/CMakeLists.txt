set(CodecZSAR_SRCS
  CodecZSAR.cpp
)

add_library(ZSAR SHARED ${CodecZSAR_SRCS})
target_link_libraries(ZSAR pthread m)
include_directories(..)
install(TARGETS ZSAR DESTINATION lib)