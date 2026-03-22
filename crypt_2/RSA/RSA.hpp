//
// Created by mvideo on 22/03/26.
//

#ifndef CRYPT_2_RSA_HPP
#define CRYPT_2_RSA_HPP

#include"../inc/interfaces.hpp"
#include "../primality_tests/BasePrimalityTest.hpp"

class RSACipher : public i_cipher
{
private:
    bigint exponent;
    bigint modulus;
    size_t modulesByteSize;

public:
    RSACipher(
            const bigint& keyExponent,
            const bigint& keyModulus
            );
    ~RSACipher() override;
    void setup_keys(const bytes_t & key) override;
    size_t get_block_size() const override;
    bytes_t encrypt_block(const bytes_t & block) override;
    bytes_t decrypt_block(const bytes_t & block) override;
    size_t get_full_modulus_size() const {return modulesByteSize;}

private:

    void calculateModulusSize()
    {
        bigint temp = modulus;
        modulesByteSize = 0;
        if(temp == 0) return;
        while(temp > 0)
        {
            temp >>= 8;
            modulesByteSize++;
        }
    }

    bigint bytesToBigInt(const bytes_t& bytes) const
    {
        bigint res = 0;
        for(byte_t b : bytes)
        {
            res = (res << 8) | b;
        }
        return res;
    }

    bytes_t bigintToBytes(bigint num, size_t targetSize) const
    {
        bytes_t bytes(targetSize);
        for(int i = targetSize - 1; i >= 0; --i)
        {
            bytes[i] = (num & 0xFF).convert_to<byte_t>();
            num >>= 8;
        }
        return bytes;
    }

};


#endif //CRYPT_2_RSA_HPP
