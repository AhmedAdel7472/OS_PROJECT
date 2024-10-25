#include <fstream>
#include <vector>
#include <string>
using namespace std;
class Virtualdisk
{
    public:
    static fstream disk;
    static void openorcreate(string path);
    static void readcluster(vector<char> cluster,int cluster_index);
    static char * writecluster(int cluster_index);
    static bool isNew();
};