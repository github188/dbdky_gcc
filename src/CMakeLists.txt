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

