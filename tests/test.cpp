#include "functions.h"


#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_suite_main_1)


BOOST_AUTO_TEST_CASE(test_1)
{  
    std::system("rm -f  test_in red_*");

    {
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
    }

    
    std::string data =  "ab@\n"
                        "abc@\n"
                        "abcd\n"
                        "a@g\n"
                        "a@m\n"
                        "b@\n"
                        "bc@\n"
                        "bcd@\n"
                        "bcde\n"
                        "e\n"
                        "ge\n"
                        "gm\n"
                        "go\n"
                        "i@\n"
                        "ih\n"
                        "ik\n"
                        "im\n";

    {
        process("test_in", "1", "1");
        std::system("sort -o red_out red_out_0");

        auto f = std::ifstream("red_out");
        std::stringstream ss;
        ss << f.rdbuf();
        f.close();

        BOOST_CHECK_EQUAL(ss.str(), data);
    }    

    std::system("rm -f  red_out*");
    {
        process("test_in", "2", "1");
        std::system("sort -o red_out red_out_0");
        auto f = std::ifstream("red_out");
        std::stringstream ss;
        ss << f.rdbuf();
        f.close();

        BOOST_CHECK_EQUAL(ss.str(), data);
    } 

    std::system("rm -f  red_out*");
    {
        process("test_in", "4", "1");
        std::system("sort -o red_out red_out_0");
        auto f = std::ifstream("red_out");
        std::stringstream ss;
        ss << f.rdbuf();
        f.close();

        BOOST_CHECK_EQUAL(ss.str(), data);
    } 


    std::system("rm -f  red_out*");
    {
        process("test_in", "4", "4");

        std::system("cat red_out_* >> red_out_tmp");  
        std::system("sort -o red_out red_out_tmp");    
        auto f = std::ifstream("red_out");
        std::stringstream ss;
        ss << f.rdbuf();
        f.close();

        BOOST_CHECK_EQUAL(ss.str(), data);
    } 

    std::system("rm -f  red_out*");
    {
        process("test_in", "4", "30");

        std::system("cat red_out_* >> red_out_tmp");  
        std::system("sort -o red_out red_out_tmp");    
        auto f = std::ifstream("red_out");
        std::stringstream ss;
        ss << f.rdbuf();
        f.close();

        BOOST_CHECK_EQUAL(ss.str(), data);
    }
}

BOOST_AUTO_TEST_SUITE_END()