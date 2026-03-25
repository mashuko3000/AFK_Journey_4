//
// Created by mvideo on 25/03/26.
//

#ifndef PROB_THEORY_6_GMURMAN_HPP
#define PROB_THEORY_6_GMURMAN_HPP

#include"../Core/IExperiment.hpp"
#include"../Core/RandomGenerator.hpp"
#include"../Core/MathUtilits.hpp"

#include <sstream>
#include <iomanip>
#include <vector>

class Task109 : public IExperiment<std::string> {
    const double p[4] = {0.1, 0.2, 0.3, 0.4};
public:
    std::string getName() const override { return "Task 109: Conditional failure of lamps 1 and 2"; }

    std::string runEmpirical(long long trials) override {
        long long totalCasesWithTwoFailures = 0;
        long long firstTwoFailed = 0;

        for (long long i = 0; i < trials; ++i) {
            bool failed[4];
            int failCount = 0;
            for (int j = 0; j < 4; ++j) {
                failed[j] = RandomGenerator::bernoulli(p[j]);
                if (failed[j]) failCount++;
            }

            if (failCount == 2) {
                totalCasesWithTwoFailures++;
                if (failed[0] && failed[1]) firstTwoFailed++;
            }
        }

        double result = (totalCasesWithTwoFailures == 0) ? 0.0 :
                        static_cast<double>(firstTwoFailed) / totalCasesWithTwoFailures;
        return std::to_string(result);
    }

    std::string calculateTheoretical() const override {
        double p_12 = p[0] * p[1] * (1-p[2]) * (1-p[3]);

        double p_total_2 = 0.0;
        for(int i=0; i<3; ++i) {
            for(int j=i+1; j<4; ++j) {
                double prob = 1.0;
                for(int k=0; k<4; ++k) {
                    if(k == i || k == j) prob *= p[k];
                    else prob *= (1 - p[k]);
                }
                p_total_2 += prob;
            }
        }
        return std::to_string(p_12 / p_total_2);
    }
};

class Task111 : public IExperiment<std::string> {
    double empiricalProb(int n, int k, long long trials, bool exact = true) const {
        long long success = 0;
        for (long long i = 0; i < trials; ++i) {
            int wins = 0;
            for (int j = 0; j < n; ++j) {
                if (RandomGenerator::bernoulli(0.5)) wins++;
            }
            if (exact ? (wins == k) : (wins >= k)) success++;
        }
        return static_cast<double>(success) / trials;
    }
public:
    std::string getName() const override { return "Task 111: Chess probabilities"; }

    std::string runEmpirical(long long trials) override {
        double p1_2 = empiricalProb(2, 1, trials, true);
        double p2_4 = empiricalProb(4, 2, trials, true);
        double p_ge2_4 = empiricalProb(4, 2, trials, false);
        double p_ge3_5 = empiricalProb(5, 3, trials, false);

        std::stringstream ss;
        ss << "a) 1 of 2 (" << p1_2 << ") vs 2 of 4 (" << p2_4 << "); ";
        ss << "b) >=2 of 4 (" << p_ge2_4 << ") vs >=3 of 5 (" << p_ge3_5 << ")";
        return ss.str();
    }

    std::string calculateTheoretical() const override {
        double p1_2 = MathUtilits::bernoulliProb(2, 1, 0.5);
        double p2_4 = MathUtilits::bernoulliProb(4, 2, 0.5);

        double p_ge2_4 = 1.0 - MathUtilits::bernoulliProb(4, 0, 0.5) - MathUtilits::bernoulliProb(4, 1, 0.5);
        double p_ge3_5 = MathUtilits::bernoulliProb(5, 3, 0.5) + MathUtilits::bernoulliProb(5, 4, 0.5) + MathUtilits::bernoulliProb(5, 5, 0.5);

        std::stringstream ss;
        ss << "a) 1 of 2 (" << p1_2 << ") vs 2 of 4 (" << p2_4 << ") -> 1 of 2 is higher; ";
        ss << "b) >=2 of 4 (" << p_ge2_4 << ") vs >=3 of 5 (" << p_ge3_5 << ") -> >=2 of 4 is higher";
        return ss.str();
    }
};

