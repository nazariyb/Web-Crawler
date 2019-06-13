#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

#include "../reader/word_usings.h"
#include "thread_safe_queue.h"
#include "map_manipulation.h"

void merge_two_maps(thread_safe_queue<WordMap> &maps_queue) {
    std::pair<WordMap, WordMap> map_pair;
    while (true) {
        map_pair = maps_queue.double_pop();
        if (map_pair.first.empty() || map_pair.second.empty()) {
            if (maps_queue.empty()) {
                (map_pair.second.empty()) ? maps_queue.double_push(map_pair.first, map_pair.second)
                                          : maps_queue.double_push(map_pair.second, map_pair.first);
                return;
            } else {
                (map_pair.second.empty()) ? maps_queue.double_push(map_pair.first, map_pair.second)
                                          : maps_queue.double_push(map_pair.second, map_pair.first);
                continue;
            }
        }

        for (auto &word: map_pair.second)
            map_pair.first[word.first] += word.second;

        maps_queue.push(map_pair.first);
    }
}
