#include <iostream>
#include <filesystem>
#include <string>
#include <iomanip>
#include <map>

namespace fs = std::filesystem;


// Function to renumber the image sequence in the given directory
void renumberImageSequence(const std::string& directoryPath) {
    fs::path directory(directoryPath);

    if (!fs::is_directory(directory)) {
        std::cout << "Error: Invalid directory path." << std::endl;
        return;
    }

    std::map<std::string, int> sequenceCounters;

    // Iterate over each file in the directory
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            const std::string& fileName = entry.path().filename().string();
            const std::string& extension = entry.path().extension().string();

            size_t underscorePos = fileName.find('_');
            size_t dotPos = fileName.find('.');

            // Check if the file name has the expected format with an underscore and a dot
            if (underscorePos != std::string::npos && dotPos != std::string::npos) {
                std::string name = fileName.substr(0, underscorePos);
                std::string sequenceStr = fileName.substr(underscorePos + 1, dotPos - underscorePos - 1);

                // Check if sequenceStr is a valid number
                try {
                    // Get the sequence counter for the current file extension
                    int& sequenceCounter = sequenceCounters[name + extension];

                    // Generate the new file name with sequential numbering
                    std::string paddedSequenceStr = std::to_string(sequenceCounter + 1);
                    while (paddedSequenceStr.length() < 2) {
                        paddedSequenceStr = "0" + paddedSequenceStr;
                    }
                    std::string newFileName = name + "_" + paddedSequenceStr + extension;

                    // Rename the file by replacing it with the new file name
                    fs::rename(entry.path(), entry.path().parent_path() / newFileName);

                    std::cout << "Renamed: " << fileName << " -> " << newFileName << std::endl;

                    // Increment the sequence counter for the current file extension
                    sequenceCounter++;
                } catch (const std::invalid_argument& e) {
                    std::cout << "Skipping file: " << fileName << " (Invalid sequence number)" << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cout << "Skipping file: " << fileName << " (Sequence number out of range)" << std::endl;
                }
            }
        }
    }
}


int main(int argc, char* argv[]) {
    // Get the program's path and directory
    std::string programPath = argv[0];
    std::string programDirectory = fs::path(programPath).parent_path().string();
    std::string programFolder = fs::path(programPath).filename().string();
    
    // Display menu options
    std::cout << "Select an option:\n";
    std::cout << "1. Enter a directory path\n";
    std::cout << "2. Use current directory (" << programFolder << ")\n";
    std::cout << "3. Exit the program\n";
    std::cout << "Enter option number: ";

    int option;
    std::cin >> option;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string directoryPath;

    switch (option) {
        case 1: {
            std::cout << "Enter directory path: ";
            std::getline(std::cin, directoryPath);
            break;
        }
        case 2: {
            directoryPath = programDirectory;
            break;
        }
        case 3:{
            exit(1);
        }
        default: {
            std::cout << "Invalid option. Exiting the program.\n";
            return 0;
        }
    }
    // Get the folder path from the directory path
    std::string folderPath = fs::path(directoryPath).filename().string();
   
    // Display selected directory and folder path
    std::cout << "Running the program for directory: " << directoryPath << '\n';
    std::cout << "Folder path: " << folderPath << '\n';
   
    // Call the function to renumber image sequence
    renumberImageSequence(directoryPath);

    return 0;
    // Run the program until the user chooses to exit
    // while (true) {
    //     std::cout << "Enter the directory path (or 'exit' to quit): ";
    //     std::getline(std::cin, directoryPath);

    //     // Check if the user wants to exit
    //     if (directoryPath == "exit") {
    //         break;
    //     }

    //     // Call the renumbering function with the provided directory path
    //     renumberImageSequence(directoryPath);
    // }

    // return 0;
}
