#ifndef WEBCRAWLER_DOMAIN_PARSER_H
#define WEBCRAWLER_DOMAIN_PARSER_H

#include <iostream>
#include <vector>

namespace {
    std::size_t callback (
            const char *in,
            std::size_t size,
            std::size_t num,
            std::string *out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}

enum PARSE_TYPE
    {
    LINKS, PARAGRAPHS
    };

std::vector<std::string> extract_hyperlinks (std::string text);
std::vector<std::string> extract_paragraphs (std::string text);
void parse_links (std::string url, PARSE_TYPE parseType);
void parse_bio (std::vector<std::string> links, int start_index, int step);

#endif //WEBCRAWLER_DOMAIN_PARSER_H
