#include"feistel.hpp"

feistel_network::feistel_network(
        i_key_expander& expander,
        i_feistel_transform& transform,
        int rounds)
        : _expander(expander),
            _transform(transform),
            _rounds(rounds)
{
    if(rounds <= 0) throw std::invalid_argument("Number of rounds must be positive");
    if(rounds > 256) throw std::invalid_argument("Too many rounds (> 256) — possible performance or security issue");
}

// i_cipher methods
void feistel_network::setup_keys(const bytes_t & key)
{
    auto expanded = _expander.expand(key);
    if(expanded.size() != static_cast<size_t>(_rounds))
    {
        throw std::runtime_error("Invalid key schedule: expected " + std::to_string(_rounds)
        + " round keys, got " + std::to_string(expanded.size())
        );
    }
    _enc_sub_keys = expanded;
    _dec_sub_keys = expanded;
    std::reverse(_dec_sub_keys.begin(), _dec_sub_keys.end());
}

bytes_t feistel_network::encrypt_block(const bytes_t & block)
{
    if(_enc_sub_keys.empty()) throw std::logic_error("Call setup_keys() before encryption");
    if(block.size() != BLOCK_SIZE)
    {
        throw std::invalid_argument("Invalid block size");
    }
    return encrypt_rounds(block);
}
bytes_t feistel_network::decrypt_block(const bytes_t & block)
{
    if(_dec_sub_keys.empty()) throw std::logic_error("Call setup_keys() before decryption");
    if(block.size() != BLOCK_SIZE)
    {
        throw std::invalid_argument("Invalid block size");
    }
    return decrypt_rounds(block);
}

bytes_t feistel_network::xor_bytes(const bytes_t& a, const bytes_t& b)
{
    const std::size_t a_size = a.size();
    const std::size_t b_size = b.size();

    if(a_size != b_size)
    {
        throw std::invalid_argument("xor_bytes: size mismatch");
    }

    bytes_t result(a_size);
    for(std::size_t i = 0; i < a_size; ++i)
    {
        result[i] = a[i] ^ b[i];
    }
    return result;
}

bytes_t feistel_network::encrypt_rounds(const bytes_t& block) const
{
    std::size_t b_size = block.size();
    if((b_size & 1) || block.empty())
    {
        throw std::invalid_argument("encrypt_rounds: block size must be even and positive");
    }

    size_t half = b_size >> 1; // divison on 2

    bytes_t left(block.begin(), block.begin() + half);
    bytes_t right(block.begin() + half, block.end());

    for (int i = 0; i < _rounds; ++i)
    {
        bytes_t f = _transform.transform(right, _enc_sub_keys[static_cast<std::size_t>(i)]);
        bytes_t new_right = xor_bytes(left, f);

        left = std::move(right);
        right = std::move(new_right);
    }

    bytes_t result;
    result.reserve(b_size);
    result.insert(result.end(), right.begin(), right.end());
    result.insert(result.end(), left.begin(), left.end());

    return result;
}
bytes_t feistel_network::decrypt_rounds(const bytes_t& block) const
{
    std::size_t b_size = block.size();
    if((b_size & 1) || block.empty())
    {
        throw std::invalid_argument("decrypt_rounds: block size must be even and positive");
    }

    size_t half = b_size >> 1; // divison on 2

    bytes_t left(block.begin(), block.begin() + half);
    bytes_t right(block.begin() + half, block.end());

    for (int i = 0; i < _rounds; ++i)
    {
        bytes_t f = _transform.transform(right, _dec_sub_keys[static_cast<std::size_t>(i)]);
        bytes_t new_right = xor_bytes(left, f);
        left = std::move(right);
        right = std::move(new_right);
    }

    bytes_t result;
    result.reserve(b_size);
    result.insert(result.end(), right.begin(), right.end());
    result.insert(result.end(), left.begin(), left.end());

    return result;
}
