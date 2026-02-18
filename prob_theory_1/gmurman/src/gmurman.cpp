#include "../inc/gmurman.hpp"

void _6gmurman(const int simulations)
{
    const int n = 10;
    const int total = 1000;

    const int m3 = 8;
    const int m2 = 12 * (n - 2);
    const int m1 = 6 * (n-2) * (n - 2);
    const int m0 = n - m1 - m2 - m3;

    int res1 = 0;
    int res2 = 0;
    int res3 = 0;

    for (int i = 0; i < simulations; ++i)
    {
        int r = std::rand() % total;
        if (r >= 512 && r < 896)
        {
            res1++;
        }
        else if(r >= 896 && r < 992)
        {
            res2++;
        }
        else if (r >= 992)
        {
            res3++;
        }
    }

    std::cout << "Results (std::rand):\n";
    std::cout << "a) 1_task face:  " << (double)res1 / simulations << "\n";
    std::cout << "b) s_task faces: " << (double)res2 / simulations << "\n";
    std::cout << "c) 3 faces: " << (double)res3 / simulations << "\n";
}

void _8gmurman(const int simulations)
{
    const int n = 6;
    int success = 0;

    for (int i = 0; i < simulations; ++i)
    {
        bool fail = false;
        for (int cubes_left = 6; cubes_left > 0; --cubes_left)
        {
            if (std::rand() % cubes_left != 0)
            {
                fail = true;
                break;
            }
        }
        if (!fail) success++;
    }
    std::cout << "Successes: " << success << std::endl;
    std::cout << "Prob: " << (double)success / simulations << std::endl;
}

void _13gmurman(const int simulations)
{
    const int n = 100;
    int success = 0;

    for(int i = 0; i < simulations; ++i)
    {
        if(std::rand() % 100 < 10)
        {
            success++;
        }
    }
    std::cout << "Success rate: " << (double)success / simulations << " (Theory: 0.1_task)" << std::endl;
}

void _15gmurman(const int simulations)
{
    const int n = 5;
    int success = 0;

    for (int i = 0; i < simulations; ++i)
    {
        if (std::rand() % 5 < 3)
        {
            if (std::rand() % 4 < 2)
            {
                success++;
            }
        }
    }

    std::cout << "Success rate: " << (double)success / simulations << std::endl;
    std::cout << "Theory: 0.3" << std::endl;
}

void _19gmurman(const int simulations)
{
    int success = 0;

    for (int i = 0; i < simulations; ++i)
    {
        int lviv_count = 0;
        int current_lviv_in_stock = 10;
        int current_total_in_stick = 15;

        for(int draw = 0; draw < 5; ++draw)
        {
            if(std::rand() % current_total_in_stick < current_lviv_in_stock)
            {
                lviv_count++;
                current_lviv_in_stock--;
            }
            current_total_in_stick--;
        }
        if(lviv_count == 3)
        {
            success++;
        }
    }
    std::cout << "Simulated Prob: " << (double)success / simulations << std::endl;
    std::cout << "Theoretical Prob: ~0.3996" << std::endl;
}

void _20gmurman(const int simulations)
{
    int success = 0;

    for (int i = 0; i < simulations; ++i)
    {
        int honors_count = 0;
        int current_honors_in_group = 8;
        int current_total_in_group = 12;

        for (int draw = 0; draw < 9; ++draw)
        {
            if(std::rand() % current_total_in_group < current_honors_in_group)
            {
                honors_count++;
                current_honors_in_group--;
            }
            current_total_in_group--;
        }
        if(honors_count == 5)
        {
            success++;
        }
    }
    std::cout << "Simulated Prob: " << (double)success / simulations << std::endl;
    std::cout << "Theoretical Prob: ~0.2545" << std::endl;
}

void _21gmurman(const int simulations)
{
    int suc_a = 0;
    int suc_b = 0;
    int suc_c = 0;


    for (int i = 0; i < simulations; ++i)
    {
        int painted_count = 0;
        int current_painted = 3;
        int current_total = 5;

        for (int draw = 0; draw < 2; ++draw)
        {
            if (std::rand() % current_total < current_painted)
            {
                painted_count++;
                current_painted--;
            }
            current_total--;
        }
        if (painted_count == 1) suc_a++;
        if (painted_count == 2) suc_b++;
        if (painted_count >= 1) suc_c++;
    }
    std::cout << "a) One painted: " << (double)suc_a / simulations << " (Theory: 0.6)" << std::endl;
    std::cout << "b) Two painted: " << (double)suc_b / simulations << " (Theory: 0.3)" << std::endl;
    std::cout << "v) At least one: " << (double)suc_c / simulations << " (Theory: 0.9)" << std::endl;
}

