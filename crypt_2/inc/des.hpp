#ifndef CRYPT_2_DES_HPP
#define CRYPT_2_DES_HPP

#include "feistel_with_hooks.hpp"

#include "../crypt_1/inc/s_box.hpp"
#include "../crypt_1/inc/p_box.hpp"
#include "des_tables.hpp"

class des_cipher final : public feistel_with_hooks, private des_tables_storage
{
public:
    des_cipher() :
            feistel_with_hooks(m_expander, m_transform, 16),
            m_expander(),
            m_transform()
    {}
protected:
    bytes_t pre_encrypt(const bytes_t& block) const override;
    bytes_t post_encrypt(const bytes_t& block) const override;

    bytes_t pre_decrypt(const bytes_t& block) const override;
    bytes_t post_decrypt(const bytes_t& block) const override;


private:

    class des_key_expander : public i_key_expander
    {
    public:
        round_keys_t expand(const bytes_t& master_key) const override;
    };

private:

    class des_feistel_transform : public i_feistel_transform
    {
        bytes_t transform(const bytes_t& right_half, const round_key_t& round_key) override;
    };

private:
    des_key_expander m_expander;
    des_feistel_transform m_transform;
};

#endif //CRYPT_2_DES_HPP
