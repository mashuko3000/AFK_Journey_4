#include "gmurman.hpp"
#include <cmath>

void task_57(const int64_t trials)
{
    if(trials <= 0) throw std::invalid_argument("Number of trials must be positive.");

    int64_t not_more_than_three = 0;
    int64_t at_least_two = 0;

    const double p1 = 0.6;
    const double p2 = 0.7;
    const double p3 = 0.8;
    const double p4 = 0.9;

    double P_A = 0.0;
    double P_B = 0.0;

    for(int64_t i= 0; i < trials; ++i)
    {
        int result = 0;

        if (((double)std::rand() / RAND_MAX) < p1) result++;
        if (((double)std::rand() / RAND_MAX) < p2) result++;
        if (((double)std::rand() / RAND_MAX) < p3) result++;
        if (((double)std::rand() / RAND_MAX) < p4) result++;

        if(result <= 3) not_more_than_three++;
        if(result>=2) at_least_two++;
    }
    std::cout << "\nResults for Task 57:\n";
    std::cout << "a) No more than 3 boxes (<=3) P(A) = " << (double)not_more_than_three / trials << "\n";
    std::cout << "b) No less than 2 boxes (>=2) P(B) = " << (double)at_least_two / trials << std::endl;
}

void task_58(const int trials)
{
    int success_a = 0;
    int success_b = 0;

    for (int i = 0; i < trials; ++i)
    {
        int d1 = std::rand() % 6 +1;
        int d2 = std::rand() % 6 +1;
        int d3 = std::rand() % 6 +1;

        if(d1 == 5 && d2 == 5 && d3 == 5)
        {
            success_a++;
        }
        if(d1 == d2 == d3)
        {
            success_b++;
        }
    }

    std::cout << "\nResults for Task 58:\n";
    std::cout << "a) All fives (5,5,5) P(A) = " << (double)success_a / trials << " (Theory: ~0.0046)\n";
    std::cout << "b) All same faces P(B) = " << (double)success_b / trials << " (Theory: ~0.0278)" << std::endl;
}

