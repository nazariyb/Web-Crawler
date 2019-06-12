#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range_core.hpp>

#include "TF_IDF.h"

using vectorString = std::vector<std::string>;

vectorString read_file (std::ifstream &file)
{
    vectorString file_strings;
    std::string temp;
    while (getline(file, temp))
        file_strings.push_back(temp);
    return file_strings;
}

int main()
{
    boost::filesystem::path data_dir("../test_data");
    std::vector<std::pair<std::string, vectorString>> files;
    for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(data_dir), {})) {
        std::ifstream file_to_read(entry.path().string());
        files.emplace_back(std::make_pair(entry.path().string(), read_file(file_to_read)));
        file_to_read.close();
    }

    TF_IDF tfidf{files};
    tfidf.compute();
    for (auto &p: tfidf.files_frequencies) {
        std::cout << "FILENAME: " << p.first << std::endl;
        for (size_t i = 0; i < tfidf.unique_words_list.size(); ++i) {
            std::cout << tfidf.unique_words_list[i] << " : " << p.second[i] << std::endl;
        }
    }
//    auto end_time = get_current_time_fenced();
//    std::cout << "Total time: " << to_us(end_time - start_time) << std::endl;
    return 0;
}