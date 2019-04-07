#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <regex>

namespace
{
    std::size_t callback(
            const char* in,
            std::size_t size,
            std::size_t num,
            std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}
std::vector<std::string> extract_hyperlinks( std::string text )
{
    static const std::regex hl_regex( "<a href=\"(.*?)\"", std::regex_constants::icase  ) ;

    return { std::sregex_token_iterator( text.begin(), text.end(), hl_regex, 1 ),
             std::sregex_token_iterator{} } ;
}
int main()
{
    const std::string url("https://www.pravda.com/");

    CURL* curl = curl_easy_init();

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

    if (httpCode == 200) {
        std::cout << "\nGot successful response from " << url << std::endl;
        std::ofstream outfile;
        outfile.open("/home/yura/CLionProjects/parser/links_to_parse.txt",std::ios_base::app);
        std::vector<std::string> links = extract_hyperlinks(*httpData);
        std::string final_url;
        for (const auto &link : links) {
            final_url = "";
            if (link[0] == '/')
                final_url = url + link.substr(1,link.size());
            else
                final_url = link;
            outfile<< final_url<<std::endl;
        }
    }
    return 0;
}
