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
#include <vector>
// includes for the rand
#include <cstdlib>
#include <ctime>
using namespace std; // forgot to add this lol

// ammount of weather options, should be a constant
const int WEATHER_CONST = 3;

// add number of crops and base growth rates
const int NUM_OF_CROPS = 3;
const int CASSAVA_RATE = 25;
const int CACAO_RATE = 35;
const int COFFEE_RATE = 45;
const int NUM_CROPS = 50;

//functions (prototypes)

// loading initial data from external file in
// arguments: reference to the map that has the field +crop data, file to load
bool loadData(map<string, array<list<int>, 3>>& farm, string fname);

// simulating one day
// reference to map, which day of the simulation
// imma need to update my functions to now also have the harvested count 
void simulate(map<string, array<list<int>, 3>>& farm, int day, map<string, array<int,3>>& harvested);

// prints current farm state
// arguments: reference to map, current day
void printState(map<string, array<list<int>, 3>>& farm, int day, const map<string, array<int,3>>& harvested);

// now that its the final release i think its good to add a function to populate the farm_data.csv
// generateData() opens the farm_data.csv file and populates it with random data
void generateData(const string& fname);

// main
int main(){
    // seed the rand
    srand(time(0));

    // farm is the main data struct
    map<string, array<list<int>, 3>> farm;
    // i feel as though a crucial thing i am missing is how many plants were actualyl harvested
    // im tracking not the most important stats, i need to know how much of each crop was actually harvested thats such a crucial piece of info
    // lets create somehting here to keep track of how much of each was harvested
    map<string, array<int,3>> harvested;

    // filename of the data file
    string fName = "farm_data.csv";

    // generate the data
    generateData(fName); 

    // check, wether file loaded correctly or no
    bool check = loadData(farm,fName);

    // if file does not open print out the fact that it did not open
    // test to see if the file is opened
    if(check){
        cout << "File opened and read" << endl;
    }

    if(!check){
        cout << "ERROR: Farm data file could not be opened" << endl;
        return 1;
    }

    // print initial farm state to show how the farm is before the simulation begins
    printState(farm,0, harvested); // 0 to show that it is initial state

    // run the simulation for 30 time periods
    for(int i = 1; i <= 30; i++){
        // for each day simulate
        simulate(farm, i, harvested);
        // then print farm state at end of each day
        printState(farm, i, harvested);
    }

    // print final results after all days are simulated
    printState(farm, -1, harvested);

    return 0;
}

// functions (defs)

// now imma input some sample data into the farm_data.csv for some testing
// wait imma change cropID to just be crop  stage, i dont technically care what the ID for each individual crop is, since the data 
// i want to be collecting is mainly focused of how things affect the yield of the crop
// I will make it so that the cropID is actually the stage of readiness its add so i will start at like S1 moving all the way to S5 and then READY when it can be harvested
// wait why overcomplicate it making them string, lemme just make the stage count go from 1-6 and 6 be equal to ready
bool loadData(map<string, array<list<int>, 3>>& farm, string fname){
    // ifstream to read data from given file
    ifstream fin(fname);

    // if file doesnt open return false
    if(!fin.is_open()){
        return false;
    }

    // temp string var to store each read line from the file
    string data;
    // int line number for error outputs
    int line = 0;

    // read each line and insert itto the map
    while(getline(fin, data)){
        // lowk i should use stringstream to read the data no?
        line++;
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
        int stage;
        // use try catch to handle exceptions if the cropString is not a number
        try {
            // convert string to integer
            crop = stoi(cropString);
        } catch(...) {
            // use cerr to ouput if something went wrong
            cerr << "WARNING: line " << line << " has a non integer index" << endl; // i should add a way to know what line of the file we are in
            continue; // go to the next line if it can not be converted to an integer
        }

        try {
            // convert string to integer
            stage = stoi(cropID);
        } catch(...) {
            // use cerr to ouput if something went wrong
            cerr << "WARNING: line " << line << " has a non integer index" << endl; // i should add a way to know what line of the file we are in
            continue; // go to the next line if it can not be converted to an integer
        }

        // check if crop is between 0-2 so if it is a valid index
        if(crop < 0 || crop > 2){
            cerr << "WARNING: line " << line << " has an unknown crop type" << endl;
            continue; 
        }

        // oops was getting errors since i was checking crop instead of stage
        if(stage < 1 || stage > 6){
            cerr << "WARNING: line " << line << " is at an unknown stage" << endl;
            continue; 
        }

        // now insert values into map
        farm[fieldName][crop].push_back(stage);
    }

    // close the file
    fin.close();

    // return true
    return true;
}

