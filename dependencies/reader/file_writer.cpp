#include <iostream>
#include <fstream>

#include "file_writer.h"
#include "word_usings.h"

void write_file(const std::string &filename, const std::vector<Pair> &words) {
    std::ofstream outfile(filename);
    for (auto &word : words) {
        outfile << word.first;
        outfile.width(20 - word.first.length());
        outfile << " :    " << word.second << std::endl;
    }
    outfile.close();
}


void write_results(const std::string &outfile, std::vector<Pair> &sorted_numbers) {
    try {
        write_file(outfile, sorted_numbers);
    } catch (std::exception &ex) {
        std::cerr << "Error while saving results to "
                  << outfile
                  << ". Try to complete work..."
                  << std::endl;
        throw (std::invalid_argument("Error while saving results"));
    }

}