//
// Created by maria on 31.03.2026.
//
#include <gtest/gtest.h>
#include <boost/multiprecision/cpp_int.hpp>
#include "fermat_attack.h"

TEST(FermatAttackTest, SuccessfulFactorization) {
    FermatAttack attack;
    auto result = attack.execute(10403);
    EXPECT_EQ(result.first, 101);
    EXPECT_EQ(result.second, 103);
}

TEST(FermatAttackTest, HandlesPerfectSquare) {
    FermatAttack attack;
    auto result = attack.execute(10201);
    EXPECT_EQ(result.first, 101);
    EXPECT_EQ(result.second, 101);
}

TEST(FermatAttackTest, HandlesEvenModulus) {
    FermatAttack attack;
    auto result = attack.execute(20);
    EXPECT_EQ(result.first, 2);
    EXPECT_EQ(result.second, 10);
}

TEST(FermatAttackTest, ThrowsOnInvalidModulus) {
    FermatAttack attack;
    EXPECT_THROW(attack.execute(0), std::invalid_argument);
    EXPECT_THROW(attack.execute(-10403), std::invalid_argument);
}