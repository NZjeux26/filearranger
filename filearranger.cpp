#include <iostream>
#include <filesystem>
#include <string>
#include <iomanip>
#include <map>

namespace fs = std::filesystem;


// Function to renumber the image sequence in the given directory
void renumberImageSequence(const std::string& directoryPath) {
    // Convert the directory path to std::filesystem path
    fs::path directory(directoryPath);

    // Check if the provided directory path is valid
    if (!fs::is_directory(directory)) {
        std::cout << "Error: Invalid directory path." << std::endl;
        return;
    }

    // Map to store the sequence counters for each file extension
    std::map<std::string, int> sequenceCounters;

    // Iterate over each entry (file or directory) in the given directory
    for (const auto& entry : fs::directory_iterator(directory)) {
        // Check if the entry is a regular file
        if (entry.is_regular_file()) {
            // Extract the file name and extension from the entry
            const std::string& fileName = entry.path().filename().string();
            const std::string& extension = entry.path().extension().string();

            // Find the positions of underscore and dot in the file name
            size_t underscorePos = fileName.find('_');
            size_t dotPos = fileName.find('.');

            // Check if the file name has the expected format
            if (underscorePos != std::string::npos && dotPos != std::string::npos) {
                // Extract the name and sequence string from the file name
                std::string name = fileName.substr(0, underscorePos);
                std::string sequenceStr = fileName.substr(underscorePos + 1, dotPos - underscorePos - 1);

                // Check if the sequence string is a valid number
                try {
                    // Get the sequence counter for the current file extension
                    int& sequenceCounter = sequenceCounters[extension];

                    // Generate the new file name with sequential numbering
                    std::string paddedSequenceStr = std::to_string(sequenceCounter + 1);
                    while (paddedSequenceStr.length() < 2) {
                        paddedSequenceStr = "0" + paddedSequenceStr;
                    }
                    std::string newFileName = name + "_" + paddedSequenceStr + extension;

                    // Rename the file by moving it to the new name
                    fs::rename(entry.path(), entry.path().parent_path() / newFileName);

                    // Print the renaming details
                    std::cout << "Renamed: " << fileName << " -> " << newFileName << std::endl;

                    // Increment the sequence counter for the current file extension
                    sequenceCounter++;
                } catch (const std::invalid_argument& e) {
                    // Handle the case where the sequence string is not a valid number
                    std::cout << "Skipping file: " << fileName << " (Invalid sequence number)" << std::endl;
                } catch (const std::out_of_range& e) {
                    // Handle the case where the sequence number is out of range
                    std::cout << "Skipping file: " << fileName << " (Sequence number out of range)" << std::endl;
                }
            }
        }
    }
}


int main() {
    std::string directoryPath;
    
    std::cout << "Enter the directory path: ";
    std::getline(std::cin, directoryPath);

    // Call the renumbering function with the provided directory path
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
