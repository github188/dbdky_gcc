add_subdirectory(utils)
add_subdirectory(port)
add_subdirectory(dbhelper)


set(confUtil_SRCS
    ConfigUtil.cpp
)

include_directories(${THIRD_PARTY_DIR}/tinyxml)
add_library(confutil ${confUtil_SRCS})
target_link_libraries(confutil tinyxml utils)
install(TARGETS confutil DESTINATION lib)

set(com_service_SRCS
  com_service.cpp
  DataQueue.cpp
  Parser.cpp
  SerialPort.cpp
  com_main.cpp
  )

add_executable(dbdky_gcc ${com_service_SRCS})
target_link_libraries(dbdky_gcc confutil tinyxml port utils)
install(TARGETS dbdky_gcc DESTINATION bin)