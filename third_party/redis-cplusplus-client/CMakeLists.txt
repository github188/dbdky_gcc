set(redisclient_lib_SRCS
  anet.c
)

add_library(redisclient ${redisclient_lib_SRCS})
#target_link_libraries(redisclient pthread boost_thread-mt)
target_link_libraries(redisclient pthread)
install(TARGETS redisclient DESTINATION lib)

set(redisclient_test_SRCS
  tests/test.cpp
)

add_executable(redisclient_test ${redisclient_test_SRCS})
include_directories(.)
target_link_libraries(redisclient_test redisclient)
install(TARGETS redisclient_test DESTINATION bin)
