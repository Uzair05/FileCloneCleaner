#include "shasum.hpp"
#include <fstream>
#include <iostream>
#include <array>
#include <openssl/evp.h>

std::optional<std::string> sha256sum(const std::string& path) { return sha256sum(fs::path(path)); }
std::optional<std::string> sha256sum(const fs::path& path) {
    if (!fs::is_regular_file(path)) return std::nullopt;

    std::ifstream file(path.string(), std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file: " << path.string() << std::endl;
        return std::nullopt;
    }

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) {
        std::cerr << "Could not create EVP_MD_CTX" << std::endl;
        return std::nullopt;
    }

    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(context);
    }

    std::array<char, 8192> buffer{};
    while (file) {
        file.read(buffer.data(), buffer.size());
        std::streamsize filesize = file.gcount();
        if (filesize > 0 &&
            EVP_DigestUpdate(context, buffer.data(), static_cast<std::size_t>(filesize)) != 1) {
            EVP_MD_CTX_free(context);
            std::cerr << "Could not update EVP_MD_CTX" << std::endl;
            return std::nullopt;
        }
    }

    if (file.bad()) {
        EVP_MD_CTX_free(context);
        std::cerr << "Could not read from file: " << path.string() << std::endl;
    }

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_size = 0;

    if (EVP_DigestFinal_ex(context, digest, &digest_size) != 1) {
        EVP_MD_CTX_free(context);
        std::cerr << "Could not finalize EVP_MD_CTX" << std::endl;
        return std::nullopt;
    }

    EVP_MD_CTX_free(context);
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < digest_size; i++) {
        oss << std::setw(2) << static_cast<unsigned>(digest[i]);
    }

    return oss.str();
}