// #include "Virtualdisk.h"
// #include <iostream>

// int main() {
//     // Usage example of the Virtualdisk class
//     Virtualdisk::openorcreate("mydisk.bin");

//     // Perform some operations
//     std::vector<char> cluster(1024);
//     Virtualdisk::readcluster(cluster, 0);
    
//     Virtualdisk::writecluster(0);

//     return 0;
// }
#include <iostream>
#include <vector>
#include <string>
#include "MiniFAT.h"
#include "Converter.h"
#include "DirectoryEntry.h"
#include "Virtualdisk.h"

using namespace std;

int main() {
    // Initialize the Virtual Disk
    string diskPath = "virtualdisk.img";
    Virtualdisk::openorcreate(diskPath);

    if (Virtualdisk::isNew()) {
        cout << "A new virtual disk has been created!" << endl;
    } else {
        cout << "Virtual disk already exists and is opened!" << endl;
    }

    // Initialize MiniFAT
    MiniFAT fs;
    fs.initializeOrOpenFileSystem(diskPath);
    cout << "File system initialized." << endl;

    // Print the FAT Table
    cout << "Initial FAT table:" << endl;
    fs.printFAT();

    // Allocate a cluster and set pointers
    int freeCluster = fs.getAvailableCluster();
    if (freeCluster != -1) {
        cout << "First available cluster: " << freeCluster << endl;

        // Set a pointer for the cluster
        fs.setClusterPointer(freeCluster, 123);
        cout << "Set cluster pointer for cluster " << freeCluster << " to 123." << endl;

        // Retrieve the pointer value
        cout << "Cluster " << freeCluster << " pointer value: " << fs.getClusterPointer(freeCluster) << endl;
    } else {
        cout << "No free clusters available!" << endl;
    }

    // Display SuperBlock Information
    fs.createSuperBlock();

    // Test Directory Entry
    DirectoryEntry dir("example.txt", 'F', freeCluster, 512);
    cout << "Directory Entry created for file 'example.txt'." << endl;
    cout << "File Name: " << string(dir.dirName, 11) << endl;
    cout << "File Attribute: " << dir.dirAttr << endl;
    cout << "File First Cluster: " << dir.dirFirstCluster << endl;
    cout << "File Size: " << dir.dirFileSize << endl;

    // Test Converter Functions
    int testInt = 2024;
    vector<char> intBytes = Converter::intToBytes(testInt);
    cout << "Integer 2024 converted to bytes: ";
    for (char c : intBytes) cout << hex << (int)c << " ";
    cout << endl;

    int recoveredInt = Converter::bytesToInt(intBytes);
    cout << "Recovered integer from bytes: " << recoveredInt << endl;

    // Test Virtual Disk Read/Write
    vector<char> cluster(512, 'A'); // Fill the cluster with 'A'
    Virtualdisk::writecluster(0);   // Write cluster 0
    Virtualdisk::readcluster(cluster, 0); // Read cluster 0

    cout << "Data from cluster 0: ";
    for (char c : cluster) cout << c;
    cout << endl;

    return 0;
}
