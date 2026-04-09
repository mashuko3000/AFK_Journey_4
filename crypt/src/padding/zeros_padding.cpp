#include "../../inc/padding/zeros_padding.hpp"
#include <stdexcept>

namespace crypto {
    bytes_t ZerosPadding::add(const bytes_t &data, size_t block_size) const {
        if (data.empty()) throw std::invalid_argument("[ZerosPadding][ADD] data is empty");
        if (block_size == 0) throw std::invalid_argument("[ZerosPadding][ADD] block size must be > 0");

        bytes_t result = data;
        size_t rem = result.size() % block_size;

        if (rem != 0) {
            size_t to_add = block_size - rem;
            result.insert(result.end(), to_add, 0x00);
        }
        return result;
    }

    bytes_t ZerosPadding::remove(const bytes_t &data, size_t block_size) const {
        if (data.empty()) throw std::invalid_argument("[ZerosPadding][REMOVE] data is empty");

        bytes_t result = data;
        while (!result.empty() && result.back() == 0x00) {
            result.pop_back();
        }
        return result;
    }
}
