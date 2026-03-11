#include "../inc/tripple_des.hpp"

void triple_des::setup_keys(const bytes_t & key)
{
    if (key.size() != 16 && key.size() != 24)
    {
        throw std::invalid_argument("3DES key must be 16 or 24 bytes (2 or 3 DES keys)");
    }

    bytes_t k1(key.begin(), key.begin() + 8);
    bytes_t k2(key.begin() + 8, key.begin() + 16);
    bytes_t k3;

    if (key.size() == 24)
    {
        k3 = bytes_t(key.begin() + 16, key.end());
    }
    else
    {
        k3 = k1;
    }

    des1.setup_keys(k1);
    des2.setup_keys(k2);

    if (mode == triple_des_mode::EEE3 || mode == triple_des_mode::EDE3)
    {
        des3.setup_keys(k3);
    }
    else
    {
        des3.setup_keys(k1);
    }
}
bytes_t triple_des::encrypt_block(const bytes_t & block)
{
    if (block.size() != 8)
    {
        throw std::invalid_argument("Block must be 8 bytes");
    }

    bytes_t temp = des1.encrypt_block(block);

    if (mode == triple_des_mode::EEE3 || mode == triple_des_mode::EEE2)
    {
        temp = des2.encrypt_block(temp);
        temp = des3.encrypt_block(temp);
    }
    else
    {
        temp = des2.decrypt_block(temp);
        temp = des3.encrypt_block(temp);
    }

    return temp;
}
bytes_t triple_des::decrypt_block(const bytes_t & block)
{
    if (block.size() != 8)
    {
        throw std::invalid_argument("Block must be 8 bytes");
    }

    bytes_t temp;

    if (mode == triple_des_mode::EEE3 || mode == triple_des_mode::EEE2)
    {
        temp = des3.decrypt_block(block);
        temp = des2.decrypt_block(temp);
        temp = des1.decrypt_block(temp);
    }
    else
    {
        temp = des3.decrypt_block(block);
        temp = des2.encrypt_block(temp);
        temp = des1.decrypt_block(temp);
    }
    return temp;
}