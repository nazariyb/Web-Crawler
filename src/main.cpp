#include <cstdint>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <regex>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range_core.hpp>
#include "../dependencies/TF_IDF.h"
#include "domain_parser.h"

vectorString read_file (std::ifstream& file)
{
    vectorString file_strings;
    std::string temp;
    while (getline(file, temp))
    file_strings.push_back(temp);
    return file_strings;
}

int main ()
{
    //    int threads_number = 8;
    //    parse_links(std::string{"https://awoiaf.westeros.org/index.php/List_of_characters"}, static_cast<PARSE_TYPE>(0));
    //    std::ifstream links_file("../links_to_parse.txt");
    //    std::vector<std::string> links;
    //    std::string temp;
    //
    //    while (getline(links_file, temp)) {
    //        links.push_back(temp);
    //    }
    //    links_file.close();
    //
    //    std::vector<std::thread> threads;
    //    std::cout << links[0] << std::endl;
    //    int thread_step = links.size() / threads_number;
    //    int start = 0;

    //    for (int i = 0; i < threads_number - 1; ++i) {
    //        threads.emplace_back(parse_bio, links, start, thread_step);
    //        start += threads_number + 1;
    //    }
    //    threads.emplace_back(parse_bio, links, start, links.size() - start);
    //
    //    for (auto &t: threads) {
    //        t.join();
    //    }

    boost::filesystem::path data_dir("../test_data");
    std::vector<std::pair<std::string, vectorString>> files;
    for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(data_dir), {})) {
        std::ifstream file_to_read(entry.path().string());
        files.emplace_back(std::make_pair(entry.path().string(), read_file(file_to_read)));
        file_to_read.close();
    }

    TF_IDF tfidf{files};
    tfidf.compute();
    for (auto& p: tfidf.files_frequencies) {
        std::cout << "FILENAME: " << p.first << std::endl;
        for (size_t i = 0; i < tfidf.unique_words_list.size(); ++i) {
            std::cout << tfidf.unique_words_list[i] << " : " << p.second[i] << std::endl;
        }
    }
}
