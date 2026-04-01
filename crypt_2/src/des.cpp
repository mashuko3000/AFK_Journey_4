#include"des.hpp"
#include"p_box.hpp"

bytes_t des_cipher::pre_encrypt(const bytes_t& block) const
{
    if (block.size() != 8)
    {
        throw std::invalid_argument("DES block must be 8 bytes");
    }
    return p_box(block,
                 des_tables_storage::IP,
                 bit_order::big_endian,
                 bit_index::one);
}
bytes_t des_cipher::post_encrypt(const bytes_t& block) const
{
    return p_box(block,
                 des_tables_storage::FP,
                 bit_order::big_endian,
                 bit_index::one);
}

bytes_t des_cipher::pre_decrypt(const bytes_t& block) const
{
    return pre_encrypt(block);
}
bytes_t des_cipher::post_decrypt(const bytes_t& block) const
{
    return post_encrypt(block);
}

round_keys_t des_cipher::des_key_expander::expand(const bytes_t& master_key) const
{
    if (master_key.size() != 8)
    {
        throw std::invalid_argument("DES master key must be 8 bytes (64 bits)");
    }

    round_keys_t round_keys;
    round_keys.reserve(16);

    bytes_t pc1_key = p_box(master_key,
                            des_tables_storage::PC1,
                            bit_order::big_endian,
                            bit_index::one);

    uint32_t c = ((uint32_t)pc1_key[0] << 20) |
                 ((uint32_t)pc1_key[1] << 12) |
                 ((uint32_t)pc1_key[2] << 4)  |
                 ((uint32_t)pc1_key[3] >> 4);

    uint32_t d = (((uint32_t)pc1_key[3] & 0x0F) << 24) |
                 ((uint32_t)pc1_key[4] << 16) |
                 ((uint32_t)pc1_key[5] << 8)  |
                 (uint32_t)pc1_key[6];

    for (size_t i = 0; i < 16; ++i)
    {
        size_t shift_count = des_tables_storage::DES_KEY_SHIFTS[i];

        auto left_shift_28 = [](uint32_t val, size_t shift)
        {
            return ((val << shift) | (val >> (28 - shift))) & 0x0FFFFFFF;
        };

        c = left_shift_28(c, shift_count);
        d = left_shift_28(d, shift_count);

        bytes_t combined_cd(7);
        combined_cd[0] = (uint8_t)(c >> 20);
        combined_cd[1] = (uint8_t)(c >> 12);
        combined_cd[2] = (uint8_t)(c >> 4);
        combined_cd[3] = (uint8_t)((c << 4) | (d >> 24));
        combined_cd[4] = (uint8_t)(d >> 16);
        combined_cd[5] = (uint8_t)(d >> 8);
        combined_cd[6] = (uint8_t)d;

        round_keys.push_back(p_box(combined_cd,
                                   des_tables_storage::PC2,
                                   bit_order::big_endian,
                                   bit_index::one));
    }
    return round_keys;
}

bytes_t des_cipher::des_feistel_transform::transform(const bytes_t& right_half, const round_key_t& round_key)
{
    if (right_half.size() != 4)
    {
        throw std::invalid_argument("DES Feistel: right half must be 4 bytes");
    }
    if (round_key.size() != 6)
    {
        throw std::invalid_argument("DES round key must be 6 bytes");
    }

    bytes_t expanded = p_box(right_half,
                             des_tables_storage::E,
                             bit_order::big_endian,
                             bit_index::one);
    bytes_t xored(6);
    for(size_t i = 0; i < 6; ++i)
    {
        xored[i] = expanded[i] ^ round_key[i];
    }

    bytes_t sbox_merged(4, 0);

    for (size_t i = 0; i < 8; ++i)
    {
        uint8_t six_bits = 0;
        for (size_t bit = 0; bit < 6; ++bit) {
            size_t abs_bit_idx = i * 6 + bit;
            bool bit_val = (xored[abs_bit_idx / 8] >> (7 - (abs_bit_idx % 8))) & 1;
            six_bits = (six_bits << 1) | bit_val;
        }

        uint8_t row = (((six_bits >> 5) & 0x01) << 1) | (six_bits & 0x01);
        uint8_t col = (six_bits >> 1) & 0x0F;
        uint8_t four_bits = des_tables_storage::SBOXES[i][row * 16 + col];

        if (i % 2 == 0) {
            sbox_merged[i / 2] |= (four_bits << 4);
        } else {
            sbox_merged[i / 2] |= (four_bits & 0x0F);
        }
    }
    return p_box(sbox_merged,
                 des_tables_storage::P,
                 bit_order::big_endian,
                 bit_index::one);
}