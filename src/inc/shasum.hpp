#pragma once

#include <string>
#include <filesystem>
#include <optional>
namespace fs = std::filesystem;

std::optional<std::string> sha256sum(const std::string& path);
std::optional<std::string> sha256sum(const fs::path& path);