#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

using namespace std;

class MiniFAT {
private:
    static const int FAT_SIZE = 1024;
    int FAT[FAT_SIZE] = {0};
    string fileName;

    void saveToDisk() {
        ofstream file(fileName, ios::binary | ios::out);
        if (!file) {
            throw runtime_error("Failed to open file for writing.");
        }
        file.write(reinterpret_cast<char*>(FAT), sizeof(FAT));
        file.close();
    }

    void loadFromDisk() {
        ifstream file(fileName, ios::binary | ios::in);
        if (!file) {
            throw runtime_error("Failed to open file for reading.");
        }
        file.read(reinterpret_cast<char*>(FAT), sizeof(FAT));
        file.close();
    }

public:
    void initializeOrOpenFileSystem(string name) {
        fileName = name;
        ifstream file(fileName, ios::binary);
        if (file.good()) {
            // File exists, load FAT from disk
            loadFromDisk();
        } else {
            // File does not exist, initialize FAT and save it
            initializeFAT();
            saveToDisk();
        }
    }

    int getAvailableCluster() {
        for (int i = 0; i < FAT_SIZE; ++i) {
            if (FAT[i] == 0) {
                return i;
            }
        }
        return -1; // No available cluster
    }

    int getFreeSize() {
        int freeSize = 0;
        for (int i = 0; i < FAT_SIZE; ++i) {
            if (FAT[i] == 0) {
                ++freeSize;
            }
        }
        return freeSize;
    }

    void setClusterPointer(int clusterIndex, int pointer) {
        if (clusterIndex < 0 || clusterIndex >= FAT_SIZE) {
            throw out_of_range("Cluster index out of range.");
        }
        FAT[clusterIndex] = pointer;
        saveToDisk(); // Persist changes to disk
    }

    int getClusterPointer(int clusterIndex) {
        if (clusterIndex < 0 || clusterIndex >= FAT_SIZE) {
            throw out_of_range("Cluster index out of range.");
        }
        return FAT[clusterIndex];
    }

    void initializeFAT() {
        memset(FAT, 0, sizeof(FAT));
    }

    void readFAT() {
        loadFromDisk();
    }

    void printFAT() {
        cout << "FAT Table:" << endl;
        for (int i = 0; i < FAT_SIZE; ++i) {
            cout << "Cluster " << i << ": " << FAT[i] << endl;
        }
    }

    void writeFAT() {
        saveToDisk();
    }

    void createSuperBlock() {
        // In this context, a superblock might just represent FAT metadata
        cout << "Creating SuperBlock:" << endl;
        cout << "Total Clusters: " << FAT_SIZE << endl;
        cout << "Free Clusters: " << getFreeSize() << endl;
    }
};
