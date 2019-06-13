#include <fstream>
#include <iostream>
#include "file_reader.h"
#include <algorithm>
#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "word_usings.h"


bool Reader::is_archive (const std::string &filename)
{
    StringVector archive_exts{".zip", ".tar", ".gz", ".tar.gz", ".7z"};
    for (auto &ext: archive_exts) {
        if (boost::iequals(boost::filesystem::extension(filename), ext))
            return true;
    }
    return false;
}

bool Reader::is_txt (const std::string &filename)
{
    return boost::iequals(boost::filesystem::extension(filename), ".txt");
}

void Reader::read_txt (std::string &address, std::stringstream &ss)
{
    std::ifstream f(address);
    ss << f.rdbuf();
    f.close();
}

void Reader::read_txt (std::string &address, std::string &s)
{
     std::ifstream f(address);
     std::stringstream ss;
     ss << f.rdbuf();
     s = std::move(ss.str());
     f.close();
}

