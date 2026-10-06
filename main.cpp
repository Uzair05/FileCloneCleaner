#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <stdexcept>

#include <CLI/CLI.hpp>
#include "walker.hpp"
#include "shasum.hpp"

namespace {
    template <typename T>
    void sorted_insert(std::list<T>& l, const T& e) {
        const auto position = std::lower_bound(l.begin(), l.end(), e);
        l.insert(position, e);
    }

    size_t directory_depth(fs::path const& path) { return std::distance(path.begin(), path.end()); }
    void path_sort(std::list<fs::path>& l) {
        l.sort([](const fs::path& a, const fs::path& b) {
            const size_t ca{directory_depth(a)}, cb{directory_depth(b)};
            if (ca != cb) return ca < cb;
            return a < b;
        });
    }

}  // namespace

int main(int argc, char* argv[]) {
    CLI::App app;
    std::string current_path{};
    bool dry_run{false};
    app.add_option("--path", current_path, "Operating Directory")
        ->default_val(fs::current_path().string());
    app.add_flag("--dry-run", dry_run, "Dry Run Command")->default_val(false);

    // populate directory
    CLI11_PARSE(app, argc, argv);
    std::map<std::string, std::list<fs::path>> cache{};
    for (const auto& f_ : walk(fs::path(current_path))) {
        const auto res{sha256sum(f_)};
        if (!res.has_value()) continue;
        if (!cache.contains(res.value())) cache[res.value()] = std::list<fs::path>{};

        auto& c = cache.at(res.value());
        ::sorted_insert(c, f_);
    }
    // ensure paths are sorted by depth
    for (auto& [hash_, path_lists] : cache) {
        if (path_lists.size() > 1) {
            ::path_sort(path_lists);
        }
    }

    std::error_code ecr{}, ecs{};
    for (const auto& cache_item : cache) {
        if (cache_item.second.size() < 2) continue;
        const auto source = cache_item.second.begin();

        for (auto fi{std::next(source)}; fi != cache_item.second.end(); ++fi) {
            ecr.clear();
            ecs.clear();
            if (!dry_run) {
                if (fs::remove(*fi, ecr)) {
                    fs::create_symlink(fs::relative(*source, fi->parent_path()), *fi, ecs);
                    if (ecs) {
                        std::cerr << "Could not create symlink " << source->string() << ": "
                                  << fi->string() << "\n"
                                  << ecs.message() << "\n";
                        throw std::runtime_error(ecs.message());
                    }
                } else if (ecr) {
                    std::cerr << "Could not delete file " << source->string() << ": "
                              << fi->string() << "\n"
                              << ecr.message() << "\n";
                    throw std::runtime_error(ecr.message());
                }
            } else {
                std::cout << fi->string() << "\t-->\t" << source->string() << "\n";
            }
        }
    }
    return 0;
}
