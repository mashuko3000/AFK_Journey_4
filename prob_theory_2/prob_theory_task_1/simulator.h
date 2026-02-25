#ifndef SIMULATOR_H
#define SIMULATOR_H

#include<cstdint>
#include<vector>
#include<string>
#include<random>
#include<algorithm>
#include<fstream>
#include <utility>
#include<sstream>
enum class health_t
{
    healthy = 0,
    infected = 1,
    recovered = 2
};

struct person_t
{
    std::uint32_t id = 0;
    health_t state = health_t::healthy;
    std::vector<uint32_t>neighbours;

    person_t() = default;
    explicit person_t(std::uint32_t i) : id(i) {}
};

class simulator
{
private:
    std::vector<person_t> persons;

    double p_infect{0.12};
    double p_recover{0.05};

    mutable std::mt19937 rng_;
    void normalise_neighbours();
public:
    std::size_t count_susceptible() const;
    std::size_t count_infected() const;
    std::size_t count_recovered() const;

public:
    std::vector<std::uint32_t> get_all_susceptible() const;
    std::vector<std::uint32_t> get_all_recovered() const;

    std::vector<std::uint32_t> get_recovered_with_vulnerable_neighbours() const;
    std::vector<std::uint32_t> get_susceptible_completely_surrounded() const;

public:
    const std::vector<person_t>& nodes() const& noexcept {return persons; }
    std::vector<person_t>& nodes() & noexcept {return persons; }

    bool empty() const noexcept {return persons.empty(); }
    std::size_t size() const noexcept {return persons.size(); }

    double infection_probability() const noexcept {return p_infect;}
    double recovery_probability() const noexcept {return p_recover;}
public:
    simulator();
    ~simulator() = default;

    simulator(simulator&) noexcept = default;
    simulator& operator=(const simulator&) = default;

    simulator(simulator&&) noexcept = default;
    simulator& operator=(simulator&&) noexcept = default;

    explicit simulator(std::vector<person_t> initial) noexcept;

public:
    void load_edge_list(const std::string& filename);
    void set_probabilities(double p_infection, double p_recovery) noexcept;
    void seed_random(int count = 1);
    void step();
    void reset_states();
    void clear();
    void seed_rng(unsigned  int seed) {rng_.seed(seed);}
};



#endif // SIMULATOR_H