void task_59(const int trials)
{
    int count_a = 0;
    int count_b = 0;
    int count_c = 0;

    for (int i = 0; i < trials; ++i)
    {
        int d1 = std::rand() % 6 + 1;
        int d2 = std::rand() % 6 + 1;
        int d3 = std::rand() % 6 + 1;

        int ones = (d1 == 1) + (d2 == 1) + (d3 == 1);
        if (ones == 2) count_a++;

        if ((d1 == d2 && d1 != d3) ||
            (d1 == d3 && d1 != d2) ||
            (d2 == d3 && d2 != d1))
        {
            count_b++;
        }

        if (d1 != d2 && d1 != d3 && d2 != d3)
        {
            count_c++;
        }
    }

    std::cout << "\nResults for Task 59:\n";
    std::cout << "a) Two 1s and other P(A) = " << (double)count_a / trials << " (Theory: ~0.0694)\n";
    std::cout << "b) Exactly one pair P(B) = " << (double)count_b / trials << " (Theory: ~0.4167)\n";
    std::cout << "c) All different P(C) = " << (double)count_c / trials << " (Theory: ~0.5556)\n";
}
void task_61(const int trials)
{
    const double p_hit = 0.8;
    const double threshold = 0.4;

    double current_prob = 1.0;
    int n = 0;

    while (current_prob >= threshold)
    {
        n++;
        int total_no_misses = 0;

        for (int i = 0; i < trials; ++i)
        {
            bool has_miss = false;
            for (int shot = 0; shot < n; ++shot)
            {
                if (((double)std::rand() / RAND_MAX) >= p_hit)
                {
                    has_miss = true;
                    break;
                }
            }
            if (!has_miss) total_no_misses++;
        }

        current_prob = (double)total_no_misses / trials;
        std::cout << "Testing n = " << n << " | Prob: " << current_prob << "\n";

        if (n > 100) break;
    }


    std::cout << "\nResults for Task 61:\n";
    std::cout << "Final Answer: n = " << n << " (Prob " << current_prob << " < " << threshold << ")\n"
    << "(Theory: n = 5)" << std::endl;
}
void task_62(const int trials)
{
    const double PI = 3.141592653589793;
    const double p_tri = (3.0 * std::sqrt(3.0)) / (4.0 * PI);
    const double p_seg = (1.0 - p_tri) / 3.0;

    int count_a = 0;
    int count_b = 0;

    for (int i = 0; i < trials; ++i)
    {
        int in_tri = 0;
        int in_seg1 = 0;
        int in_seg2 = 0;
        int in_seg3 = 0;

        for (int dot = 0; dot < 4; ++dot)
        {
            double r = (double)std::rand() / RAND_MAX;
            if (r < p_tri) in_tri++;
            else if (r < p_tri + p_seg) in_seg1++;
            else if (r < p_tri + 2.0 * p_seg) in_seg2++;
            else in_seg3++;
        }

        if (in_tri == 4) count_a++;

        if (in_tri == 1 && in_seg1 == 1 && in_seg2 == 1 && in_seg3 == 1) {
            count_b++;
        }
    }
    std::cout << "\nResults for Task 62:\n";
    std::cout << "a) All 4 in triangle: " << (double)count_a / trials << " (Theory: ~0.0292)\n";
    std::cout << "b) 1 in tri, 1 in each segment: " << (double)count_b / trials << " (Theory: ~0.0741)" << std::endl;
}
void task_63(const int trials)
{
    int success = 0;

    for(int i = 0; i < trials; ++i)
    {
        int part1 = 0;
        int part2 = 0;
        int part3 = 0;

        for(int dot = 0; dot < 3; ++dot)
        {
            double pos = (double)std::rand() / RAND_MAX;

            if (pos < 1.0 / 3.0) part1++;
            else if (pos < 2.0 / 3.0) part2++;
            else part3++;
        }

        if (part1 == 1 && part2 == 1 && part3 == 1)
        {
            success++;
        }
    }
    std::cout << "\nResults for Task 63:\n";
    std::cout << "Simulated Prob: " << (double)success / trials << "(Theory: ~" << 2.0 / 9.0 << ")" << std::endl;
}
void task_65(const int trials)
{
    int success = 0;
    for(int i = 0; i < trials; ++i)
    {
        int current_winning = 5;
        int current_total = 100;
        int winning_drawn = 0;

        for (int draw = 0; draw < 2; ++draw)
        {
            if (std::rand() % current_total < current_winning)
            {
                winning_drawn++;
                current_winning--;
            }
            current_total--;
        }

        if (winning_drawn == 2)
        {
            success++;
        }
    }
    std::cout << "\nResults for Task 65 (Lottery):\n";
    std::cout << "Simulated Prob: " << (double)success / trials << "(Theory: ~" << 1.0 / 495.0 << ")" << std::endl;
}
void task_67(const int trials)
{
    int success = 0;
    for(int i = 0; i < trials; ++i)
    {
        int current_painted = 6;
        int current_total = 10;
        int painted_drawn = 0;

        for (int draw = 0; draw < 4; ++draw)
        {
            if (std::rand() % current_total < current_painted)
            {
                painted_drawn++;
                current_painted--;
            }
            current_total--;
        }
        if (painted_drawn == 4)
        {
            success++;
        }
    }
    std::cout << "Results for Task 67:\n";
    std::cout << "Simulated Prob: " << (double)success / trials << " (Theory ~" << 1.0 / 14.0 << ")" <<std::endl;
}
void task_68(const int trials)
{
    int success_a = 0;
    int success_b = 0;

    for (int i = 0; i < trials; ++i)
    {
        int urn[] = {1, 2, 3, 4, 5};
        int drawn[3];
        int current_size = 5;

        for (int draw = 0; draw < 3; ++draw)
        {
            int idx = std::rand() % current_size;
            drawn[draw] = urn[idx];

            for (int j = idx; j < current_size - 1; ++j)
            {
                urn[j] = urn[j + 1];
            }
            current_size--;
        }
        if (drawn[0] == 1 && drawn[1] == 4 && drawn[2] == 5)
        {
            success_a++;
        }
        bool has1 = false, has4 = false, has5 = false;
        for (int j = 0; j < 3; ++j)
        {
            if (drawn[j] == 1) has1 = true;
            if (drawn[j] == 4) has4 = true;
            if (drawn[j] == 5) has5 = true;
        }
        if (has1 && has4 && has5)
        {
            success_b++;
        }
    }

    std::cout << "Results for Task 68:\n";
    std::cout << "a) Sequence 1-4-5: " << (double)success_a / trials << " (Theory ~ 0.0167)\n";
    std::cout << "b) Set {1, 4, 5}: " << (double)success_b / trials << " (Theory ~ 0.1)" << std::endl;
}
void task_69(const int trials)
{
    int success = 0;

    for (int i = 0; i < trials; ++i)
    {
        int known_left = 20;
        int total_left = 25;
        int questions_answered = 0;
        
        for (int q = 0; q < 3; ++q)
        {
            if (std::rand() % total_left < known_left)
            {
                questions_answered++;
                known_left--;
            }
            total_left--;
        }
        
        if (questions_answered == 3)
        {
            success++;
        }
    }
    std::cout << "\nResults for Task 69:\n";
    std::cout << "Simulated Prob: " << (double)success / trials << "(Theory~" << 57.0 / 115.0 << ")" << std::endl;
}
void task_70(const int trials)
{
    int success_a = 0;
    int success_b = 0;

    for (int i = 0; i < trials; ++i)
    {
        int total_a = 10;
        bool ok_a = true;
        for (int val = 1; val <= 3; ++val) {
            if (std::rand() % total_a != 0) {
                ok_a = false; break;
            }
            total_a--;
        }
        if (ok_a) success_a++;
        if (std::rand() % 10 == 0 && std::rand() % 10 == 1 && std::rand() % 10 == 2)
            success_b++;
    }

    std::cout << "\nResults for Task 70:\n";
    std::cout << "a) No return: " << (double)success_a / trials << " (Theory~" << 1.0/720.0 << ")\n";
    std::cout << "b) With return: " << (double)success_b / trials << " (Theory: 0.001)" << std::endl;
}