class Task112 : public IExperiment<std::string> {
public:
    std::string getName() const override { return "Task 112: Coin tosses"; }

    std::string runEmpirical(long long trials) override {
        long long lessThanTwo = 0, twoOrMore = 0;
        for (long long i = 0; i < trials; ++i) {
            int heads = 0;
            for (int j = 0; j < 5; ++j) {
                if (RandomGenerator::bernoulli(0.5)) heads++;
            }
            if (heads < 2) lessThanTwo++;
            else twoOrMore++;
        }
        std::stringstream ss;
        ss << "a) <2: " << (double)lessThanTwo / trials << ", b) >=2: " << (double)twoOrMore / trials;
        return ss.str();
    }

    std::string calculateTheoretical() const override {
        double p0 = MathUtilits::bernoulliProb(5, 0, 0.5);
        double p1 = MathUtilits::bernoulliProb(5, 1, 0.5);
        double less = p0 + p1;
        double more = 1.0 - less;

        std::stringstream ss;
        ss << "a) <2: " << less << ", b) >=2: " << more;
        return ss.str();
    }
};

class Task113 : public IExperiment<std::string> {
public:
    std::string getName() const override { return "Task 113: Events A and B"; }

    std::string runEmpirical(long long trials) override {
        long long a_success = 0, b_success = 0;
        for (long long i = 0; i < trials; ++i) {
            int a_count_4 = 0;
            for (int j = 0; j < 4; ++j) if (RandomGenerator::bernoulli(0.4)) a_count_4++;
            if (a_count_4 >= 3) a_success++;

            int a_count_5 = 0;
            for (int j = 0; j < 5; ++j) if (RandomGenerator::bernoulli(0.8)) a_count_5++;
            if (a_count_5 >= 4) b_success++;
        }
        std::stringstream ss;
        ss << "a) P(A>=3): " << (double)a_success / trials << ", b) P(B): " << (double)b_success / trials;
        return ss.str();
    }

    std::string calculateTheoretical() const override {
        double a_prob = MathUtilits::bernoulliProb(4, 3, 0.4) + MathUtilits::bernoulliProb(4, 4, 0.4);
        double b_prob = MathUtilits::bernoulliProb(5, 4, 0.8) + MathUtilits::bernoulliProb(5, 5, 0.8);
        std::stringstream ss;
        ss << "a) P(A>=3): " << a_prob << ", b) P(B): " << b_prob;
        return ss.str();
    }
};

class Task114 : public IExperiment<std::string> {
    double simulate(int totalElements, long long trials) const {
        long long success = 0;
        for (long long i = 0; i < trials; ++i) {
            int working = 0;
            for (int j = 0; j < totalElements; ++j) {
                if (RandomGenerator::bernoulli(0.9)) working++;
            }
            if (working >= 3) success++;
        }
        return static_cast<double>(success) / trials;
    }
    double theoretical(int totalElements) const {
        double prob = 0.0;
        for (int k = 3; k <= totalElements; ++k) {
            prob += MathUtilits::bernoulliProb(totalElements, k, 0.9);
        }
        return prob;
    }
public:
    std::string getName() const override { return "Task 114: Device Reliability"; }

    std::string runEmpirical(long long trials) override {
        std::stringstream ss;
        ss << "a) No reserve: " << simulate(3, trials) << ", b) 1 reserve: " << simulate(4, trials) << ", c) 2 reserve: " << simulate(5, trials);
        return ss.str();
    }