// day simulation
void simulate(map<string, array<list<int>, 3>>& farm, int day, map<string, array<int,3>>& harvested){
    // ok so first imma randomly select weather condition for this day, lets start with three possible options
    // 0 = rani, 1 = heat, 2 = normal
    // imma make a variable so that i can up the weather options easilty if needed
    int weather = rand() % WEATHER_CONST;
    
    // ok one thing i need to add is how the weather actually affects the crops, and i want it to be different depending on the crop as well
    // i need to add base growth rates for each crop, lets give cassava a 25% growth rate, cacao 35%, and coffee 45%
    static const int baseGrowth[NUM_OF_CROPS] = {CASSAVA_RATE,CACAO_RATE,COFFEE_RATE};
    // also made this a static const^^

    // now i need to add environmental factor
    // if it weather = 0 rain, so it speeds up
    // if weather = 1 it is heat so slows down
    int weatherRate = 0;
    if(weather == 0){
        weatherRate = 10;
    }
    if(weather == 1){
        weatherRate = -10;
    }

    // now i need to make it so that each different field affects crops differently
    // lets say north field makes cassava and coffee thrive, while brings cacao down a bit
    // south field makes  cacao thriveand hurts coffee, while cassava is neutral
    // and east field is neutral for all, our control group even
    // these maps are currently being rebuilt every day run i believe so i should just make them static consts
    static const map<string, array<int,NUM_OF_CROPS>> fieldRate = {
        {"North Field", {5,-5,5}}, // north field, coffee and cassava thrive and cacao is hurt
        {"South Field", {0,5,-5}},
        {"East Field", {0,0,0}}
    };

    // OOOO i can also make the chance of new plants sprouting different depending on the field, this could make it so cool
    // so lets make chance of replanting out of 100 for each type of plant for each field
    static const map<string, array<int,NUM_OF_CROPS>> fieldSprout = {
        {"North Field", {20,10,15}}, // north field, coffee and cassava thrive and cacao is hurt
        {"South Field", {10,25,5}},
        {"East Field", {10,10,10}}
    };

    // loop through each field in map
    for(auto& f : farm){
        // f.first = name of field
        // f.second = array of the three crop lists
        // for each crop in this field - adjust growth due to weather, advance some crops growth stage, remove harvestable crops, add new budding crops
        const string& field = f.first;
        auto& lists = f.second;
        
        // okay now i have to find the fieldRates and the field sprout rates
        // start with a default array of 0s
        array<int,NUM_OF_CROPS> rate = {0,0,0};
        // now imma try and find the field in the fieldRate map
        auto itRate = fieldRate.find(field);
        // now if it is found
        if(itRate != fieldRate.end()){
            // set the three values to be the rates
            rate = itRate->second;
        }

        // same thing for sprout
        array<int,NUM_OF_CROPS> sprout = {0,0,0};
        // now imma try and find the field in the fieldRate map
        auto itSprout = fieldSprout.find(field);
        // now if it is found
        if(itSprout != fieldSprout.end()){
            // set the three values to be the rates
            sprout = itSprout->second;
        }

        // ok nowlets make a loop for each of thecrop types of that given  field 
        for(int i = 0; i < NUM_OF_CROPS; i++){
            // we calculate the total growth chance for the given crop
            int chance = baseGrowth[i] + weatherRate + rate[i]; // add the original base rate of the plant with the weather affect and the rate of the crop on the filed
            
            // i should make it so that the growth chance can never be the extreems, like 100 or 0 to add an element of randomness
            if(chance > 95){
                chance = 95;
            }
            if(chance < 5){
                chance = 5;
            }
            
            // list of all individual plants for that crop type
            auto& l = lists[i];
            // now we loop through each plant of that crop type
            for (auto it = l.begin(); it != l.end();){
                // check if it is ready to harvest
                if(*it >= 6){
                    // add to the harvest count herre
                    ++harvested[field][i];
                    it = l.erase(it); // harvest the plant
                    continue;
                }
                
                // roll the dice to see if the plant grows, grow by one stage
                if((rand() % 100) < chance){
                    (*it)++;
                    // now check if it is ready to be harvest
                    if(*it >= 6) {
                        // here too add to the harvest count 
                        ++harvested[field][i];
                        it = l.erase(it); // harvest
                        continue;
                    }
                }

                it++;
            }

            // check for sprouting
            if((rand() % 100) < sprout[i]){
                l.push_back(1); // push_backa stage one plant
            } 
        }
    }

    // print that the day ended
    // i should print out what the weather was for each daty too
    string w;
    if(weather == 0){
        w = "Rainy";
    }
    if(weather == 1){
        w = "Hot";
    }
    if(weather == 2){
        w = "Normal";
    }

    cout << "Day " << day << " done." << endl; 
    cout << "Weather: " << w << endl;
}

// print farm state
void printState(map<string, array<list<int>, 3>>& farm, int day, const map<string, array<int,3>>& harvested){
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
        array<list<int>, 3>& cLists = f.second;
        
        // i can adjust what i want to output but for nnow lets keep it basic
        cout << fName << ": " << endl;
        cout << "\tCassava count: " << cLists[0].size() << endl;
        cout << "\tCacao count: " << cLists[1].size() << endl;
        cout << "\tCoffee count: " << cLists[2].size() << endl;

        // find the field
        auto itH = harvested.find(fName);
        // set a base harvest count of 0s in case field not found
        array<int,3> h = {0,0,0};
        // set the actual harvest count
        if(itH != harvested.end()) {
            h = itH->second;
        }

        // output the harvested details
        cout << "Harvested: " << endl;
        cout << "\tCassava: " << h[0] << endl;
        cout << "\tCacao: " << h[1] << endl;
        cout << "\tCoffee: " << h[2] << endl;
    }

    cout << endl;
}

void generateData(const string& fname){
    // open the file to output data into
    ofstream fout(fname);
    // check if file is open and if  not output an error
    if(!fout.is_open()){
        cerr << "ERROR: Output file could not be opened" << endl;
        return;
    }

    // create a vector to hold the field names - north field south field east field
    vector<string> fields = {"North Field", "South Field", "East Field"};
    int fieldCount = fields.size();

    // loop through the fields
    for(int i = 0; i < fields.size();i++){
        // now we need to cycle through each of the names
        string field = fields[i % fieldCount];

        // now loop through the ammount of crops you want to add per field
        for(int j = 0; j < NUM_CROPS; j++){
            // get a random crop
            int crop = rand() % 3; // this should give us random from 0-2
            // now we want random stage from 1-6
            int stage = 1 + rand() % 6;
            // output the data to the file
            fout << field << "," << crop << "," << stage << endl;
        }
    }
}