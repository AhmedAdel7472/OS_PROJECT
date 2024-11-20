#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>

using namespace std;

class Converter {
public:
    static vector<char> intToBytes(int n) {
        vector<char> bytes(4);
        for (int i = 0; i < 4; ++i) {
            bytes[i] = (n >> (i * 8)) & 0xFF;
        }
        return bytes;
    }

    static int bytesToInt(const vector<char>& bytes) {
        if (bytes.size() != 4) {
            throw invalid_argument("Invalid byte array size for conversion to int.");
        }
        int n = 0;
        for (int i = 0; i < 4; ++i) {
            n |= (static_cast<unsigned char>(bytes[i]) << (i * 8));
        }
        return n;
    }

    static vector<char> intArrayToBytes(int* ints, int size) {
        vector<char> bytes(size * 4);
        for (int i = 0; i < size; ++i) {
            vector<char> temp = intToBytes(ints[i]);
            for (int j = 0; j < 4; ++j) {
                bytes[i * 4 + j] = temp[j];
            }
        }
        return bytes;
    }

    static void bytesToIntArray(int* ints, const vector<char>& bytes, int size) {
        if (bytes.size() != size * 4) {
            throw invalid_argument("Invalid byte array size for conversion to int array.");
        }
        for (int i = 0; i < size; ++i) {
            ints[i] = bytesToInt(vector<char>(bytes.begin() + i * 4, bytes.begin() + (i + 1) * 4));
        }
    }

    static vector<vector<char>> splitBytes(const vector<char>& bytes, int chunkSize) {
        vector<vector<char>> chunks;
        for (size_t i = 0; i < bytes.size(); i += chunkSize) {
            chunks.emplace_back(bytes.begin() + i, bytes.begin() + min(bytes.size(), i + chunkSize));
        }
        return chunks;
    }
};
