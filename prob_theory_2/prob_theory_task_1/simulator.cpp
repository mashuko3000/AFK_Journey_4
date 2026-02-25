#include "simulator.h"

simulator::simulator() : rng_(std::random_device{}())
{
    persons.reserve(5000);
}

simulator::simulator(std::vector<person_t> initial) noexcept
    : persons(std::move(initial)),
    rng_(std::random_device{}())
{
    normalise_neighbours();
}

void simulator::normalise_neighbours()
{
    for(auto& p : persons) // persons is vector which consists of person_t
    {
        auto& nbr = p.neighbours;

        //sort and delete dublicates
        std::sort(nbr.begin(), nbr.end());
        nbr.erase(std::unique(nbr.begin(), nbr.end()), nbr.end());
        nbr.erase(std::remove(nbr.begin(), nbr.end(), p.id), nbr.end());
    }
}

void simulator::load_edge_list(const std::string &filename) {
    std::ifstream file(filename);
    if(!file.is_open()) throw std::runtime_error("Cannot open file : " + filename);
    if (filename.empty()) {
        throw std::invalid_argument("Empty filename provided");
    }

    persons.clear();

    std::uint32_t max_id = 0;
    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        uint32_t u, v;
        if(!(iss >> u >> v)) continue;
        max_id = std::max({max_id, u, v});
    }
    if (max_id == 0 && file.tellg() == 0) throw std::runtime_error("Graph file is empty or invalid format");

    file.clear();
    file.seekg(0, std::ios::beg);

    persons.assign(max_id + 1, person_t{});

    for(uint32_t i = 0; i <= max_id; ++i)
    {
        persons[i].id = i;
    }

    while(std::getline(file, line))
    {
        if(line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        uint32_t u, v;
        if(!(iss >> u >> v)) continue;
        if(u > max_id || v > max_id) continue;

        persons[u].neighbours.push_back(v);
        persons[v].neighbours.push_back(u);
    }

    normalise_neighbours();

    bool has_edges = false;
    for (const auto& p : persons) {
        if (!p.neighbours.empty()) {
            has_edges = true;
            break;
        }
    }
    if (!has_edges) {
        throw std::runtime_error("Graph loaded but contains no edges after normalization");
    }
}

void simulator::set_probabilities(double p_infection, double p_recovery) noexcept
{
    p_infect = std::clamp(p_infection, 0.0, 1.0);
    p_recover = std::clamp(p_recovery, 0.0, 1.0);
}

void simulator::reset_states() {
    for(auto& p : persons)
    {
        p.state = health_t::healthy;
    }
}

void simulator::clear()
{
    persons.clear();
    persons.shrink_to_fit();
}

void simulator::seed_random(int count)
{
    if(empty() || count <= 0) return;

    const std::size_t size_p = size();
    count = std::min(count, static_cast<int>(size_p));

    std::vector<std::size_t> indices(size_p);
    std::iota(indices.begin(), indices.end(), 0);

    for (std::size_t i = 0; i < static_cast<std::size_t>(count); ++i)
    {
        std::uniform_int_distribution<std::size_t> dist(i, size_p - 1);
        std::size_t j = dist(rng_);
        std::swap(indices[i], indices[j]);
    }

    for (std::size_t i = 0; i < static_cast<std::size_t>(count); ++i)
    {
        persons[indices[i]].state = health_t::infected;
    }
}

std::size_t simulator::count_susceptible() const
{
    std::size_t count = 0;
    for(const auto& p : persons)
    {
        if(p.state == health_t::healthy) count++;
    }
    return count;
}

std::size_t simulator::count_infected() const
{
    std::size_t count = 0;
    for(const auto& p : persons)
    {
        if(p.state == health_t::infected) count++;
    }
    return count;
}

std::size_t simulator::count_recovered() const
{
    std::size_t count = 0;
    for(const auto& p : persons)
    {
        if(p.state == health_t::recovered) count++;
    }
    return count;
}

std::vector<std::uint32_t> simulator::get_all_susceptible() const
{
    std::vector<std::uint32_t> result;
    for(const auto& p : persons)
    {
        if(p.state == health_t::healthy) result.push_back(p.id);
    }
    return result;
}

std::vector<std::uint32_t> simulator::get_all_recovered() const
{
    std::vector<std::uint32_t> result;
    for(const auto& p : persons)
    {
        if(p.state == health_t::recovered) result.push_back(p.id);
    }
    return result;
}

std::vector<std::uint32_t> simulator::get_recovered_with_vulnerable_neighbours() const
{
    std::vector<std::uint32_t> result;
    for(const auto& p : persons)
    {
        if(p.state == health_t::recovered)
        {
            for (auto neighbor_id : p.neighbours)
            {
                if(persons[neighbor_id].state != health_t::recovered)
                {
                    result.push_back(p.id);
                    break;
                }
            }
        }
    }
    return result;
}

std::vector<std::uint32_t> simulator::get_susceptible_completely_surrounded() const
{
    std::vector<std::uint32_t> result;
    for(const auto& p : persons)
    {
        if(p.state == health_t::healthy && !p.neighbours.empty())
        {
            bool all_infected = true;
            for (auto neighbor_id : p.neighbours)
            {
                if(persons[neighbor_id].state != health_t::infected)
                {
                    all_infected = false;
                    break;
                }
            }
            if(all_infected) result.push_back(p.id);
        }
    }
    return result;
}

void simulator::step()
{
    if(empty()) return;

    std::vector<bool> will_infect(size(), false);
    std::vector<bool> will_recover(size(), false);

    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for(const auto& p : persons)
    {
        if (p.state == health_t::infected) {
            if (dist(rng_) < p_recover) {
                will_recover[p.id] = true;
            }

            for (auto nbr_id: p.neighbours) {
                const auto &nbr = persons[nbr_id];
                if (nbr.state == health_t::healthy && dist(rng_) < p_infect) {
                    will_infect[nbr_id] = true;
                }
            }
        }
    }

    for (std::size_t i = 0; i < size(); ++i) {
        if (will_recover[i]) {
            persons[i].state = health_t::recovered;
        } else if (will_infect[i]) {
            persons[i].state = health_t::infected;
        }
    }
}
