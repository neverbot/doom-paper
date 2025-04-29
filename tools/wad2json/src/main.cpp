#include "./wad.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "Usage: wad2json <wad file> <output json file>\n";
    std::cout << "  wad file: Path to the WAD file to convert\n";
    std::cout << "  output json file: Path to the output JSON file\n";
    return 1;
  }

  std::cout << "Converting WAD file to JSON...\n";
  std::string wadFilePath  = argv[1];
  std::string jsonFilePath = argv[2];

  WAD wad(wadFilePath);

  // Process the WAD file and load all data
  wad.processWAD();

  // Convert WAD data to JSON format
  nlohmann::json jsonData = wad.toJSON();

  // print json data
  // std::cout << jsonData.dump(4) << std::endl;

  // print json data in jsonFilePath
  std::ofstream jsonFile(jsonFilePath);
  if (!jsonFile) {
    std::cerr << "Unable to open output JSON file: " << jsonFilePath << "\n";
    return 1;
  }

  // parameter is number of spaces to indent
  jsonFile << jsonData.dump(1);
  jsonFile.close();

  std::cout << "WAD file converted to JSON successfully.\n";

  return 0;
}
