set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wno-format -g -DTIXML_USE_STL -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-unknown-programs -Wno-format -O3 -DTIXML_USE_STL")

set(tinyxml_lib_SRCS
  tinyxml.cpp
  tinyxmlparser.cpp
  tinyxmlerror.cpp
  tinystr.cpp
)

add_library(tinyxml ${tinyxml_lib_SRCS})
install(TARGETS tinyxml DESTINATION lib)

add_executable(xmltest xmltest.cpp)
target_link_libraries(xmltest tinyxml)
install(TARGETS xmltest DESTINATION bin)
