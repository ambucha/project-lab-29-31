// pseodo code
// include needed librarires
// ight imma need to actually include thse or else my code will be full of red and its just annoying lol
#include <iostream>
#include <string>
#include <map>
#include <array>
#include <list>
#include <fstream>
#include <sstream>
using namespace std; // forgot to add this lol

// ammount of weather options
int WEATHER_CONST = 3;

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
int main(){
    // farm is the main data struct
    map<string, array<list<string>, 3>> farm;

    // filename of the data file
    string fName = "farm_data.csv";

    // check, wether file loaded correctly or no
    bool check = loadData(farm,fName);

    // if file does not open print out the fact that it did not open
    if(!check){
        cout << "ERROR: Farm data file could not be opened" << endl;
        return 1;
    }

    // print initial farm state to show how the farm is before the simulation begins
    printState(farm,0); // 0 to show that it is initial state

    // run the simulation for 30 time periods
    for(int i = 1; i <= 30; i++){
        // for each day simulate
        simulate(farm, i);
        // then print farm state at end of each day
        printState(farm, i);
    }

    // print final results after all days are simulated
    printState(farm, -1);
    
    return 0;
}

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
        // lowk i should use stringstream to read the data no?
        stringstream d(data);

        // create the three variabnles for the different data
        string fieldName;
        string cropString; // imma need to collect it as a string and convert it to an int
        string cropID;

        // split the line into its components
        // make it so that if something goes wrong it skips the line entirely instead of inputting bad data
        if(!getline(d, fieldName, ',')) continue; // continue so that if one does not work it wont input the bad data
        if(!getline(d, cropString, ',')) continue;
        if(!getline(d, cropID)) continue;

        // convert number string to an int
        int crop;
        // use try catch to handle exceptions if the cropString is not a number
        try {
            // convert string to integer
            crop = stoi(cropString);
        } catch(...) {
            // use cerr to ouput if something went wrong
            cerr << "WARNING: line has a non integer index";
            continue; // go to the next line if it can not be converted to an integer
        }

        // check if crop is between 0-2 so if it is a valid index
        if(crop < 0 || crop > 2){
            cerr << "WARNING: line has an unknown crop type";
            continue; 
        }

        // now insert values into map
        farm[fieldName][crop].push_back(cropID);
    }

    // close the file
    fin.close();

    // return true
    return true;
}

// day simulation
void simulate(map<string, array<list<string>, 3>>& farm, int day){
    // ok so first imma randomly select weather condition for this day, lets start with three possible options
    // 0 = rani, 1 = heat, 2 = normal
    // imma make a variable so that i can up the weather options easilty if needed
    int weather = rand() % WEATHER_CONST;

    // loop through each field in map
    for(auto& f : farm){
        // f.first = name of field
        // f.second = array of the three crop lists
        // for each crop in this field - adjust growth due to weather, advance some crops growth stage, remove harvestable crops, add new budding crops
    }

    // print that the day ended
    cout << "Day " << day << "done." << endl; 
}

// print farm state
void printState(map<string, array<list<string>, 3>>& farm, int day){
    // set a base case for the initial state and final state
    if(day == 0){
        cout << "Initial State: " << endl;
    }
    else if(day == -1){
        cout << "Final State: " << endl;
    }
    else {
        cout << "Day " << day << ": " << endl;
    }

    //now loop through every field in the map
    for(auto& f : farm){
        // get the name of the field
        string fName = f.first;

        // array of the three crops lists belonging to the field
        array<list<string>, 3>& cLists = f.second;
        
        // i can adjust what i want to output but for nnow lets keep it basic
        cout << "Field " << fName << ": " << endl;
        cout << "Cassava count: " << cLists[0].size() << endl;
        cout << "Cacao count: " << cLists[1].size() << endl;
        cout << "Coffee count: " << cLists[2].size() << endl;
    }

    cout << endl;
}
