#include <iostream>
#include <algorithm>

#include "map_reduce.h"

class Map
{

public:
    auto operator()(std::string s)
    {
        contaner_t list;   

        std::generate_n(std::back_inserter(list), s.size(),
                        [&s, idx = 1] () mutable 
                        {
                            return s.substr(0, idx++);
                        });    

        return list;
    } 
};


class Reduce
{
    size_t ln;
    bool   is_eq;
    std::string s_prv;

public:
    Reduce():ln(1), is_eq(false), s_prv(" ") {}
    
    auto operator()(std::string s)
    {
        contaner_t list_out;

        if(s_prv == s)
            is_eq = true;
        else {
            auto prefix = s.compare(0, s_prv.length(), s_prv) != 0;
            if(s.length() > ln && (is_eq || (!is_eq && prefix)))
                ln = s.length();
            is_eq = false;
            s_prv = s;
        }
               
        list_out.push_back(std::to_string(ln));
        return list_out;
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