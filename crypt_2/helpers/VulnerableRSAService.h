//
// Created by maria on 30.03.2026.
//

#ifndef CRYPT_2_VULNERABLERSASERVICE_H
#define CRYPT_2_VULNERABLERSASERVICE_H

#include "../core_interfaces/i_vulnerable_rsa_service.h"
#include "../helpers/PrimeGenerator.h"
#include "../helpers/NumberTheoryService.hpp"

class VulnerableRSAService : public IVulnerableRSAService
{
public:
    const PrimeGenerator primeGenerator;

public:
    ~VulnerableRSAService() override = default;
    explicit VulnerableRSAService(const PrimeGenerator& pg) : primeGenerator(pg) {}
    [[nodiscard]] RSAKeyPair generateFermatVulnerable(size_t bit_length) const override;
    [[nodiscard]] RSAKeyPair generateWienerVulnerable(size_t bit_length) const override;
};


#endif //CRYPT_2_VULNERABLERSASERVICE_H
