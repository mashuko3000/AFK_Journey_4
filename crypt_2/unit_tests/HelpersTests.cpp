//
// Created by maria on 31.03.2026.
//

#include <gtest/gtest.h>
#include <boost/multiprecision/cpp_int.hpp>
#include "MillerRobinPrimalityTest.hpp"
#include "PrimeGenerator.h"
#include "VulnerableRSAService.h"
#include "FractionService.h"

class VulnerableRSAServiceTest : public ::testing::Test
{
protected:
    MillerRabinPrimalityTest mr_test;
    PrimeGenerator generator{mr_test};
    VulnerableRSAService service{generator};
};

TEST_F(VulnerableRSAServiceTest, FermatKeysAreMathematicallyValid)
{
auto keys = service.generateFermatVulnerable(64);
EXPECT_EQ(keys.p * keys.q, keys.n);
}

/*
TEST_F(VulnerableRSAServiceTest, WienerKeysAreMathematicallyValid)
{
    auto keys = service.generateWienerVulnerable(64);
    EXPECT_EQ(keys.p * keys.q, keys.n);
    bigint d4 = keys.d * keys.d * keys.d * keys.d;
    EXPECT_LT(d4 * 81, keys.n);
}*/


TEST_F(VulnerableRSAServiceTest, ThrowsOnTinyBitLength) {
    EXPECT_THROW(service.generateFermatVulnerable(15), std::invalid_argument);
    EXPECT_THROW(service.generateWienerVulnerable(10), std::invalid_argument);
}

class FractionServiceTest : public ::testing::Test {
protected:
    FractionService fs;
};

TEST_F(FractionServiceTest, CalculatesContinuedFractionCorrectly) {
    Fraction f{10, 7};
    auto cf = fs.getContinuedFraction(f);
    std::vector<bigint> expected = {1, 2, 3};
    EXPECT_EQ(cf, expected);
}


TEST_F(FractionServiceTest, ReconstructsFractionFromContinued) {
    std::vector<bigint> cf = {1, 2, 3};
    Fraction f = fs.getFractionFromContinued(cf);
    EXPECT_EQ(f.numerator, 10);
    EXPECT_EQ(f.denominator, 7);
}

TEST_F(FractionServiceTest, CalculatesConvergentsCorrectly) {
    Fraction f{10, 7};
    auto convs = fs.getConvergents(f);
    ASSERT_EQ(convs.size(), 3);
    EXPECT_EQ(convs[0].numerator, 1);  EXPECT_EQ(convs[0].denominator, 1);
    EXPECT_EQ(convs[1].numerator, 3);  EXPECT_EQ(convs[1].denominator, 2);
    EXPECT_EQ(convs[2].numerator, 10); EXPECT_EQ(convs[2].denominator, 7);
}

TEST_F(FractionServiceTest, ValidatesFractionInputs) {
    Fraction zero_denom{5, 0};
    Fraction negative_num{-1, 5};
    Fraction not_irreducible{10, 8};

    EXPECT_THROW(fs.getContinuedFraction(zero_denom), std::invalid_argument);
    EXPECT_THROW(fs.getContinuedFraction(negative_num), std::invalid_argument);
    EXPECT_THROW(fs.getContinuedFraction(not_irreducible), std::invalid_argument);
}

TEST_F(FractionServiceTest, ThrowsOnEmptyContinuedFraction) {
    std::vector<bigint> empty_cf;
    EXPECT_THROW(fs.getFractionFromContinued(empty_cf), std::invalid_argument);
}