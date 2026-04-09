//
// Created by maria on 07.04.2026.
//
#include "../../inc/layers/RijndaelSBox.hpp"
#include <iomanip>

namespace crypto
{
    RijndaelSBox::SBoxTable RijndaelSBox::buildSBox(const GaloisField<8>& field)
    {
        if (field.modulus().degree() != 8)
        {
            throw std::invalid_argument("GaloisField must be GF(2^8) for Rijndael S-box");
        }

        SBoxTable sbox{};
        for(uint32_t i = 0; i < 256; ++i)
        {
            const Byte val = static_cast<Byte>(i);

            const Byte inv = (val == 0) ? Byte{0}
            : static_cast<Byte>(field.inverse(GFPolynomial<8>{static_cast<uint64_t>(val)}).value());

            sbox[val] = applyAffine(inv);
        }
        return sbox;
    }
    RijndaelSBox::SBoxTable RijndaelSBox::buildInvSBox(const SBoxTable& sbox)
    {
        SBoxTable invSBox{};
        for(uint32_t i = 0; i < 256; ++i)
        {
            invSBox[sbox[i]] = static_cast<Byte>(i);
        }
        return invSBox;
    }
    void RijndaelSBox::print(const RijndaelSBox::SBoxTable& table, std::ostream& os)
    {
        os << "   0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n";
        os << " +------------------------------------------------\n";

        for (uint32_t i = 0; i < 256; ++i) {
            if (i % 16 == 0) {
                os << std::hex << (i >> 4) << " | ";
            }
            os << std::setw(2) << std::setfill('0') << static_cast<int>(table[i]) << " ";
            if (i % 16 == 15) os << "\n";
        }
        os << std::dec << std::setfill(' ');
    }
}