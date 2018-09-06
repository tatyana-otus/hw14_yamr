#include <vector>
#include <array>
#include <list>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <mutex>

using contaner_t = std::vector<std::string>;

template<class M, class R>
class map_reduce
{
public:
    map_reduce(std::string f_name_, 
               size_t mapper_cnt_, 
               size_t reducer_cnt_)
    :f_name(f_name_),
    mapper_cnt(mapper_cnt_),
    reducer_cnt(reducer_cnt_)
    {
        split();
    }


    void run()
    {
        //------------------------------
        //map
        map_result.resize(mapper_cnt);

        run_threads(mapper_cnt, &map_reduce::mapper_run);

        //-------------------
        //shuffle
        reduce_in.resize(reducer_cnt);
        reducer_mx.resize(reducer_cnt);

        for(auto & p: reducer_mx){
            p = std::make_unique<std::mutex>();
        }

        run_threads(mapper_cnt, &map_reduce::shuffle_run);

        //-------------------
        //reduce
        run_threads(reducer_cnt, &map_reduce::reducer_run);
    }


private:

    void split()
    {
        std::ifstream ifs(f_name);
        if (!ifs.is_open()) {
            throw std::invalid_argument("File could not be opened");
        }

        ifs.seekg(0, std::ios::end);
        const auto size = ifs.tellg();

        const int chunk_size = size / mapper_cnt;
        if(chunk_size == 0){
            throw std::invalid_argument("Too much <mnum>");
        }

        char ch;
        ifs.seekg(0, std::ios::beg);

        chunk_offset.resize(mapper_cnt);
        chunk_offset[0][0] = 0;
        for(size_t i = 1; i < mapper_cnt; ++i) {
            ifs.seekg(chunk_size * i, std::ios::beg);       
            do{
                ifs.read(&ch, 1);
                if (ifs.fail()){ 
                    throw std::invalid_argument("File could not be readed.");
                }    
            }while(ch != '\n');
            chunk_offset[i - 1][1] = ifs.tellg();
            chunk_offset[i][0] = ifs.tellg();
            if(chunk_offset[i - 1][1] <= chunk_offset[i - 1][0]){
                throw std::invalid_argument("Too much <mnum>");
            }
        }
        ifs.seekg(0, std::ios::end);
        chunk_offset[mapper_cnt - 1][1] = ifs.tellg();
        ifs.close();    
    }


    void reducer_run(size_t th_idx)
    {
        try
        {
            R func;

            contaner_t res;
            for(const auto & s : reduce_in[th_idx]){
                res = func(s);
            }
           
            std::string name = "red_out_" + std::to_string(th_idx);
            std::ofstream ofs(name);

            for(const auto& s : res) {
                ofs << s << std::endl;
            }
                
        }
        catch(const std::exception &e){
            std::cerr << e.what() << std::endl;
        }    
    }


    void shuffle_run(size_t th_idx)
    {
        try
        {
            for(auto &s : map_result[th_idx]){
                if(!s.empty()){
                    auto idx = s[0] % reducer_cnt;
                    std::lock_guard<std::mutex> lock(*reducer_mx[idx]);

                    reduce_in[idx].insert(std::lower_bound(reduce_in[idx].begin(),
                                                           reduce_in[idx].end(), s
                                                           ),
                                          std::move(s)
                                          );
                }
            }
        }
        catch(const std::exception &e){
            std::cerr << e.what() << std::endl;
        }    
    }


    void mapper_run(size_t th_idx)
    {
        try
        {
            M func;

            std::ifstream ifs(f_name);
            if (!ifs.is_open()) {
                throw std::invalid_argument("File could not be opened");
            }

            int size = chunk_offset[th_idx][1] - chunk_offset[th_idx][0];
            ifs.seekg(chunk_offset[th_idx][0], std::ios::beg);
            std::string str(size, ' ');
            ifs.read(&str[0], size);
            ifs.close();


            std::istringstream iss(str);

            std::string line;
            while (std::getline(iss, line))
            {
                if(!line.empty()){
                    auto res = func(line);
                    map_result[th_idx].insert(
                                  map_result[th_idx].end(),
                                  std::make_move_iterator(res.begin()),
                                  std::make_move_iterator(res.end())
                                );
                }    
            }
            std::sort(map_result[th_idx].begin(), map_result[th_idx].end());
        }
        catch(const std::exception &e){
            std::cerr << e.what() << std::endl;
        }    
    }


    template <class F>
    void run_threads(int th_cnt, F fun) 
    {
        std::vector<std::thread> th;
        th.resize(th_cnt);

        auto i = 0;
        for(auto & t : th){
            t = std::thread(fun , this, i);
            ++i;
        }
        for(auto & t : th){
            t.join();
        }
    }

    std::string f_name;

    size_t mapper_cnt;
    size_t reducer_cnt; 

    std::vector<std::array<std::size_t, 2>> chunk_offset;

    std::vector<contaner_t> map_result;
    std::vector<contaner_t> reduce_in;

    std::vector<std::unique_ptr<std::mutex>> reducer_mx;
};