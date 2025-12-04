#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <regex>

namespace fs = std::filesystem;

const std::string PROJECT_ROOT = "/home/justice/projects/preboing/";
const std::string LEVEL_DIR = "resource/levels";
const std::string RAYBOING_BINARY = "bin/Debug/rayboing";

int extractLevelNumber(const std::string& filename) {
    std::regex re("level(\\d+)\\.data");
    std::smatch match;
    if (std::regex_match(filename, match, re)) {
        return std::stoi(match[1]);
    }
    return -1; // Non-level files get -1
}

int main() {
    std::vector<std::string> levels;
    std::string fullLevelPath = PROJECT_ROOT + LEVEL_DIR;

    try {
        for (const auto& entry : fs::directory_iterator(fullLevelPath)) {
            if (entry.path().extension() == ".data") {
                std::string name = entry.path().filename().string();
                if (extractLevelNumber(name) != -1) { // Only include levelXX.data
                    levels.push_back(name);
                }
            }
        }

        // Sort numerically
        std::sort(levels.begin(), levels.end(), [](const std::string &a, const std::string &b) {
            return extractLevelNumber(a) < extractLevelNumber(b);
        });

    } catch (fs::filesystem_error& e) {
        std::cerr << "Error accessing levels directory: " << e.what() << std::endl;
        return 1;
    }

    if (levels.empty()) {
        std::cerr << "No levels found in " << fullLevelPath << std::endl;
        return 1;
    }

    size_t currentLevel = 0;

    while (true) {
        std::cout << "\nCurrent Level: " << levels[currentLevel] << "\n";
        std::cout << "Options:\n";
        std::cout << "n - Next level\np - Previous level\nm - Show menu\nq - Quit\nl - Launch this level\nEnter option: ";

        std::string input;
        std::cin >> input;

        if (input == "q") {
            break;
        } else if (input == "n") {
            currentLevel = (currentLevel + 1) % levels.size();
        } else if (input == "p") {
            currentLevel = (currentLevel + levels.size() - 1) % levels.size();
        } else if (input == "m") {
            std::cout << "\nAvailable Levels:\n";
            for (size_t i = 0; i < levels.size(); ++i) {
                std::cout << i + 1 << ". " << levels[i] << std::endl;
            }
        } else if (input == "l") {
            std::string command = PROJECT_ROOT + RAYBOING_BINARY + " " + fullLevelPath + "/" + levels[currentLevel];
            std::cout << "Launching " << levels[currentLevel] << "...\n";
            std::system(command.c_str());
            std::cout << "\nLevel finished. Returning to launcher...\n";
        } else {
            // Numeric jump
            try {
                int num = std::stoi(input);
                if (num >= 1 && num <= (int)levels.size()) {
                    currentLevel = num - 1;
                } else {
                    std::cout << "Invalid level number.\n";
                }
            } catch (...) {
                std::cout << "Invalid option. Try again.\n";
            }
        }
    }

    return 0;
}
