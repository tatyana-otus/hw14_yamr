cmake_minimum_required(VERSION 3.2)

project(yamr VERSION 1.0.$ENV{TRAVIS_BUILD_NUMBER})

add_executable(yamr main.cpp)

find_package(Threads)

target_include_directories(yamr PRIVATE ./include)

set_target_properties(yamr PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wall -Wextra -Wpedantic 
)

target_link_libraries(yamr
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

#set (CMAKE_CXX_FLAGS "-fsanitize=thread")

install(TARGETS yamr RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT lisstic.tt@gmail.com)

include (CPack)

add_subdirectory(tests)
enable_testing()
