#include <cstdint>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <regex>
#include <thread>
#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range_core.hpp>
#include "../../dependencies/TF_IDF.h"
#include "domain_parser.h"

inline std::chrono::steady_clock::time_point get_current_time_fenced ()
{
    static_assert(std::chrono::steady_clock::is_steady, "Timer should be steady (monotonic).");
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::steady_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us (const D &d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

int main ()
{
    auto start_time = get_current_time_fenced();
    int threads_number = std::thread::hardware_concurrency();
    parse_links(std::string{"https://awoiaf.westeros.org/index.php/List_of_characters"}, PARSE_TYPE::LINKS);
    std::ifstream links_file("../links_to_parse.txt");
    std::vector<std::string> links;
    std::string temp;
    while (getline(links_file, temp)) {
        links.push_back(temp);
    }
    links_file.close();

    std::vector<std::thread> threads;
    //        std::cout << links[0] << std::endl;
    int thread_step = links.size() / threads_number;
    int start = 0;

    for (int i = 0; i < threads_number - 1; ++i) {
        threads.emplace_back(parse_bio, links, start, thread_step);
        start += threads_number + 1;
    }
    threads.emplace_back(parse_bio, links, start, links.size() - start);

    for (auto &t: threads) {
        t.join();
    }

}
