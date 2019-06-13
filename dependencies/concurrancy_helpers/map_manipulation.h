#ifndef LAB4_MAP_MANIPULATION_H
#define LAB4_MAP_MANIPULATION_H

#include <string>
#include <atomic>

#include "../reader/word_usings.h"
#include "thread_safe_queue.h"


void merge_two_maps (thread_safe_queue<WordMap> &maps_queue);


#endif //LAB4_MAP_MANIPULATION_H
