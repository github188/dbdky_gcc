set(simple_ini_SRCS
  ConvertUTF.c
  )

add_library(simpleini ${simple_ini_SRCS})
install(TARGETS simpleini DESTINATION lib)

