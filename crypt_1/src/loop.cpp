#include "../inc/loop.hpp"
#pragma region a
std::vector<uint8_t> rotate_left(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        size_t k_shift
)
{
    if (n_bits == 0) return {};

    validate_input(input, n_bits);

    const size_t n_bytes = (n_bits + 7) >> 3;
    k_shift %= n_bits;

    if(k_shift == 0)
    {
        std::vector<uint8_t> result(input.begin(), input.begin() + n_bytes);
        trim_unused_bits(result, n_bits);
        return result;
    }

    std::vector<uint8_t> result(n_bytes, 0);

    for(size_t i = 0; i < n_bits; ++i)
    {
        const size_t src_idx = (i + k_shift) % n_bits;

        const size_t src_byte = src_idx >> 3;
        const uint8_t src_offset = src_idx & 7; // % 8
        const uint8_t bit = (input[src_byte] >> (7 - src_offset)) & 1;

        if(bit)
        {
            size_t dest_byte = i >> 3;
            uint8_t dest_offset = i & 7;
            result[dest_byte] |= (1 << (7 - dest_offset));
        }
        /* //short_version
        uint8_t bit = (input[src_idx >> 3] >> (7 - (src_idx & 7))) & 1;
        if(bit)
        {
            result[i >> 3] |= (1 << (7 - (i & 7)));
        }
        */
    }
    trim_unused_bits(result, n_bits);
    return result;
}
#pragma endregion
#pragma region b

std::vector<uint8_t> rotate_right(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        size_t k_shift
)
{
    if (n_bits == 0) return {};
    return rotate_left(input, n_bits, n_bits - (k_shift % n_bits));
}

/*
std::vector<uint8_t> rotate_right(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        size_t k_shift
)
{
    if (n_bits == 0) return {};

    validate_input(input, n_bits);

    const size_t n_bytes = (n_bits + 7) >> 3;
    k_shift %= n_bits;

    if(k_shift == 0)
    {
        std::vector<uint8_t> result(input.begin(), input.begin() + n_bytes);
        trim_unused_bits(result, n_bits);
        return result;
    }

    std::vector<uint8_t> result(n_bytes, 0);

    for(size_t i = 0; i < n_bits; ++i)
    {
        const size_t src_idx = (i + n_bits - k_shift) % n_bits;
        const uint8_t bit = (input[src_idx >> 3] >> (7 - (src_idx & 7))) & 1;

        if (bit)
        {
            result[i >> 3] |= (1 << (7 - (i & 7)));
        }
    }

    trim_unused_bits(result, n_bits);
    return rotate_left(input, n_bits, n_bits - (k_shift * n_bits));
}
 */
#pragma endregion
#pragma region c

std::vector<uint8_t> apply_mask(
        const std::vector<uint8_t>& input,
        const std::vector<uint8_t>& mask,
        const size_t n_bits,
        const mask_operation op
)
{
    validate_input(input, n_bits);

    const size_t n_bytes = (n_bits + 7) >> 3;
    const size_t mask_bytes = mask.size();

    std::vector<uint8_t> result = input;
    if (result.size() > n_bytes)
    {
        result.resize(n_bytes);
    }

    const size_t common_bytes = (n_bytes < mask_bytes) ? n_bytes : mask_bytes;

    for (size_t i = 0; i < common_bytes; ++i)
    {
        const uint8_t m = mask[i];
        switch (op)
        {
            case mask_operation::And: result[i] &= m; break;
            case mask_operation::Or:  result[i] |= m; break;
            case mask_operation::Xor: result[i] ^= m; break;
        }
    }

    if (n_bytes > mask_bytes)
    {
        for (size_t i = mask_bytes; i < n_bytes; ++i)
        {
            if (op == mask_operation::And)
            {
                result[i] = 0;
            }
        }
    }

    trim_unused_bits(result, n_bits);
    return result;
}

#pragma endregion
#pragma region d
std::vector<uint8_t> bit_slice(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        const size_t i,
        const size_t j
)
{
    validate_input(input, n_bits);

    if(i > j)
    {
        throw std::invalid_argument("Start index i must be less than or equal to end index j");
    }

    if(j >= n_bits)
    {
        throw std::out_of_range("Index j is out of n_bits range");
    }

    const size_t range_len =  j - i + 1;
    const size_t res_bytes = (range_len + 7) >> 3;
    std::vector<uint8_t> result(res_bytes, 0);

    for (size_t bit_pos = 0; bit_pos < range_len; ++bit_pos)
    {
        const size_t src_idx = i + bit_pos;
        const uint8_t bit = ((input[src_idx >> 3] >> (7 - (src_idx & 7))) & 1);
        if (bit)
        {
            result[bit_pos >> 3] |= (1 << (7 - (bit_pos & 7)));
        }
    }

    trim_unused_bits(result, range_len);
    return result;
}
#pragma endregion
#pragma region e
std::vector<uint8_t> swap_bits(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        const size_t i,
        const size_t j
)
{
    validate_input(input, n_bits);

    if (i >= n_bits || j >= n_bits) {
        throw std::out_of_range("Bit index is out of n_bits range");
    }

    if (i == j) return input;

    std::vector<uint8_t> result = input;

    const uint8_t bit_i = (result[i >> 3] >> (7 - (i & 7))) & 1;
    const uint8_t bit_j = (result[j >> 3] >> (7 - (j & 7))) & 1;

    if (bit_i != bit_j)
    {
        result[i >> 3] ^= (1 << (7 - (i & 7)));
        result[j >> 3] ^= (1 << (7 - (j & 7)));
    }

    return result;
}
#pragma endregion
#pragma region e
std::vector<uint8_t> set_bit(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        const size_t i,
        const bool state
)
{
    validate_input(input, n_bits);

    if (i >= n_bits)
    {
        throw std::out_of_range("Bit index is out of n_bits range");
    }

    std::vector<uint8_t> result = input;

    const size_t  byte_pos = i >> 3;
    const uint8_t bit_offset = 7 - (i & 7);
    const uint8_t mask = static_cast<uint8_t>(1 << bit_offset);

    if (state)
    {
        result[byte_pos] |= mask;
    }
    else
    {
        result[byte_pos] &= ~mask;
    }
    return result;
}
#pragma endregion
#pragma region helpers
void validate_input(
        const std::vector<uint8_t>& input,
        const size_t n_bits
        )
{
    if(input.empty())
    {
        throw std::invalid_argument("Input vector is empty, but n_bits > 0");
    }

    const size_t n_bytes = (n_bits + 7) >> 3;
    if(n_bytes > input.size())
    {
        throw std::out_of_range("n_bits is greater than the actual bit capacity of the input vector");
    }

    size_t capacity_bits = input.size() * 8;
    if(n_bits > capacity_bits)
    {
        throw std::invalid_argument("n_bits exceeds input capacity");
    }
}

void trim_unused_bits(
        std::vector<uint8_t>& result,
        const size_t n_bits
        )
{
    const size_t last_bits = n_bits & 7; // %8
    if(last_bits != 0)
    {
        const uint8_t mask = static_cast<uint8_t>(((1 << last_bits) - 1) << (8 - last_bits));
        result.back() &= mask;
    }
}
#pragma endregion