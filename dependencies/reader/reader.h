#ifndef WEBCRAWLER_READER_H
#define WEBCRAWLER_READER_H

#include "word_usings.h"

struct CountedWords {
    WordMap total;
    std::vector<std::pair<std::string, WordMap>> per_file;
    };

CountedWords read_and_count (const std::string& directory_name);

#endif //WEBCRAWLER_READER_H
