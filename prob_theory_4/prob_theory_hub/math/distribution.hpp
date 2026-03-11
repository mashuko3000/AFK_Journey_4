//
// Created by mvideo on 07/03/26.
//

#ifndef PROB_THEORY_HUB_DISTRIBUTION_HPP
#define PROB_THEORY_HUB_DISTRIBUTION_HPP

#include"idistribution.hpp"
#include<stdexcept>
#include<vector>
#include<cmath>

class uniform_distribution_model : public i_distribution_model
{
private:
    size_t m_branches;
public:
    explicit uniform_distribution_model(size_t m)
    {
        if(m <= 0)
        {
            throw std::invalid_argument("Count of branches should be more than 2");
        }
        m_branches = m;
    }

    size_t get_next_index() override
    {
        if(m_branches == 0)
        {
            throw std::invalid_argument("Count of branches should be more than 0");
        }

        const unsigned threshold = RAND_MAX - (RAND_MAX % m_branches);

        unsigned r;
        do
        {
            r = std::rand();
        } while(r > threshold);

        return r % m_branches;
    }

    i_distribution_model* clone() const override
    {
        return new uniform_distribution_model(m_branches);
    }
};

class discrete_distribution_model : public i_distribution_model
{
private:
    std::vector<double> cumulative_prob;
public:
    // weights is  for each element of alphabet
    explicit discrete_distribution_model(const std::vector<double>& weights)
    {
        if(weights.empty())
        {
            throw std::invalid_argument("Weights cannot be empty");
        }
        double sum = 0;
        for(double w : weights)
        {
            if (w < 0) throw std::invalid_argument("Negative weight not allowed");
            sum += w;
        }

        if (sum <= 0.0) throw std::invalid_argument("Total weight must be positive");

        cumulative_prob.reserve(weights.size());
        double cur_sum = 0.0;
        for(double w : weights)
        {
            cur_sum += w/sum;
            cumulative_prob.push_back(cur_sum);
        }

        if (!cumulative_prob.empty())
        {
            cumulative_prob.back() = 1.0;
        }
    }

    size_t get_next_index() override
    {
        double r = static_cast<double>(std::rand()) / (RAND_MAX);
        size_t size = cumulative_prob.size();
        auto it = std::lower_bound(cumulative_prob.begin(), cumulative_prob.end(), r);
        return static_cast<size_t>(it - cumulative_prob.begin());
    }

    i_distribution_model* clone() const override { return new discrete_distribution_model(*this); }
};

class geometric_distribution : public i_distribution_model
{
private:
    double p_success;
    double log_q;
    size_t m_branches;
public:
    explicit geometric_distribution(double p, size_t m)
            : p_success(p), m_branches(m)
    {
        if (p <= 0.0 || p >= 1.0) throw std::invalid_argument("p must be in (0, 1)");
        if (m == 0) throw std::invalid_argument("m must be > 0");
        log_q = std::log(1.0 - p);
    }
    virtual size_t get_next_index()
    {
        if(p_success >= 1.0) return 0;
        double u = static_cast<double>(std::rand()) / (RAND_MAX + 1.0); // safe, never get 1
        double x = std::ceil(std::log(1.0 - u) / log_q); // how muh u need to multiply (1 - p) to go from 1 to (1-u)
        size_t raw = (x < 1.0) ? 1 : static_cast<size_t>(x); // number of branch 1.....m
        if(raw > m_branches)
        {
            return m_branches - 1;
        }
        return raw - 1;
    }
    virtual i_distribution_model* clone() const {
        return new geometric_distribution(*this);
    }
};

class binomial_distribution : public i_distribution_model
{
private:
    size_t m_branches;
    double p_success;
    std::vector<double> cumulative_prob;

    double combinations (size_t n, size_t k) const
    {
        if (k > n - k) return k = n - k;
        if (k == 0 || k == n) return 1;
        if (k > n / 2) k = n - k;

        double res = 1.0;
        for (size_t i = 1; i <= k; ++i) {
            res = res * (n - i + 1) / i;
        }
        return res;
    }

public:
    explicit binomial_distribution(double p, size_t m) : m_branches(m > 0 ? m - 1 : 0), p_success(p)
    {
        if(m_branches < 2) throw std::invalid_argument("count of branches should be more than 2");
        if(m_branches > 30000) throw std::invalid_argument("count of branches too big, choose smth that will be less than 30_000");
        if (p < 0.0 || p > 1.0) throw std::invalid_argument("p must be [0, 1]");
        cumulative_prob.reserve(m_branches);

        size_t n = m_branches;

        double sum = 0.0;
        for(int i = 0; i <= n; ++i)
        {
            double prob = combinations(n, i) * pow(p, i) * pow(1 - p, n - i);
            sum += prob;
            cumulative_prob.push_back(sum);
        }
        if (!cumulative_prob.empty()) cumulative_prob.back() = 1.0;
    }
    size_t get_next_index()
    {
        double r = static_cast<double>(std::rand()) / (RAND_MAX + 1.0);
        auto it = std::lower_bound(cumulative_prob.begin(), cumulative_prob.end(), r);
        size_t idx = std::distance(cumulative_prob.begin(), it);
        return (idx >= m_branches) ? m_branches - 1 : idx;
    }
    i_distribution_model* clone() const
    {
        return new binomial_distribution(*this);
    }
};

class bernoulli_distribution : public i_distribution_model
{
private:
    double m_p;
    size_t m_branches;
public:
    explicit bernoulli_distribution(double p, size_t m) : m_p(p), m_branches(m)
    {
        if (p < 0.0 || p > 1.0) throw std::invalid_argument("p must be in [0, 1]");
        if (m == 0) throw std::invalid_argument("m must be > 0");
    }

    size_t get_next_index() override
    {
        if (m_branches == 1) return 0;

        double r = static_cast<double>(std::rand()) / (RAND_MAX);
        int bernoulli_result = (r < m_p) ? 1 : 0;

        size_t mid = m_branches / 2;
        if(bernoulli_result == 0)
        {
            return std::rand() % mid;
        }
        else
        {
            return mid + (std::rand() % (m_branches - mid));
        }
    }

    i_distribution_model* clone() const override
    {
        return new bernoulli_distribution(*this);
    }
};

class triangular : public i_distribution_model
{
private:
    std::vector<double> cumulative_prob;
    int m_size;

public:
    explicit triangular(size_t m) : m_size(m)
    {
        std::vector<double> weights;
        int mid = m_size / 2;
        double sum = 0;

        for(int i = 0; i < m_size; ++i)
        {
            double w = mid - std::abs(i - mid) + 1;
            weights.push_back(w);
            sum += w;
        }

        double cur_sum = 0;
        for(double w : weights)
        {
            cur_sum += w / sum;
            cumulative_prob.push_back(cur_sum);
        }
        cumulative_prob.push_back(1.0);
    }

    size_t get_next_index() override
    {
        double r = static_cast<double>(std::rand()) / RAND_MAX;
        auto it = std::lower_bound(cumulative_prob.begin(), cumulative_prob.end(), r);
        return std::distance(cumulative_prob.begin(), it);
    }

    i_distribution_model* clone()const override
    {
        return new triangular(*this);
    }
};

#endif //PROB_THEORY_HUB_DISTRIBUTION_HPP
