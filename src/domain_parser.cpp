#include <regex>
#include <iostream>
#include <curl/curl.h>
#include <fstream>


#include "domain_parser.h"

std::vector<std::string> extract_hyperlinks (std::string text)
{
    static const std::regex hl_regex("<a href=\"(.*?)\"", std::regex_constants::icase);
    //                                   "<a href="(.*?)""
    return {std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
            std::sregex_token_iterator{}};
}

std::vector<std::string> extract_paragraphs (std::string text)
{
    //    static const std::regex hl_regex("<p class=\"css-1ygdjhk evys1bk0\">(.*?)</p>", std::regex_constants::icase);
    static const std::regex hl_regex("<p>(.*?)\n</p>", std::regex_constants::icase);

    return {std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
            std::sregex_token_iterator{}};
}

void parse_links (std::string url, PARSE_TYPE parseType)
{
    //    const std::string url("https://www.nytimes.com/");
    const std::string domain("https://awoiaf.westeros.org/");

    CURL *curl = curl_easy_init();

    // Set remote URL.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Response information.
    int httpCode(0);
    auto *httpData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData);

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);
    printf("response: %d\n", httpCode);
    if (httpCode == 200) {
        std::cout << "\nGot successful response from " << url << std::endl;
        std::ofstream outfile;
        if (parseType == PARSE_TYPE::LINKS) {
            outfile.open("../links_to_parse.txt", std::ios_base::app);
            std::vector<std::string> links = extract_hyperlinks(*httpData);
            for (const auto &link : links)
                if (link.find("/index.php/") != std::string::npos && link.find(':') == std::string::npos)
                    outfile << domain + link.substr(1, link.size()) << std::endl;
        } else {
            std::string final_str;
            std::string name = url.substr(url.find_last_of('/') + 1);
            std::string file_name;
            file_name.append("../data/");
            file_name.append(name);
            file_name.append(".txt");
            outfile.open(file_name, std::ios_base::app);
            std::vector<std::string> links = extract_paragraphs(*httpData);
            for (const auto &link : links) {
                final_str = "";
                std::regex tags(R"(<("[^"]*?"|'[^']*?'|[^'">])*>)");
                final_str = std::regex_replace(link, tags, "");
                outfile << final_str << std::endl;
            }
        }
        outfile.close();
    }
}

void parse_bio (std::vector<std::string> links, int start_index, int step)
{
    for (int i = start_index; i < start_index + step; ++i) {
        parse_links(links[i], PARSE_TYPE::PARAGRAPHS);
    }
}
