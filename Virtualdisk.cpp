#include <fstream>
#include <vector>
#include <string>
#include<Virtualdisk.h>

using namespace std;

fstream Virtualdisk::disk;

// Function to open or create the virtual disk file
void Virtualdisk::openorcreate(string path) {
    // Open the disk file in read/write mode, creating it if it doesn't exist
    disk.open(path, ios::in | ios::out | ios::binary);

    // If the file doesn't exist, create it
    if (!disk) {
        // Create a new file
        disk.open(path, ios::out | ios::binary);
        disk.close();
        // Reopen it in read/write mode
        disk.open(path, ios::in | ios::out | ios::binary);
    }
}

// Function to read a cluster of data from the virtual disk
void Virtualdisk::readcluster(vector<char> cluster, int cluster_index) {
    int cluster_size = cluster.size();
    // Seek to the beginning of the desired cluster
    disk.seekg(cluster_index * cluster_size, ios::beg);

    // Read the data into the cluster vector
    disk.read(&cluster[0], cluster_size);
}

// Function to write a cluster of data to the virtual disk
char * Virtualdisk::writecluster(int cluster_index) {
    static char data[512]; // Assuming a cluster size of 512 bytes

    // Seek to the correct cluster index position
    disk.seekp(cluster_index * sizeof(data), ios::beg);

    // Write the data to the disk at the specified position
    disk.write(data, sizeof(data));

    return data;
}

// Function to check if the disk file is new
bool Virtualdisk::isNew() {
    return !disk.is_open();
}
