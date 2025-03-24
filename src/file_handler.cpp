#include <iostream>
#include <fstream>
#include <string>

class FileHandler {
private:
    std::fstream file;
public:
    // Open a file with the specified mode.
    bool open(const std::string& filename, std::ios::openmode mode) {
        file.open(filename, mode);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file \"" << filename << "\".\n";
            return false;
        }
        return true;
    }

    // Read the entire file content into a string.
    std::string read() {
        std::string content, line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        return content;
    }

    // Write data to the file.
    bool write(const std::string& data) {
        if (!file.is_open()) {
            std::cerr << "Error: File is not open for writing.\n";
            return false;
        }
        file << data;
        return true;
    }

    // Close the file if it's open.
    void close() {
        if (file.is_open()) {
            file.close();
        }
    }

    // Ensure the file is closed on destruction.
    ~FileHandler() {
        close();
    }
};
