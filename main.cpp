#include "functions.h"

int main(int argc, char** argv)
{
    try {    
        if (argc !=  4){
            std::cerr << "Usage: yamr <src> <mnum> <rnum>" << std::endl;
            return 1;
        }
        
        process(argv[1], argv[2], argv[3]);
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