    std::string calculateTheoretical() const override {
        std::stringstream ss;
        ss << "a) No reserve: " << theoretical(3) << ", b) 1 reserve: " << theoretical(4) << ", c) 2 reserve: " << theoretical(5);
        return ss.str();
    }
};

class Task115 : public IExperiment<std::string> {
public:
    std::string getName() const override { return "Task 115: 5 children"; }

    std::string runEmpirical(long long trials) override {
        long long a=0, b=0, c=0, d=0;
        for (long long i = 0; i < trials; ++i) {
            int boys = 0;
            for(int j=0; j<5; ++j) if(RandomGenerator::bernoulli(0.51)) boys++;

            if (boys == 2) a++;
            if (boys <= 2) b++;
            if (boys > 2) c++;
            if (boys >= 2 && boys <= 3) d++;
        }
        std::stringstream ss;
        ss << "a: " << (double)a/trials << ", b: " << (double)b/trials << ", c: " << (double)c/trials << ", d: " << (double)d/trials;
        return ss.str();
    }

    std::string calculateTheoretical() const override {
        double p[6];
        for(int i=0; i<=5; ++i) p[i] = MathUtilits::bernoulliProb(5, i, 0.51);

        double a = p[2];
        double b = p[0] + p[1] + p[2];
        double c = p[3] + p[4] + p[5];
        double d = p[2] + p[3];

        std::stringstream ss;
        ss << "a: " << a << ", b: " << b << ", c: " << c << ", d: " << d;
        return ss.str();
    }
};

class Task116 : public IExperiment<std::string> {
public:
    std::string getName() const override { return "Task 116: Points on segment 2:1"; }

    std::string runEmpirical(long long trials) override {
        long long success = 0;
        for (long long i = 0; i < trials; ++i) {
            int left = 0;
            for (int j = 0; j < 4; ++j) {
                if (RandomGenerator::uniformReal(0.0, 1.0) < 2.0 / 3.0) left++;
            }
            if (left == 2) success++;
        }
        return std::to_string(static_cast<double>(success) / trials);
    }

    std::string calculateTheoretical() const override {
        return std::to_string(MathUtilits::bernoulliProb(4, 2, 2.0 / 3.0));
    }
};

class Task117 : public IExperiment<std::string> {
    double ratio_x_a = 0.3;
public:
    std::string getName() const override { return "Task 117: Distance < x (assumed x/a = 0.3)"; }

    std::string runEmpirical(long long trials) override {
        long long success = 0;
        for (long long i = 0; i < trials; ++i) {
            int lessThanX = 0;
            for (int j = 0; j < 5; ++j) {
                if (RandomGenerator::uniformReal(0.0, 1.0) < ratio_x_a) lessThanX++;
            }
            if (lessThanX == 2) success++;
        }
        return std::to_string(static_cast<double>(success) / trials);
    }

    std::string calculateTheoretical() const override {
        return std::to_string(MathUtilits::bernoulliProb(5, 2, ratio_x_a));
    }
};

class Task118 : public IExperiment<std::string> {
public:
    std::string getName() const override { return "Task 118: 8 points in 4 equal parts"; }

    std::string runEmpirical(long long trials) override {
        long long success = 0;
        for (long long i = 0; i < trials; ++i) {
            int parts[4] = {0, 0, 0, 0};
            for (int j = 0; j < 8; ++j) {
                double val = RandomGenerator::uniformReal(0.0, 1.0);
                if(val < 0.25) parts[0]++;
                else if(val < 0.5) parts[1]++;
                else if(val < 0.75) parts[2]++;
                else parts[3]++;
            }
            if (parts[0] == 2 && parts[1] == 2 && parts[2] == 2 && parts[3] == 2) success++;
        }
        return std::to_string(static_cast<double>(success) / trials);
    }

    std::string calculateTheoretical() const override {
        double prob = 2520.0 / 65536.0;
        return std::to_string(prob);
    }
};

#endif //PROB_THEORY_6_GMURMAN_HPP
