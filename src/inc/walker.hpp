#pragma once

#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;
std::vector<fs::path> walk(const fs::path& path);
std::vector<fs::path> walk(const std::string& path);