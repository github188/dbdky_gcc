set(protocol_codec_test_SRCS
  main.cpp
)

add_executable(protocol_codec_test ${protocol_codec_test_SRCS})
include_directories(../MMXN ../ZSAR)
target_link_libraries(protocol_codec_test pthread m dl)
install(TARGETS protocol_codec_test DESTINATION bin)