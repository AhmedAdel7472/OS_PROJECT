#include "Virtualdisk.h"
#include <iostream>

int main() {
    // Usage example of the Virtualdisk class
    Virtualdisk::openorcreate("mydisk.bin");

    // Perform some operations
    std::vector<char> cluster(1024);
    Virtualdisk::readcluster(cluster, 0);
    
    Virtualdisk::writecluster(0);

    return 0;
}
