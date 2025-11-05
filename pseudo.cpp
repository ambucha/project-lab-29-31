// pseodo code
// include needed librarires
// ight imma need to actually include thse or else my code will be full of red and its just annoying lol
#include <iostream>
#include <string>
#include <map>
#include <array>
#include <list>
#include <fstream>

//functions (prototypes)

// loading initial data from external file in
// arguments: reference to the map that has the field +crop data, file to load
bool loadData(map<string, array<list<string>, 3>>& farm, string fname);

// simulating one day
// reference to map, which day of the simulation
void simulate(map<string, array<list<string>, 3>>& farm, int day);

// prints current farm state
// arguments: reference to map, current day
void printState(map<string, array<list<string>, 3>>& farm, int day);

// main
int main(){}

// functions (defs)

bool loadData(map<string, array<list<string>, 3>>& farm, string fname){
    // ifstream to read data from given file
    ifstream fin(fname);

    // if file doesnt open return false
    if(!fin.is_open()){
        return false;
    }

    // temp string var to store each read line from the file
    string data;

    // read each line and insert itto the map
    while(getline(fin, data)){
        // split each line into parts
        // string before the first comma will be the field name
        // then there will be numbers 0 1 and 2, each one linked to a different crop
        // last string after comma will be crop ID i suppose to help me track when the crop is ready to harvest
        // insert these values into the map
    }

    // close the file
    fin.close();

    // return true
    return true;
}

// day simulation
void simulate(map<string, array<list<string>, 3>>& farm, int day){
    
}
