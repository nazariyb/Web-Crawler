#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <map>
#include <atomic>
#include <algorithm>
#include <archive.h>
#include <archive_entry.h>
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <cmath>
#include <mutex>
#include <string>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <qt5/QtCore/QThreadPool>
#include <TF_IDF.h>

#include "reader.h"
#include "word_usings.h"
#include "conf_reader.h"
#include "file_reader.h"
#include "../concurrancy_helpers/thread_safe_queue.h"
#include "../concurrancy_helpers/Task.h"
#include "../concurrancy_helpers/map_manipulation.h"
#include "../concurrancy_helpers/function_wrapper.h"

StringVector find_files_to_index (const std::string &directory_name)
{
    WordMap wMap;
    // iterate through text
    StringVector txt;
    for (boost::filesystem::recursive_directory_iterator end, dir(directory_name);
         dir != end; ++dir) {
        std::string pathname{(*dir).path().string()};

        if (Reader::is_txt(pathname)) {
            txt.push_back(pathname);
        }

    }

    return txt;
}

void index_text (const std::string &filepath,
                 thread_safe_queue<std::stringstream> &stream_queue,
                 thread_safe_queue<WordMap> &maps_queue,
                 thread_safe_queue<std::pair<std::string, WordMap>> &maps_queue_not_to_merge,
                 std::atomic_int &threads_finished,
                 std::atomic_int &threads_to_be_finished,
                 QThreadPool &pool)
{
    std::string temp, text;
    using namespace boost::locale::boundary;

    while (true) {

        std::stringstream file_stream;
        stream_queue.wait_and_pop(file_stream);
        auto wMap = new WordMap;

        // iterate through text
        while (getline(file_stream, temp)) {
            // normalize encoding
            text = boost::locale::normalize(temp);

////            if word is actually a number, skip it
//            try {
//                boost::lexical_cast<double>(text);
//                continue;
//            }
//            catch (boost::bad_lexical_cast &) {}

            // bound text by words
            ssegment_index map(word, text.begin(), text.end());
            map.rule(word_any);

            // convert them to fold case and add to the vector
            for (ssegment_index::iterator it = map.begin(), e = map.end(); it != e; ++it) {
                ++(*wMap)[boost::locale::fold_case(it->str())];
            }
        }
        if (wMap->empty() && stream_queue.empty()) {
            stream_queue.push(std::move(file_stream));
            ++threads_finished;
            if (threads_finished == threads_to_be_finished) {
                maps_queue.push(*wMap);
                maps_queue_not_to_merge.push(std::pair<std::string, WordMap>{filepath, *wMap});
            }
            return;
        }
        if (!wMap->empty()) {
            maps_queue.push(*wMap);
            maps_queue_not_to_merge.push(std::pair<std::string, WordMap>{filepath, *wMap});
            auto f = new function_wrapper([&maps_queue] () { merge_two_maps(maps_queue); });
            auto t = new Task(*f);
            pool.start(t);
        }
    }
}

CountedWords read_and_count (const std::string &directory_name)
{
    // Create system default locale
    boost::locale::generator gen;
    std::locale loc = gen("");
    std::locale::global(loc);
    std::wcout.imbue(loc);
    std::ios_base::sync_with_stdio(false);

    thread_safe_queue<std::stringstream> stream_queue{};
    thread_safe_queue<WordMap> maps_queue{};
    thread_safe_queue<std::pair<std::string, WordMap>> maps_queue_not_to_merge{};

    std::stringstream ss;
    QThreadPool pool{};

    unsigned short max_threads = std::thread::hardware_concurrency();
    pool.setMaxThreadCount(max_threads);
    std::atomic_int threads_finished{0}, threads_to_be_finished{max_threads};

    auto files_to_index = find_files_to_index(directory_name);
    std::cout << "Start processing data..." << std::endl;

    for (auto &filepath: files_to_index) {
        try {
            Reader::read_txt(filepath, ss);
            stream_queue.push(std::move(ss));
            auto f = new function_wrapper([&filepath,
                                           &stream_queue,
                                           &maps_queue,
                                           &maps_queue_not_to_merge,
                                           &threads_finished,
                                           &threads_to_be_finished,
                                           &pool]
                                           () {
                                            index_text(filepath,
                                                       stream_queue,
                                                       maps_queue,
                                                       maps_queue_not_to_merge,
                                                       threads_finished,
                                                       threads_to_be_finished,
                                                       pool);
                                            });
            auto t = new Task(*f);
            pool.start(t, 1);
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            continue;
        }
    }
    std::cout << "Data read" << std::endl;
    stream_queue.push(std::move(std::stringstream{}));

    pool.waitForDone();
    auto wordsMap = *maps_queue.try_pop();

    std::vector<std::pair<std::string, WordMap>> maps_to_return;
    auto start_it = maps_to_return.begin();
    std::cout << "here" << std::endl;

    while (!maps_queue_not_to_merge.empty())
    maps_to_return.push_back(*maps_queue_not_to_merge.try_pop());

    std::cout << "and here" << std::endl;
    return {wordsMap, maps_to_return};
}
