#include <iostream>
#include <algorithm>

#include "map_reduce.h"

class Map
{

public:
    auto operator()(std::string s)
    {
        return contaner_t{s};
    } 
};


class Reduce
{
    size_t max_prefix;
    std::string s_prv;

public:
    Reduce():max_prefix(1), s_prv(" ") {}
    
    auto operator()(std::string s)
    {
        size_t prefix = std::distance(s.cbegin(),
                                      std::mismatch(s.cbegin(), s.cend(),
                                                    s_prv.cbegin(), s_prv.cend()).first);
        if (prefix + 1 > max_prefix){
            max_prefix = prefix + 1;
        }
        s_prv = s;
        return contaner_t{std::to_string(max_prefix)};
    }    
};


void process(const char* file_arg, const char* mnum_arg, const char* rnum_arg)
{  
  
    std::string str_mnum = mnum_arg;
    decltype(std::stoull("")) mnum;
    
    try {
        if(!std::all_of(str_mnum.begin(), str_mnum.end(), ::isdigit))
            throw std::invalid_argument("");
        mnum = std::stoull(str_mnum);
        if(mnum == 0)
            throw std::invalid_argument("");
    }    
    catch(const std::exception &e) {
        throw std::invalid_argument("Invalid <mnum>");
    } 


    std::string str_rnum = rnum_arg;
    decltype(std::stoull("")) rnum;

    try {
        if(!std::all_of(str_rnum.begin(), str_rnum.end(), ::isdigit))
            throw std::invalid_argument("");
        rnum = std::stoull(str_rnum);
        if(rnum == 0)
            throw std::invalid_argument("");
    }    
    catch(const std::exception &e) {
        throw std::invalid_argument("Invalid <rnum>");
    } 

    map_reduce<Map, Reduce> framework(file_arg, mnum, rnum);
    framework.run();
}