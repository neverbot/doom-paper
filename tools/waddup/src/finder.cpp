#include "finder.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <sstream>

namespace fs = std::filesystem;

namespace waddup {

  const unsigned int BUFFER_SIZE = 4096;

  /**
   * @brief Constructor for the Finder class.
   * @param directory The directory to search for duplicate files.
   * @throws std::runtime_error if the directory does not exist or is not a
   * directory.
   */
  Finder::Finder(std::string directory) {
    directory_ = std::move(directory);

    // Now you can add more initialization code here
    if (!fs::exists(directory_)) {
      throw std::runtime_error("Directory does not exist: " + directory_);
    }

    if (!fs::is_directory(directory_)) {
      throw std::runtime_error("Path is not a directory: " + directory_);
    }
  }

  /**
   * @brief Calculate the SHA-256 hash of a file.
   * @param filepath The path to the file.
   * @return The SHA-256 hash of the file as a hexadecimal string.
   */
  std::string Finder::calculateFileHash(const std::string &filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
      std::cerr << "Could not open file: " << filepath << "\n";
      return "";
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
      std::cerr << "Could not create hash context" << "\n";
      return "";
    }

    if (!EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr)) {
      EVP_MD_CTX_free(ctx);
      std::cerr << "Could not initialize hash context" << "\n";
      return "";
    }

    char buffer[BUFFER_SIZE];
    while (file.read(buffer, sizeof(buffer))) {
      if (!EVP_DigestUpdate(ctx, buffer, file.gcount())) {
        EVP_MD_CTX_free(ctx);
        std::cerr << "Could not update hash" << "\n";
        return "";
      }
    }

    // Don't forget to process the last chunk if file size isn't a multiple of
    // buffer size
    if (file.gcount() > 0) {
      if (!EVP_DigestUpdate(ctx, buffer, file.gcount())) {
        EVP_MD_CTX_free(ctx);
        std::cerr << "Could not update hash with final chunk" << "\n";
        return "";
      }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int  hash_len;
    if (!EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
      EVP_MD_CTX_free(ctx);
      std::cerr << "Could not finalize hash" << "\n";
      return "";
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; i++) {
      ss << std::hex << std::setw(2) << std::setfill('0')
         << static_cast<int>(hash[i]);
    }

    return ss.str();
  }

  /**
   * @brief Find duplicate files in the specified directory.
   * @return A vector of strings describing the duplicate files found.
   *         Each string contains the hash and the paths of the duplicate files.
   */
  std::vector<std::string> Finder::findDuplicates() {
    std::vector<std::string>                                  results;
    std::unordered_map<std::size_t, std::vector<std::string>> size_map;

    // First pass: group files by size
    for (const auto &entry : fs::recursive_directory_iterator(directory_)) {
      if (entry.is_regular_file()) {
        auto size = entry.file_size();
        size_map[size].push_back(entry.path().string());
      }
    }

    // Second pass: calculate hashes only for files with same size
    for (const auto &[size, files] : size_map) {
      if (files.size() > 1) {  // Only process potential duplicates
        for (const auto &filepath : files) {
          auto hash = calculateFileHash(filepath);
          if (!hash.empty()) {
            FileInfo info{filepath, size};
            hash_map_[hash].push_back(info);
          }
        }
      }
    }

    // Generate results for files with matching hashes
    for (const auto &[hash, files] : hash_map_) {
      if (files.size() > 1) {
        results.push_back("Found duplicate files with hash " + hash + ":");
        for (const auto &file : files) {
          results.push_back("  " + file.path + " (" +
                            std::to_string(file.size) + " bytes)");

          // Add first few lines of each file for verification
          // std::ifstream f(file.path);
          // std::string   line;
          // int           lineCount = 0;
          // results.push_back("  Contents:");
          // while (std::getline(f, line) && lineCount < 3) {
          //   results.push_back("    " + line);
          //   lineCount++;
          // }
          // results.push_back("");
        }
      }
    }

    return results;
  }

}  // namespace waddup
