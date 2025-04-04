#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;

// File Metadata Structure
struct FileMetadata {
    string name;            // File name
    int size;               // Size of the file in bytes
    time_t creationTime;    // Time of creation
    bool isDeleted;         // Flag to indicate if the file is deleted
};

// Directory Structure (simple, with support for nested directories)
struct Directory {
    string name;                            // Directory name
    unordered_map<string, FileMetadata> files;   // Files in this directory
    unordered_map<string, Directory> subdirectories;  // Subdirectories inside this directory
};

// File System Class
class FileSystem {
private:
    Directory rootDirectory;   // Root directory of the file system

    void displayDirectory(const Directory& dir, const string& indent = "") {
        cout << indent << "Directory: " << dir.name << endl;
        for (const auto& file : dir.files) {
            if (!file.second.isDeleted) {
                cout << indent << "  File: " << file.second.name << ", Size: " << file.second.size << " bytes" << endl;
            }
        }
        for (const auto& subDir : dir.subdirectories) {
            displayDirectory(subDir.second, indent + "  ");
        }
    }

public:
    FileSystem() {
        rootDirectory.name = "/";
    }

    // Create a file in the root or subdirectory
    void createFile(const string& directoryPath, const string& fileName, int size) {
        Directory* dir = &rootDirectory;
        
        if (directoryPath != "/") {
            size_t pos = 0;
            string subDirName;
            string path = directoryPath;
            
            while ((pos = path.find("/")) != string::npos) {
                subDirName = path.substr(0, pos);
                dir = &dir->subdirectories[subDirName];
                path.erase(0, pos + 1);
            }
        }

        if (dir->files.find(fileName) == dir->files.end()) {
            FileMetadata file = {fileName, size, time(0), false};
            dir->files[fileName] = file;
            cout << "File '" << fileName << "' created with size " << size << " bytes." << endl;
        } else {
            cout << "Error: File '" << fileName << "' already exists." << endl;
        }
    }

    // Delete a file from the directory
    void deleteFile(const string& directoryPath, const string& fileName) {
        Directory* dir = &rootDirectory;
        
        if (directoryPath != "/") {
            size_t pos = 0;
            string subDirName;
            string path = directoryPath;
            
            while ((pos = path.find("/")) != string::npos) {
                subDirName = path.substr(0, pos);
                dir = &dir->subdirectories[subDirName];
                path.erase(0, pos + 1);
            }
        }

        auto fileIt = dir->files.find(fileName);
        if (fileIt != dir->files.end() && !fileIt->second.isDeleted) {
            fileIt->second.isDeleted = true;
            cout << "File '" << fileName << "' deleted." << endl;
        } else {
            cout << "Error: File '" << fileName << "' not found or already deleted." << endl;
        }
    }

    // Read data from a file (simulated)
    void readFile(const string& directoryPath, const string& fileName) {
        Directory* dir = &rootDirectory;
        
        if (directoryPath != "/") {
            size_t pos = 0;
            string subDirName;
            string path = directoryPath;
            
            while ((pos = path.find("/")) != string::npos) {
                subDirName = path.substr(0, pos);
                dir = &dir->subdirectories[subDirName];
                path.erase(0, pos + 1);
            }
        }

        auto fileIt = dir->files.find(fileName);
        if (fileIt != dir->files.end() && !fileIt->second.isDeleted) {
            cout << "Reading file '" << fileName << "'..." << endl;
            // Simulating file read with some data
            cout << "File Content: [Simulated data for " << fileName << "]" << endl;
        } else {
            cout << "Error: File '" << fileName << "' not found or deleted." << endl;
        }
    }

    // Write data to a file (simulated)
    void writeFile(const string& directoryPath, const string& fileName, const string& content) {
        Directory* dir = &rootDirectory;
        
        if (directoryPath != "/") {
            size_t pos = 0;
            string subDirName;
            string path = directoryPath;
            
            while ((pos = path.find("/")) != string::npos) {
                subDirName = path.substr(0, pos);
                dir = &dir->subdirectories[subDirName];
                path.erase(0, pos + 1);
            }
        }

        auto fileIt = dir->files.find(fileName);
        if (fileIt != dir->files.end() && !fileIt->second.isDeleted) {
            cout << "Writing to file '" << fileName << "': " << content << endl;
            // Simulate writing data to the file (update file size, etc.)
            fileIt->second.size += content.size();
        } else {
            cout << "Error: File '" << fileName << "' not found or deleted." << endl;
        }
    }

    // Create a subdirectory
    void createDirectory(const string& directoryPath, const string& dirName) {
        Directory* dir = &rootDirectory;
        
        if (directoryPath != "/") {
            size_t pos = 0;
            string subDirName;
            string path = directoryPath;
            
            while ((pos = path.find("/")) != string::npos) {
                subDirName = path.substr(0, pos);
                dir = &dir->subdirectories[subDirName];
                path.erase(0, pos + 1);
            }
        }

        if (dir->subdirectories.find(dirName) == dir->subdirectories.end()) {
            dir->subdirectories[dirName] = Directory{dirName};
            cout << "Directory '" << dirName << "' created." << endl;
        } else {
            cout << "Error: Directory '" << dirName << "' already exists." << endl;
        }
    }

    // Display the root directory and subdirectories
    void displayFileSystem() {
        displayDirectory(rootDirectory);
    }
};

int main() {
    FileSystem fs;

    // Create directories
    fs.createDirectory("/", "docs");
    fs.createDirectory("/docs", "images");

    // Create files
    fs.createFile("/docs", "file1.txt", 100);
    fs.createFile("/docs/images", "image1.jpg", 500);

    // Read and write files
    fs.readFile("/docs", "file1.txt");
    fs.writeFile("/docs/images", "image1.jpg", "New Image Data");

    // Delete files
    fs.deleteFile("/docs", "file1.txt");

    // Display file system structure
    fs.displayFileSystem();

    return 0;
}
