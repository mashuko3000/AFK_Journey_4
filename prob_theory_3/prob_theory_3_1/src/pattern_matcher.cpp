//
// Created by mvideo on 28/02/26.
//

#include "pattern_matcher.hpp"

match_result pattern_matcher::are_connectable(const cluster& left,
                            const cluster& right,
                            const string& pattern)
{
    match_result result;
    const size_t k = pattern.length();
    if (pattern.empty() || k < 2) return result;

    const size_t nL = left.size();
    const size_t nR = right.size();

    for(size_t left_len = 1; left_len < k; ++left_len)
    {
        size_t right_len = k - left_len;
        if (left_len > nL || right_len > nR)
        {
            continue;
        }

        std::string suffix = left.get_suffix(left_len);
        std::string prefix = right.get_prefix(right_len);

        if (suffix + prefix == pattern)
        {
            result.connected = true;
            result.left_match = suffix;
            result.right_match = prefix;
            string left_full = left.content();
            string right_full = right.content();
            result.left_rem = left_full.substr(0, left_full.size() - left_len);
            result.right_rem = right_full.substr(right_len);
            return result;
        }
    }
    return result;
}
