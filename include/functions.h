#include <iostream>
#include <algorithm>

#include "map_reduce.h"


auto map(std::string s)
{
    contaner_t list;   

    std::generate_n(std::back_inserter(list), s.size(),
                    [&s, idx = 1] () mutable 
                    {
                        return s.substr(0, idx++);
                    });    

    return list;
}


auto reduce(contaner_t list_in)
{
    contaner_t list_out;

    auto it = list_in.begin();

    size_t ln = 0;
    while(it != std::end(list_in)){
        auto it_next = std::find_if(it, std::end(list_in), [&it](const auto & s)
                                                           {
                                                                return s != *it;
                                                            });
        if(std::distance(it, it_next) == 1){
            it_next = std::find_if(std::next(it), 
                                    std::end(list_in), 
                                    [&it](const auto & s)
                                    {
                                        return (s.compare(0, it->length(), *it) != 0); 
                                    });
            if(it->length() > ln){
                ln = it->length();
            }
        }
        it = it_next;
    } 

    list_out.push_back(std::to_string(ln));
    return list_out;
}


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
    
    map_reduce framework(map, reduce, file_arg, mnum, rnum);
    framework.run();
}