#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

int main() {
    std::string levelPath = "resource/levels";
    std::vector<std::string> levels;

    //Collect all of the .data files in the levels directory
    for (const auto& entry : fs::directory_iterator(levelPath)) {
        if (entry.path().extension() == ".data") {
            levels.push_back(entry.path().filename().string());
        }
    }

    if (levels.empty()) {
        std::cout << "No levels found in " << levelPath << std::endl;
        return 1;
    }

    // Display all available levels
    std::cout << "Available Levels:" << std::endl;
    for (size_t i = 0; i < levels.size(); ++i) {
        std::cout << i + 1 << ". " << levels[i] << std::endl;
    }

    // Prompt user to select a level
    int choice = 0;
    std::cout << "\nEnter number: ";
    std::cin >> choice;

    // Validate user input
    if (choice < 1 || choice > (int)levels.size()) {
        std::cout << "Invalid choice." << std::endl;
        return 1;
    }

    // Launch the selected level
    std::string selectedLevel = levels[choice - 1];
    std::string command = "./bin/Debug/rayboing " + levelPath + "/" + selectedLevel;
    std::system(command.c_str());

    return 0;
}