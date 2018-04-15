cmake_minimum_required(VERSION 2.8)

project(my)

find_package( OpenCV REQUIRED )
find_package( RapidJSON REQUIRED )

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++11")
endif()


include_directories("include"
                    ${OpenCV_INCLUDE_DIRS}
                    ${RapidJSON_INCLUDE_DIRS})

file(GLOB PROJECT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
# ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
add_executable( Test ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${PROJECT_SOURCE} )
target_link_libraries( Test  ${OpenCV_LIBS} ${RapidJSON_LIBS})