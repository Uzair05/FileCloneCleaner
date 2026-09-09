#include <iostream>
#include "walker.hpp"
#include "shasum.hpp"
#include <map>
#include <list>
#include <algorithm>
#include <stdexcept>

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

    std::error_code ecr{}, ecs{};
    for (const auto& mm: cache) {
        if (mm.second.size() < 2) continue;
        const auto source = mm.second.begin();

        for (auto fi{std::next(source)}; fi != mm.second.end(); ++fi) {
            ecr.clear();
            ecs.clear();

            if (fs::remove(*fi, ecr)) {
                fs::create_symlink(*source, *fi, ecs);
                if (ecs) {
                    std::cerr << "Could not create symlink "
                        << source->string() << ": "
                        << fi->string() << "\n"
                        << ecs.message() << "\n";
                    throw std::runtime_error(ecs.message());
                }
            }else if (ecr) {
                std::cerr << "Could not delete file "
                    << source->string() << ": "
                    << fi->string() << "\n"
                    << ecr.message() << "\n";
                throw std::runtime_error(ecr.message());
            }
        }
    }
    return 0;
}