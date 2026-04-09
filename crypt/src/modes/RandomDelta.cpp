#include "../../inc/modes/RandomDelta.hpp"
#include <algorithm>
#include <random>

namespace crypto {

    RandomDelta::RandomDelta(const bytes_t &delta) : delta_(delta) {}

    RandomDelta::RandomDelta(size_t block_size)
    {
        delta_.resize(block_size);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        for (auto& byte : delta_)
        {
            byte = static_cast<unsigned char>(dis(gen));
        }
    }

    void RandomDelta::next_counter(bytes_t &counter) const {
        int carry = 0;
        for (int i = static_cast<int>(counter.size()) - 1; i >= 0; --i) {
            int delta_val = (i < static_cast<int>(delta_.size())) ? delta_[i] : 0;

            int sum = counter[i] + delta_val + carry;
            counter[i] = static_cast<unsigned char>(sum & 0xFF);
            carry = sum >> 8;
        }
    }

    bytes_t RandomDelta::get_counter_for_block(const bytes_t &iv, uint64_t block_idx) const {
        bytes_t res = iv;
        for (uint64_t i = 0; i < block_idx; ++i) {
            next_counter(res);
        }
        return res;
    }

}