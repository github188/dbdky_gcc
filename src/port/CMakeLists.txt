set(port_SRCS
  Buffer.cpp
  EPollPoller.cpp
  EventLoopThreadPool.cpp
  Timer.cpp
  Channel.cpp
  EventLoop.cpp
  Poller.cpp
  TimerQueue.cpp
  DefaultPoller.cpp
  EventLoopThread.cpp
  PollPoller.cpp
  InetAddress.cpp
  SocketsOps.cpp
  TcpConnection.cpp
  Socket.cpp
  Acceptor.cpp
  TcpServer.cpp
  TcpClient.cpp
  Connector.cpp 
  UdpServer.cpp
  )

add_library(port ${port_SRCS})
target_link_libraries(port pthread rt)
install(TARGETS port DESTINATION lib)
