project(test)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Threads)

add_executable(test_cli   test.cpp )

target_include_directories(test_cli  PRIVATE ../include)
target_include_directories(test_cli  PRIVATE ${Boost_INCLUDE_DIR})


set_target_properties(test_cli  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
)
#set (CMAKE_CXX_FLAGS "-fsanitize=thread")

target_link_libraries(test_cli
  ${CMAKE_THREAD_LIBS_INIT} 
  ${Boost_LIBRARIES}
)

enable_testing()

add_test(test          test_cli  -t test_suite_main_1)