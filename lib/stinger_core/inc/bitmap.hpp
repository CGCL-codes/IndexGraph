/*
Copyright (c) 2014-2015 Xiaowei Zhu, Tsinghua University
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef SRC_LIB_BITMAP_HPP_
#define SRC_LIB_BITMAP_HPP_

#include <cstdint>
#include <cstddef>

#define WORD_OFFSET(i) (i >> 6)  // long type contains 64 (2^6) bits
#define BIT_OFFSET(i) (i & 0x3f)

class Bitmap {
 public:
    size_t size;
    uint64_t *data;
    Bitmap() {
        size = 0;
        data = NULL;
    }
    explicit Bitmap(size_t size) {
        init(size);
    }
    void init(size_t size) {
        this->size = size;
        data = new uint64_t[WORD_OFFSET(size) + 1];
    }
    void clear() {
        size_t bm_size = WORD_OFFSET(size);
#pragma omp parallel for
        for (size_t i = 0; i <= bm_size; i++) {
            data[i] = 0;
        }
#pragma omp barrier
    }
    void fill() {
        size_t bm_size = WORD_OFFSET(size);
#pragma omp parallel for
        for (size_t i = 0; i < bm_size; i++) {
            data[i] = 0xffffffffffffffff;
        }
#pragma omp barrier
        data[bm_size] = 0;
        for (size_t i = (bm_size << 6); i < size; i++) {
            data[bm_size] |= 1ul << BIT_OFFSET(i);
        }
    }
    inline uint64_t get_bit(size_t i) {
        return data[WORD_OFFSET(i)] & (1ul << BIT_OFFSET(i));
    }
    // void clear_bit(size_t i) {
    //     __sync_fetch_and_and(data + WORD_OFFSET(i), 0ul << BIT_OFFSET(i));
    // }
    void clear_bit(size_t i) {
        data[WORD_OFFSET(i)] &= (~(1ul << BIT_OFFSET(i)));
        //__sync_fetch_and_and(data + WORD_OFFSET(i), ~(1ul << BIT_OFFSET(i)));
    }
    void set_bit(size_t i) {
        __sync_fetch_and_or(data + WORD_OFFSET(i), 1ul << BIT_OFFSET(i));
    }
};

#endif  // SRC_LIB_BITMAP_HPP_