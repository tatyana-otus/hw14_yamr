#include "functions.h"


#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_suite_main_1)

void check(std::string file_name, int m, int r, int expected_res)
{
    std::system("rm -f  red_*");

    std::string res;

    process(file_name.c_str(), std::to_string(m).c_str(), std::to_string(r).c_str());

    std::system("cat red_out_* >> red_out_tmp");  
    std::system("sort -nr -o red_out red_out_tmp"); 

    auto f = std::ifstream("red_out");
    std::getline(f, res);
    f.close();

    BOOST_CHECK_EQUAL(res, std::to_string(expected_res));
}


BOOST_AUTO_TEST_CASE(test_1)
{  
    std::system("rm -f  test_in red_*");

    std::string data = 
    "a@gmail.com\n"
    "a@mail.ru\n"
    "ab@gmail.com\n"
    "abc@gmail.com\n"
    "abcd@gmail.com\n"
    "b@gmail.com\n"
    "bc@gmail.com\n"
    "bcd@gmail.com\n"
    "bcde@gmail.com\n"
    "i@gmail.com\n"
    "ik@gmail.com\n"
    "im@gmail.com\n"
    "ih@gmail.com\n"
    "e@gmail.com\n"
    "gogp@gmail.com\n"
    "gerty@gmail.com\n"
    "gmkt@gmail.com\n";
    
    auto f = std::ofstream("test_in");
    std::stringstream ss;
    ss << data;
    f << ss.rdbuf();
    f.close();
  
    check("test_in", 1, 1,  4);
    check("test_in", 2, 1,  4);
    check("test_in", 4, 2,  4);
    check("test_in", 4, 4,  4);
    check("test_in", 4, 30, 4);
}


BOOST_AUTO_TEST_CASE(test_2)
{  
    std::system("rm -f  test_in red_*");

    std::system("echo \"123456789\"  > test_in");
    std::system("cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 16 | head -n 2000 >> test_in");
    std::system("echo \"1234567890\" >> test_in");
  
    check("test_in", 1,   1,  10);
    check("test_in", 5,   5,  10);
    check("test_in", 10,  10, 10);
    check("test_in", 100, 20, 10);
    check("test_in", 200, 40, 10);
}


BOOST_AUTO_TEST_CASE(test_3)
{  
    std::system("rm -f  test_in red_*");

    std::system("echo \"a\"  > test_in");
    
    check("test_in", 1, 1,  1);
    check("test_in", 1, 10, 1);
}

BOOST_AUTO_TEST_SUITE_END()