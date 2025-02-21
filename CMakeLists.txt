cmake_minimum_required(VERSION 3.30)

project(chatServer VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.8.2 REQUIRED COMPONENTS system filesystem)
find_package(jsoncpp 1.9.6 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
find_package(gRPC CONFIG REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS} ${PROTOBUF_INCLUDE_DIRS} ${GRPC_INCLUDE_DIRS})

#protobuf_generate_cpp(PROTO_SRC PROTO_HEADER "./src/message.proto")



file(GLOB RES_SRC "./src/*.cpp" "./src/*.cc") 

add_executable(chatServer
        ${RES_SRC}
)

target_link_libraries(chatServer
 ${Boost_LIBRARIES} 
 jsoncpp
  ${Protobuf_LIBRARIES}
  gRPC::grpc++ 

  )