void _22gmurman(const int simulations)
{
    int success = 0;

    for(int i = 0; i < simulations; ++i)
    {
        bool opened = true;
        for (int draw = 0; draw < 4; ++draw)
        {
            if (std::rand() % 5 != 0)
            {
                opened = false;
                break;
            }
        }
        if (opened)
        {
            success++;
        }
    }
    std::cout << "Simulated Prob: " << (double)success / simulations << std::endl;
    std::cout << "Theoretical Prob: " << 1.0 / 625.0 << " (0.0016)" << std::endl;
}
void _26gmurman(const int simulations)
{
    int success = 0;

    const double L = 20.0;
    const double l = 10.0;

    for(int i =0; i < simulations; ++i)
    {
        double point = ((double)std::rand() / RAND_MAX) * L;
        if(point <= l)
        {
            success++;
        }
    }
    std::cout << "Simulated Prob: " << (double)success / simulations << std::endl;
    std::cout << "Theoretical Prob: 0.5" << std::endl;
}

void _27gmurman(const int simulations)
{
    int success = 0;

    const double L = 100.00;

    for(int i = 0; i < simulations; ++i)
    {
        double x = ((double)std::rand() / RAND_MAX) * L;
        double OB = x;
        double BA = L - x;

        double smaller = (OB < BA) ? OB : BA;
        if(smaller > (L/3.0)) success++;
    }

    std::cout << "Simulated Prob: " << (double)success / simulations << std::endl;
    std::cout << "Theoretical Prob: " << 1.0 / 3.0 << " (~0.3333)" << std::endl;
}
void _28gmurman(const int simulations)
{
    int total_in_big = 0;
    int success = 0;

    const double R = 10.0;
    const double r = 5.0;

    const double R2 = R * R;
    const double r2 = r * r;

    for (int i = 0; i < simulations; ++i)
    {
        double x = ((double)std::rand() / RAND_MAX) * 2 * R - R;
        double y = ((double)std::rand() / RAND_MAX) * 2 * R - R;

        double dist_sq = x * x + y * y;

        if (dist_sq <= R2)
        {
            total_in_big++;

            if (dist_sq <= r2)
            {
                success++;
            }
        }
    }

    std::cout << "Simulated Prob: " << (double)success / total_in_big << std::endl;
    std::cout << "Theoretical Prob (r/R)^s_task: " << r2 / R2 << std::endl;
}
void _29gmurman(const int simulations)
{
    int success = 0;

    const double a = 10.0;
    const double r = 3.0;

    for (int i = 0; i < simulations; ++i)
    {
        double distance_to_line = ((double)std::rand() / RAND_MAX) * a;

        if (distance_to_line > r)
        {
            success++;
        }
    }

    std::cout << "Simulated Prob: " << (double)success / simulations << std::endl;
    std::cout << "Theoretical Prob (1_task - r/a): " << 1.0 - (r / a) << std::endl;
}
void _30gmurman(const int simulations)
{
    int success = 0;

    const double a = 20.0;
    const double r = 2.0;

    for(int i = 0; i < simulations; ++i)
    {
        double x = ((double)std::rand() / RAND_MAX) * a;
        double y = ((double)std::rand() / RAND_MAX) * a;

        double res = a - r;
        if(x >= r && x <= res && y >= r && y<=res)
        {
            success++;
        }
    }
    double side_ratio = (a - 2.0 * r) / a;

    std::cout << "Simulated Prob: " << (double)success / simulations << std::endl;
    std::cout << "Theoretical Prob: " << side_ratio * side_ratio << std::endl;
}

void _32gmurman(const int simulations)
{
    int total_in_big = 0;
    int success = 0;

    const double R = 10.0;
    const double r = 5.0;
    const double R2 = R * R;
    const double r2 = r * r;

    while (total_in_big < simulations)
    {
        double x = ((double)std::rand() / RAND_MAX) * 2 * R - R;
        double y = ((double)std::rand() / RAND_MAX) * 2 * R - R;

        double dist_sq = x * x + y * y;

        if (dist_sq <= R2)
        {
            total_in_big++;

            if (dist_sq > r2)
            {
                success++;
            }
        }
    }

    std::cout << "Simulated Prob: " << (double)success / total_in_big << std::endl;
    std::cout << "Theoretical Prob (0.75): 0.75" << std::endl;
}

void _45gmurman(const int simulations)
{
    int success = 0;

    for (int i = 0; i < simulations; ++i)
    {
        double x = (double)std::rand() / RAND_MAX;
        double y = (double)std::rand() / RAND_MAX;

        bool condition1 = (x + y <= 1.0);
        bool condition2 = (x * y >= 0.09);

        if (condition1 && condition2) {
            success++;
        }
    }
    std::cout << "Simulated Prob: " << (double)success / simulations << std::endl;
    std::cout << "Theoretical Prob: ~0.2023" << std::endl;
}
