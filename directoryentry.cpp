class DirectoryEntry {
public:
    char dirName[11];
    char dirAttr;
    char dirEmpty[12];
    int dirFirstCluster;
    int dirFileSize;

    DirectoryEntry(string name, char dirAttr, int dirFirstCluster, int dirFileSize = 0) {
        assignDirName(name);
        this->dirAttr = dirAttr;
        memset(dirEmpty, 0, sizeof(dirEmpty));
        this->dirFirstCluster = dirFirstCluster;
        this->dirFileSize = dirFileSize;
    }

    static string cleanTheName(string s) {
        size_t pos = s.find_last_not_of(' ');
        if (pos != string::npos) {
            s.erase(pos + 1);
        }
        return s;
    }

    void assignFileName(string name, string extension) {
        string fullName = cleanTheName(name) + "." + cleanTheName(extension);
        assignDirName(fullName);
    }

    void assignDirName(string name) {
        memset(dirName, ' ', sizeof(dirName));
        size_t len = min(name.size(), size_t(11));
        for (size_t i = 0; i < len; ++i) {
            dirName[i] = name[i];
        }
    }
};
