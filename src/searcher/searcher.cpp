#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <reader/word_usings.h>
#include <reader/conf_reader.h>
#include <chrono>

#include "TF_IDF.h"
#include "reader/reader.h"

using vectorString = std::vector<std::string>;

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
vectorString read_file (std::ifstream &file)
{
    vectorString file_strings;
    std::string temp;
    while (getline(file, temp))
        file_strings.push_back(temp);
    return file_strings;
}

int main(int argc, char* argv[])
{
    // set name of configuration file
    std::string config_file("../config.dat");
//    if (argc >= 2) { config_file = argv[1]; }

    // try to open that file
    std::ifstream cf(config_file);
    if (!cf.is_open()) {
        std::cerr << "Error while opening configuration file.\n"
                     "Check filename or path to file: "
                  << config_file
                  << std::endl;
        return OPEN_FILE_ERROR;
    }

    // try to read configurations and save them to map conf
    std::map<std::string, std::string> conf;
    try {
        conf = read_conf(cf, '=');
        cf.close();
    }
    catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return READ_FILE_ERROR;
    }

//    boost::filesystem::path data_dir(conf["data_folder"]);
//    std::vector<std::pair<std::string, vectorString>> files;
    auto counting_results = read_and_count(conf["data_folder"]);
    for (auto& p: counting_results.total)
        std::cout << p.first << " : " << p.second << std::endl;

    for (auto& f: counting_results.per_file) {
        std::cout << "\nFILENAME:" << f.first << std::endl;
        for (auto& p: f.second)
            std::cout << p.first << " : " << p.second << std::endl;
    }

//    for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(data_dir), {})) {
//        std::ifstream file_to_read(entry.path().string());
//        files.emplace_back(std::make_pair(entry.path().string(), read_file(file_to_read)));
//        file_to_read.close();
//    }

//    TF_IDF tfidf{files};
//    tfidf.compute();
//    for (auto &p: tfidf.files_frequencies) {
//        std::cout << "FILENAME: " << p.first << std::endl;
//        for (size_t i = 0; i < tfidf.unique_words_list.size(); ++i) {
//            std::cout << tfidf.unique_words_list[i] << " : " << p.second[i] << std::endl;
//        }
//    }
//    auto end_time = get_current_time_fenced();
//    std::cout << "Total time: " << to_us(end_time - start_time) << std::endl;
    return 0;
}