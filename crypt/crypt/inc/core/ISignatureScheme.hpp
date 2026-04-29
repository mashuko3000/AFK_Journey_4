//
// Created by maria on 18.04.2026.
//

#ifndef CRYPT_ISIGNATURESCHEME_HPP
#define CRYPT_ISIGNATURESCHEME_HPP

#include "../common/CryptoTypes.hpp"

#include <string>

namespace crypto{
class ISignatureScheme
{
public:
    virtual ~ISignatureScheme() = default;
    [[nodiscard]] virtual std::string name() const = 0;
    [[nodiscard]] virtual bytes_t sign(const bytes_t& message) = 0;
    [[nodiscard]] virtual bool verify(const bytes_t& message, const bytes_t& signature) = 0;
};
}


#endif //CRYPT_ISIGNATURESCHEME_HPP
