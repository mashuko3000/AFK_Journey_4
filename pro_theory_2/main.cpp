#include <iostream>
#include"gmurman/gmurman.hpp"
#include<vector>
#include <algorithm>

void task_group_info(const int64_t trials);
void task_cards_no_return(const int trials, int deck_size);
void task_cards_with_return(const int trials, int deck_size);

int main() {
    std::srand(time(0));
    task_57(1000000);
    task_58(1000000);
    task_59(1000000);
    task_61(1000000);
    task_62(1000000);
    task_63(1000000);
    //task_65(1000000000);
    task_67(1000000);
    task_68(1000000);
    task_69(1000000);
    task_70(1000000);
    task_group_info(1000000);
    task_cards_no_return(1000000, 36);
    task_cards_with_return(1000000, 36);
    //task_cards_no_return(1000000, 52);
    //task_cards_with_return(1000000, 52);
    return 0;
}

double get_rand_double()
{
    return (double)std::rand() / RAND_MAX;
}

void task_group_info(const int64_t trials)
{
    const double p_starosta = 0.3;
    const double p_student = 0.5;
    for (int n = 1; n <= 4; ++n)
    {
        int informed_count = 0;
        for (int i = 0; i < trials; ++i)
        {
            bool informed = false;
            if (get_rand_double() < p_starosta) informed = true;
            if (!informed)
            {
                for (int s = 0; s < n; ++s)
                {
                    if (get_rand_double() < p_student)
                    {
                        informed = true;
                        break;
                    }
                }
            }
            if (informed) informed_count++;
        }
        std::cout << "With n = " << n << " students: P = " << (double) informed_count / trials << "\n";
    }
}

void task_cards_no_return(const int trials, int deck_size)
{
    int num_values = (deck_size == 36) ? 9 : 13;
    int c31 = 0, c22 = 0, c_diff = 0, c4 = 0;

    for (int i = 0; i < trials; ++i)
    {
        std::vector<int> deck;
        for (int v = 0; v < num_values; ++v)
        {
            for (int m = 0; m < 4; ++m) deck.push_back(v);
        }
        for (int j = deck.size() - 1; j > 0; --j)
        {
            int k = std::rand() % (j + 1);
            std::swap(deck[j], deck[k]);
        }
        std::vector<int> hand = {deck[0], deck[1], deck[2], deck[3]};
        std::sort(hand.begin(), hand.end());

        if (hand[0] == hand[3]) c4++;
        else if ((hand[0] == hand[2] && hand[2] != hand[3]) || (hand[1] == hand[3] && hand[1] != hand[0])) c31++;
        else if (hand[0] == hand[1] && hand[2] == hand[3]) c22++;
        else if (hand[0] != hand[1] && hand[1] != hand[2] && hand[2] != hand[3]) c_diff++;
    }

    std::cout << "\n--- Mode: NO RETURN | Deck: " << deck_size << " ---\n";
    std::cout << "1) 3+1 (Three of a kind): " << (double)c31 / trials << "\n";
    std::cout << "2) 2+2 (Two pairs): " << (double)c22 / trials << "\n";
    std::cout << "3) All different: " << (double)c_diff / trials << "\n";
    std::cout << "4) 4 same: " << (double)c4 / trials << "\n";
}

void task_cards_with_return(const int trials, int deck_size)
{
    int num_values = (deck_size == 36) ? 9 : 13;
    int c31 = 0, c22 = 0, c_diff = 0, c4 = 0;

    for (int i = 0; i < trials; ++i)
    {
        std::vector<int> hand;

        for (int j = 0; j < 4; ++j)
        {
            hand.push_back(std::rand() % num_values);
        }
        std::sort(hand.begin(), hand.end());

        if (hand[0] == hand[3]) c4++;
        else if ((hand[0] == hand[2] && hand[2] != hand[3]) || (hand[1] == hand[3] && hand[1] != hand[0])) c31++;
        else if (hand[0] == hand[1] && hand[2] == hand[3]) c22++;
        else if (hand[0] != hand[1] && hand[1] != hand[2] && hand[2] != hand[3]) c_diff++;
    }

    std::cout << "\n--- Mode: WITH RETURN | Deck: " << deck_size << " ---\n";
    std::cout << "1) 3+1 (Three of a kind): " << (double)c31 / trials << "\n";
    std::cout << "2) 2+2 (Two pairs): " << (double)c22 / trials << "\n";
    std::cout << "3) All different: " << (double)c_diff / trials << "\n";
    std::cout << "4) 4 same: " << (double)c4 / trials << "\n";
}