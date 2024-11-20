#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H

#include <string>
#include <cstring>

class DirectoryEntry {
public:
    char dirName[11];      // Directory name (11 characters)
    char dirAttr;          // Directory attributes
    char dirEmpty[12];     // Empty space for padding or reserved
    int dirFirstCluster;   // First cluster of the directory/file
    int dirFileSize;       // Size of the file (in bytes)

    // Constructor to initialize a directory entry
    DirectoryEntry(std::string name, char dirAttr, int dirFirstCluster, int dirFileSize = 0);

    // Cleans up the name by trimming spaces
    static std::string cleanTheName(std::string s);

    // Assigns a file name and extension to the directory entry
    void assignFileName(std::string name, std::string extension);

    // Assigns a name to the directory entry
    void assignDirName(std::string name);
};

#endif // DIRECTORYENTRY_H
