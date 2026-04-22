//
// Created by maria on 22.04.2026.
//

#ifndef HW_10_STAT_HPP
#define HW_10_STAT_HPP

struct Stat
{
    long long count  = 0;
    double mean = 0.0;
    double m2 = 0.0;

    void add(double x)
    {
        count++;
        double delta = x - mean;
        mean += delta/count;
        double delta2 = x - mean;
        m2 += delta * delta2;
    }

    double getMean() const {return mean;}
    double getVariance() const
    {
        if(count < 2) return 0.0;
        return m2/(count -1);
    }
};

#endif //HW_10_STAT_HPP
