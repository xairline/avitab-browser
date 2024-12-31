#ifndef PATH_H
#define PATH_H

#include <string>
using namespace std;

class Path {
private:
    Path();
    ~Path();
    static Path* instance;

public:
    string rootDirectory;
    string pluginDirectory;
    string aircraftDirectory;
    string aircraftFilename;
    
    static Path* getInstance();
    void reloadPaths();
};

#endif
