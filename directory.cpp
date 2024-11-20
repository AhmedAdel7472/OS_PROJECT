#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

// Include necessary components
#include "Virtualdisk.h"  
#include "minifat.h"      
#include "converter.h"    

namespace OSProjectCPP {
    class Directory : public DirectoryEntry {
    private:
        vector<DirectoryEntry> DirOrFiles; // Holds files and subdirectories
        Directory* parent;                // Pointer to the parent directory

    public:
        // Constructor
        Directory(string name, char dir_attr, int dir_firstCluster, Directory* pa)
            : DirectoryEntry(name, dir_attr, dir_firstCluster, 0), parent(pa) {}

        // Get the directory entry of this directory
        DirectoryEntry GetDirectory_Entry() {
            return *this;
        }

        // Get the size of the directory on disk
        int getMySizeOnDisk() {
            int size = 0;
            for (const auto& entry : DirOrFiles) {
                size += entry.dirFileSize;
            }
            return size;
        }

        // Check if an entry can be added to the directory
        bool canAddEntry(DirectoryEntry d) {
            auto it = find_if(DirOrFiles.begin(), DirOrFiles.end(), [&](const DirectoryEntry& entry) {
                return string(entry.dirName) == string(d.dirName);
            });
            return it == DirOrFiles.end(); // Entry can be added if it doesn't exist
        }

        // Clear all clusters associated with this directory
        void emptyMyClusters() {
            DirOrFiles.clear();
        }

        // Write the directory to disk
        void writeDirectory() {
            vector<char> data; // Serialized directory data

            for (const auto& entry : DirOrFiles) {
                data.insert(data.end(), entry.dirName, entry.dirName + sizeof(entry.dirName));
                data.push_back(entry.dirAttr);
                data.insert(data.end(), entry.dirEmpty, entry.dirEmpty + sizeof(entry.dirEmpty));
                vector<char> clusterBytes = Converter::intToBytes(entry.dirFirstCluster);
                data.insert(data.end(), clusterBytes.begin(), clusterBytes.end());
                vector<char> sizeBytes = Converter::intToBytes(entry.dirFileSize);
                data.insert(data.end(), sizeBytes.begin(), sizeBytes.end());
            }

            vector<vector<char>> chunks = Converter::splitBytes(data, 512); // Assuming 512 bytes per cluster
            int clusterIndex = dirFirstCluster;

            for (const auto& chunk : chunks) {
                Virtualdisk::writecluster(clusterIndex);
                clusterIndex = MiniFAT::getAvailableCluster();
                if (clusterIndex == -1) {
                    throw runtime_error("No free clusters available on disk.");
                }
            }
        }

        // Read the directory from disk
        void readDirectory() {
            DirOrFiles.clear(); // Clear current entries
            int clusterIndex = dirFirstCluster;

            while (clusterIndex != -1) {
                vector<char> clusterData(512); // Assuming 512 bytes per cluster
                Virtualdisk::readcluster(clusterData, clusterIndex);

                size_t offset = 0;
                while (offset + sizeof(DirectoryEntry) <= clusterData.size()) {
                    DirectoryEntry entry("", 0, 0, 0);
                    memcpy(entry.dirName, &clusterData[offset], sizeof(entry.dirName));
                    offset += sizeof(entry.dirName);
                    entry.dirAttr = clusterData[offset++];
                    memcpy(entry.dirEmpty, &clusterData[offset], sizeof(entry.dirEmpty));
                    offset += sizeof(entry.dirEmpty);
                    vector<char> clusterBytes(clusterData.begin() + offset, clusterData.begin() + offset + 4);
                    entry.dirFirstCluster = Converter::bytesToInt(clusterBytes);
                    offset += 4;
                    vector<char> sizeBytes(clusterData.begin() + offset, clusterData.begin() + offset + 4);
                    entry.dirFileSize = Converter::bytesToInt(sizeBytes);
                    offset += 4;

                    DirOrFiles.push_back(entry);
                }

                clusterIndex = MiniFAT::getClusterPointer(clusterIndex);
            }
        }

        // Add an entry to the directory
        void addEntry(DirectoryEntry d) {
            if (!canAddEntry(d)) {
                throw runtime_error("Entry already exists in the directory.");
            }
            DirOrFiles.push_back(d);
        }

        // Remove an entry from the directory
        void removeEntry(DirectoryEntry d) {
            auto it = find_if(DirOrFiles.begin(), DirOrFiles.end(), [&](const DirectoryEntry& entry) {
                return string(entry.dirName) == string(d.dirName);
            });
            if (it == DirOrFiles.end()) {
                throw runtime_error("Entry not found in the directory.");
            }
            DirOrFiles.erase(it);
        }

        // Delete the directory and all its contents
        void deleteDirectory() {
            for (auto& entry : DirOrFiles) {
                if (entry.dirAttr == 'D') { // Assuming 'D' indicates a subdirectory
                    Directory subDir(entry.dirName, entry.dirAttr, entry.dirFirstCluster, this);
                    subDir.deleteDirectory();
                }
            }
            emptyMyClusters();
        }

        // Update the content of an entry
        void updateContent(DirectoryEntry OLD, DirectoryEntry NEW) {
            auto it = find_if(DirOrFiles.begin(), DirOrFiles.end(), [&](const DirectoryEntry& entry) {
                return string(entry.dirName) == string(OLD.dirName);
            });
            if (it == DirOrFiles.end()) {
                throw runtime_error("Entry to update not found.");
            }
            *it = NEW;
        }

        // Search for an entry by name
        int searchDirectory(string name) {
            auto it = find_if(DirOrFiles.begin(), DirOrFiles.end(), [&](const DirectoryEntry& entry) {
                return string(entry.dirName).find(name) != string::npos;
            });
            return it == DirOrFiles.end() ? -1 : distance(DirOrFiles.begin(), it);
        }
    };
}
