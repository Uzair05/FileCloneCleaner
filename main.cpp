#include <iostream>
#include "walker.hpp"
#include "shasum.hpp"
#include <map>
#include <list>
#include <algorithm>

namespace {
    template <typename T>
    void sorted_insert(std::list<T>& l, const T& e) {
        const auto position = std::lower_bound(l.begin(), l.end(), e);
        l.insert(position, e);
    }
}  // namespace

int main(int argc, char* argv[]) {
    std::map<std::string, std::list<fs::path>> cache{};

    for (const auto& f_: walk(fs::current_path())) {
        const auto res{sha256sum(f_)};
        if (!res.has_value()) continue;
        if (cache.find(res.value()) == cache.end()) cache[res.value()] = std::list<fs::path>{};

        auto& c = cache.at(res.value());
        ::sorted_insert(c, f_);
    }

    for (const auto& mm: cache) {
        if (mm.second.size() < 2) continue;

        std::cout << mm.first << "\n";
        for (const auto& f: mm.second) {
            std::cout << f.string() << "\n";
        }
        std::cout << "\n\n";
    }
    return 0;
}