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

    k_shift %= n_bits;
    const size_t n_bytes = bytes_for_bits(n_bits);

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

        if (get_bit(input, src_idx)) {
            set_bit(result, i, true);
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

    const size_t n_bytes = bytes_for_bits(n_bits);
    std::vector<uint8_t> result(input.begin(), input.begin() + n_bytes);

    size_t bytes_to_process = std::min(n_bytes, mask.size());

    for (size_t i = 0; i < bytes_to_process; ++i)
    {
        switch (op)
        {
            case mask_operation::And: result[i] &= mask[i]; break;
            case mask_operation::Or:  result[i] |= mask[i]; break;
            case mask_operation::Xor: result[i] ^= mask[i]; break;
        }
    }

    if (op == mask_operation::And)
    {
        for (size_t i = bytes_to_process; i < n_bytes; ++i)
        {
            result[i] = 0;
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
        if (get_bit(input, i + bit_pos))
        {
            set_bit(result, bit_pos, true);
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

    if (i >= n_bits || j >= n_bits)
    {
        throw std::out_of_range("Bit index is out of n_bits range");
    }

    if (i == j) return input;

    std::vector<uint8_t> result = input;

    bool bit_i = get_bit(result, i);
    bool bit_j = get_bit(result, j);

    if (bit_i != bit_j)
    {
        set_bit(result, i, bit_j);
        set_bit(result, j, bit_i);
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
    set_bit(result, i, state);
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

inline bool get_bit(const std::vector<uint8_t>& data, size_t bit_idx)
{
    size_t byte_idx = bit_idx >> 3;
    uint8_t bit_in_byte = 7 - (bit_idx & 7);
    return (data[byte_idx] & (uint8_t(1) << bit_in_byte)) != 0;
}

inline void set_bit(std::vector<uint8_t>& data, size_t bit_idx, bool value)
{
    size_t byte_idx = bit_idx >> 3;
    uint8_t bit_in_byte = 7 - (bit_idx & 7);
    uint8_t mask = uint8_t(1) << bit_in_byte;
    if (value)
    {
        data[byte_idx] |= mask;
    }
    else
    {
        data[byte_idx] &= ~mask;
    }
}

inline const size_t bytes_for_bits(size_t n_bits)
{
    return (n_bits + 7) >> 3;
}
#pragma endregion