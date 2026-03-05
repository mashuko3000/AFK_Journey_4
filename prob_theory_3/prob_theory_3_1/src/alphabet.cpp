//
// Created by mvideo on 28/02/26.
//
#include "alphabet.hpp"
#include <stdexcept>

alphabet::alphabet(const string& characters, i_distribution_model* model)
        : symbols(characters), distribution(model)
{
    if (symbols.empty())
    {
        throw std::invalid_argument("ERROR : alphabet cannot be empty");
    }
    if (!distribution)
    {
        throw std::invalid_argument("ERROR : distribution model cannot be nullptr");
    }
}

alphabet::~alphabet()
{
    delete distribution;
    distribution = nullptr;
}

alphabet::alphabet(const alphabet& source)
        : symbols(source.symbols),
          distribution(source.distribution ? source.distribution->clone() : nullptr)
{
    if (!distribution && !source.symbols.empty())
    {
        throw std::runtime_error("Failed to clone distribution model");
    }
}
alphabet& alphabet::operator=(const alphabet& source)
{
    if (this == &source)
    {
        return *this;
    }
    alphabet temp(source);
    swap(*this, temp);
    return *this;
}

alphabet::alphabet(alphabet&& source) noexcept
        : symbols(std::move(source.symbols)),
          distribution(source.distribution)
{
    source.distribution = nullptr;
}
alphabet& alphabet::operator=(alphabet&& source) noexcept
{
    if (this == &source)
    {
        return *this;
    }

    delete distribution;

    symbols = std::move(source.symbols);
    distribution = source.distribution;

    source.distribution = nullptr;

    return *this;
}

char alphabet::generate_symbol()
{
    if(!distribution)
    {
        throw std::runtime_error("Distribution model is null");
    }

    size_t idx = distribution->get_next_index();
    if(idx >= symbols.size())
    {
        throw std::out_of_range("Generated index out of alphabet range");
    }
    return symbols[idx];
}
bool alphabet::contains(char s) const
{
    return symbols.find(s) != std::string::npos;
}

void swap(alphabet& first, alphabet& second) noexcept
{
    using std::swap;
    swap(first.symbols,second.symbols);
    swap(first.distribution,second.distribution);
}