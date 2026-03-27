//
// Created by mvideo on 22/03/26.
//

#include "RSA.hpp"

RSACipher::RSACipher(
        const bigint& keyExponent,
        const bigint& keyModulus
) : exponent(keyExponent), modulus(keyModulus)
{
    if(modulus<= 0)
    {
        throw std::invalid_argument("Modulus mast be positive");
    }
    calculateModulusSize();
}
RSACipher::~RSACipher()
{

}
void RSACipher::setup_keys(const bytes_t & key)
{
    throw std::logic_error("Use constructor or specific RSA key setter for RSA");
}

size_t RSACipher::get_block_size() const
{
    return modulesByteSize > 1 ? modulesByteSize - 1 : 1;
}
bytes_t RSACipher::encrypt_block(const bytes_t & block)
{
    if(block.size() > get_block_size())
    {
        throw std::invalid_argument("Plaintext block too large");
    }
    bigint m = bytesToBigInt(block);
    if(m>= modulus) throw std::invalid_argument("Plaintext block too large");

    bigint c = NumberTheoryService::modPow(m, exponent, modulus);
    return bigintToBytes(c, modulesByteSize);
}
bytes_t RSACipher::decrypt_block(const bytes_t & block)
{
    if(block.size() != modulesByteSize)
    {
        throw std::invalid_argument("Invalid ciphertext block size");
    }

    bigint c = bytesToBigInt(block);
    bigint m = NumberTheoryService::modPow(c, exponent, modulus);

    return bigintToBytes(m, get_block_size());
}