#ifndef MINIFAT_H
#define MINIFAT_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>

class MiniFAT {
private:
    static const int FAT_SIZE = 1024; // Size of the FAT table
    int FAT[FAT_SIZE] = {0};          // The FAT table
    std::string fileName;             // File where the FAT is stored

    // Saves the FAT table to the disk
    void saveToDisk();

    // Loads the FAT table from the disk
    void loadFromDisk();

public:
    // Initializes or opens the file system with a given name
    void initializeOrOpenFileSystem(std::string name);

    // Gets the index of an available cluster (or -1 if none)
    int getAvailableCluster();

    // Gets the total number of free clusters
    int getFreeSize();

    // Sets a cluster pointer to a value
    void setClusterPointer(int clusterIndex, int pointer);

    // Gets the pointer value of a cluster
    int getClusterPointer(int clusterIndex);

    // Resets the FAT table to zero
    void initializeFAT();

    // Reads the FAT table from the disk
    void readFAT();

    // Prints the current FAT table to the console
    void printFAT();

    // Writes the current FAT table to the disk
    void writeFAT();

    // Creates a superblock with metadata about the file system
    void createSuperBlock();
};

#endif // MINIFAT_H
