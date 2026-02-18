#include "1_task.hpp"

void task_1(const int N, const int64_t simulation, bool shifted, stat& emp, stat& theo)
{
    std::srand(std::time(0));

    if (N < 2) throw std::invalid_argument("N must be more than s_task T_T");
    if (simulation < 1) throw std::invalid_argument("count of experiments must be more than 0 T_T");

    int64_t cnt1 = 0, cnt2 = 0, cnt_both = 0;
    for(int64_t i = 0; i < simulation; ++i)
    {
        const int pos1 = rand() % N;
        const int num1 = pos1 + 1;
        const bool even1 = !(num1 & 1);

        int pos2 = rand() % (N - 1);

        bool even2;
        if(shifted)
        {
            const int new_num = pos2 + 1;
            even2 = !(new_num & 1);
        }
        else
        {
            const int original_number = (pos2 < pos1) ? (pos2 + 1) : (pos2 + 2);
            even2 = !(original_number & 1);
        }

        if (even1) ++cnt1;
        if (even2) ++cnt2;
        if (even1 && even2) ++cnt_both;
    }

    double denom = static_cast<double>(simulation);

    emp.first = static_cast<double>(cnt1) / denom;
    emp.second = static_cast<double>(cnt2) / denom;
    emp.both = static_cast<double>(cnt_both) / denom;

    double E = N / 2.0;
    theo.first = E / N;

    if (shifted)
    {
        int even_positions_after_remove = (N - 1) / 2;
        theo.second = static_cast<double>(even_positions_after_remove) / (N - 1);
        theo.both   = theo.first * theo.second;
    }
    else
    {
        double p_second = (E / N) * ((E - 1) / (N - 1.0)) + ((N - E) / N) * (E / (N - 1.0));
        theo.second = p_second;

        double p_both = E * (E - 1) / (N * (N - 1.0));
        theo.both = p_both;
    }
}
