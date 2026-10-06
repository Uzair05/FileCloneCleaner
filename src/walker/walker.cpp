#include "walker.hpp"

std::vector<fs::path> walk(const fs::path& path) {
    std::vector<fs::path> result{};

    for (const auto& f_ : fs::recursive_directory_iterator(path))
        if (f_.is_regular_file() && !f_.is_symlink()) result.push_back(f_.path());
    return result;
}

std::vector<fs::path> walk(const std::string& path) {
    const fs::path curr(path);
    return walk(is_directory(curr) ? curr : curr.parent_path());
}