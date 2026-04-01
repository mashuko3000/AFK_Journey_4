//
// Created by maria on 31.03.2026.
//
#include <gtest/gtest.h>
#include <boost/multiprecision/cpp_int.hpp>
#include "wiener_attack.h"
#include"FractionService.h"

class WienerAttackTest : public ::testing::Test {
protected:
    FractionService fractionService;
    WienerAttack attack{fractionService};
};

TEST_F(WienerAttackTest, SuccessfulFactorizationKnownValues) {
    auto result = attack.execute(90581, 17993);
    EXPECT_EQ(result.first, 239);
    EXPECT_EQ(result.second, 379);
}

TEST_F(WienerAttackTest, ThrowsOnInvalidInputs) {
EXPECT_THROW(attack.execute(0, 17993), std::invalid_argument);
EXPECT_THROW(attack.execute(90581, 0), std::invalid_argument);
EXPECT_THROW(attack.execute(90581, 99999), std::invalid_argument);
}
