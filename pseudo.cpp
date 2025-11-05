// pseodo code
// include needed librarires
// ight imma need to actually include thse or else my code will be full of red and its just annoying lol
#include <iostream>
#include <string>
#include <map>
#include <array>
#include <list>

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

bool loadData(map<string, array<list<string>, 3>>& farm, string fname){}