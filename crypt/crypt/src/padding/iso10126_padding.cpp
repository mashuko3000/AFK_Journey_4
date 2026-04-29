#include "../../inc/padding/iso10126_padding.hpp"
#include <stdexcept>

namespace crypto {
    bytes_t ISO10126::add(const bytes_t &data, size_t block_size) const {
        if (block_size <= 0 || block_size > 256) throw std::invalid_argument("[ISO10126][ADD] invalid block size");
        if (data.empty()) throw std::invalid_argument("[ISO10126][ADD] empty data");

        size_t data_size = data.size();
        size_t to_add = block_size - (data_size % block_size);
        if (to_add == 0) to_add = block_size;

        bytes_t result = data;

        std::uniform_int_distribution<int> dis(0, 255);

        for (size_t i = 0; i < to_add - 1; ++i) {
            result.push_back(static_cast<uint8_t>(dis(gen)));
        }

        result.push_back(static_cast<uint8_t>(to_add));
        return result;
    }

    bytes_t ISO10126::remove(const bytes_t &data, size_t block_size) const {
        if (block_size <= 0 || block_size > 256) throw std::invalid_argument("[ISO10126][REMOVE] invalid block size");
        if (data.empty()) throw std::invalid_argument("[ISO10126][REMOVE] empty data");

        size_t data_size = data.size();
        if (data_size % block_size != 0)
            throw std::invalid_argument("[ISO10126][REMOVE] data size not multiple of block size");

        uint8_t was_added = data.back();
        if (was_added == 0 || was_added > block_size)
            throw std::runtime_error("[ISO10126][REMOVE] invalid padding length byte");

        size_t original_size = data_size - was_added;
        return bytes_t(data.begin(), data.begin() + original_size);
    }
}