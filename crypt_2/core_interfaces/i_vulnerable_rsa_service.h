//
// Created by maria on 30.03.2026.
//

#ifndef CRYPT_2_I_VULNERABLE_RSA_SERVICE_H
#define CRYPT_2_I_VULNERABLE_RSA_SERVICE_H

#include"rsa_types.h"

class IVulnerableRSAService
{
public:
    virtual ~IVulnerableRSAService() = default;
    [[nodiscard]] virtual RSAKeyPair generateFermatVulnerable(size_t bit_length) const = 0;
    [[nodiscard]] virtual RSAKeyPair generateWienerVulnerable(size_t bit_length) const = 0;
};

#endif //CRYPT_2_I_VULNERABLE_RSA_SERVICE_H
