//
// Created by mvideo on 28/02/26.
//

#include "cluster.hpp"
#include<stdexcept>

cluster::cluster(size_t size) : n(size), data()
{
    if (size == 0)
    {
        throw std::invalid_argument("cluster size must be greater than zero");
    }
    data.reserve(n);
}

void cluster::fill(alphabet& alpha)
{
    if(alpha.size() == 0)
    {
        throw std::invalid_argument("cannot fill cluster with empty alphabet");
    }

    data.clear();
    data.reserve(n);

    for (std::size_t i = 0; i < n; ++i)
    {
        data.push_back(alpha.generate_symbol());
    }

    if(data.size() != n)
    {
        throw std::runtime_error("cluster fill produced string of wrong length");
    }
}
string cluster::get_content() const
{
    return data;
}
string cluster::get_suffix(size_t k) const
{
    if(k == 0)
    {
        return "";
    }
    std::size_t len = std::min(k, n);
    return data.substr(n - len);
}
string cluster::get_prefix(size_t k) const
{
    if(k == 0)
    {
        return "";
    }
    std::size_t len = std::min(k, n);
    return data.substr(0, len);
}

const string& cluster::content() const
{
    return data;
}
