//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_RIJNDAELSBOX_HPP
#define CRYPT_RIJNDAELSBOX_HPP

#include "../math/GaloisField.hpp"
#include "../common/Common.h"
#include<iostream>
#include <array>

namespace crypto
{
    class RijndaelSBox final
    {
        using Byte = uint8_t;
        using SBoxTable = std::array<Byte, 256>;
    public:
        [[nodiscard]] static SBoxTable buildSBox(const GaloisField<8>& field);
        [[nodiscard]] static SBoxTable buildInvSBox(const SBoxTable& sbox);

        static void print (const SBoxTable& table, std::ostream& = std::cout);

    private:

        static constexpr uint8_t kAffineMatrix[8][8] =
                {
                        {1, 0, 0, 0, 1, 1, 1, 1},
                        {1, 1, 0, 0, 0, 1, 1, 1},
                        {1, 1, 1, 0, 0, 0, 1, 1},
                        {1, 1, 1, 1, 0, 0, 0, 1},
                        {1, 1, 1, 1, 1, 0, 0, 0},
                        {0, 1, 1, 1, 1, 1, 0, 0},
                        {0, 0, 1, 1, 1, 1, 1, 0},
                        {0, 0, 0, 1, 1, 1, 1,1}
                };
        static constexpr Byte kAffine = 0x63u;
        /*
        [[nodiscard]] static Byte applyAffine(Byte x) noexcept
        {
            Byte res = kAffine;
            for(int i = 0; i < 8; ++i)
            {
                Byte dot = 0;
                for(int j = 0; j < 8; ++j)
                {
                    if(kAffineMatrix[i][j])
                    {
                        dot ^= ((x>> j) & 1u);
                    }
                }
                if(dot) res ^= (1u << i);
            }
            return res;
        }*/


        [[nodiscard]] static Byte applyAffine(Byte b) noexcept
        {
            uint8_t x = b;
            uint8_t res = x;

            for(int i = 0; i < 4; ++i)
            {
                x = static_cast<uint8_t>((x << 1) | (x >> 7));
                res ^= x;
            }
            return res^0x63u;
        }

    };
}

#endif //CRYPT_RIJNDAELSBOX_HPP
