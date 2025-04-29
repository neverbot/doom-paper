#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace waddup {
  class Finder {
  public:
    explicit Finder(std::string directory);
    std::vector<std::string> findDuplicates();

  private:
    struct FileInfo {
      std::string path;
      std::size_t size;
    };

    std::string directory_;
    std::unordered_map<std::string, std::vector<FileInfo>>
        hash_map_;  // hash -> vector of files with that hash

    static std::string calculateFileHash(const std::string &filepath);
  };
}  // namespace waddup
