set(dbhelper_SRCS
  dbhelper.cpp
)

link_directories(/usr/lib/mysql)
add_library(dbhelper ${dbhelper_SRCS})
target_link_libraries(dbhelper mysqlcppconn)
install(TARGETS dbhelper DESTINATION lib)
