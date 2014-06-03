set(weishuimidu_SRCS
  weishuimidu.cpp
)

add_library(weishuimidu SHARED ${weishuimidu_SRCS})
install(TARGETS weishuimidu DESTINATION lib)