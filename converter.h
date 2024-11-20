#include <vector>
class converter
{
    public:
    static vector<char> inttoByte(int n);
    static int bytetoInt(vector<char> bytes);
    static vector<char> intArraytoByteArray(int* ints,int size);
    static void byteArraytoIntArray(int* ints,vector<char> bytes);
    static vector<vector<char>> splitBytes(vector<char> bytes);
    static Directory_Entry BytesToDirectory_Entry(vector<char> bytes);
    static vector<char> Directory_EntryToBytes(DirectoryEntry d);
};