#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include "Virtualdisk.h"
#include "minifat.h"
#include "converter.h"
#include "directoryentry.h" // Ensure DirectoryEntry is defined in its own header file

namespace OSProjectCPP {

    class Directory : public DirectoryEntry {
    private:
        std::vector<DirectoryEntry> DirOrFiles; // Holds files and subdirectories
        Directory* parent;                      // Pointer to the parent directory

    public:
        // Constructor
        Directory(std::string name, char dir_attr, int dir_firstCluster, Directory* pa);

        // Get the directory entry of this directory
        DirectoryEntry GetDirectory_Entry();

        // Get the size of the directory on disk
        int getMySizeOnDisk();

        // Check if an entry can be added to the directory
        bool canAddEntry(DirectoryEntry d);

        // Clear all clusters associated with this directory
        void emptyMyClusters();

        // Write the directory to disk
        void writeDirectory();

        // Read the directory from disk
        void readDirectory();

        // Add an entry to the directory
        void addEntry(DirectoryEntry d);

        // Remove an entry from the directory
        void removeEntry(DirectoryEntry d);

        // Delete the directory and all its contents
        void deleteDirectory();

        // Update the content of an entry
        void updateContent(DirectoryEntry OLD, DirectoryEntry NEW);

        // Search for an entry by name
        int searchDirectory(std::string name);
    };

} // namespace OSProjectCPP

#endif // DIRECTORY_H
