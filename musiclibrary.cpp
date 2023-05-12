/// TODO Update File Header
///
/// A program that loads in music data and allows the user to see the information on that music data, write the music data to a new file, and search for specified items in the music data
/// Matthew Mohaupt
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <stdexcept>

using namespace std;

// INSERTS GIVEN HELPERS CODE BELOW - DO NOT REMOVE
#include "helpers.cpp"
// INSERTS GIVEN HELPERS CODE ABOVE - DO NOT REMOVE

//
// NOTE TO STUDENTS:
// You may use the functions in the helpers file,
// but you may not modify the file or its contents in any way whatsoever.
//

//
// STUDENT CODE MAY GO BELOW THIS LINE
//


//the struct that is accessed in the map with the keyword of an album name, holds a set of artists and songs, so that if there are more than 1 artists or songs in album it wont be an issue
struct albuminfo{
    set<string> artists;
    set<string> songs;
};


//we take the file name and a map with keyword of the album name and holds the struct albuminfo
//knowing that the first and last line of data is the album name of a data chunk, we are able to hold the first line and stop grabbing new lines new line and first line are the same. then we know we got all the artists and songs in the album
void loadding(const string& filename, map<string, albuminfo>& loaded){
    ifstream ifs;
    string albumname, temp, numcheck;
    albuminfo nfo;
    int tempi;

    ifs.open(filename);

    while(getline(ifs, albumname)){
        getline(ifs, temp);

        while(temp != albumname){
            try{
                tempi = stoi(temp);  
                if(tempi > -1){
                    nfo.songs.insert(temp);
                }
                else{
                    throw(tempi);
                }
            }
            catch(...){
                nfo.artists.insert(temp);
            } 
            getline(ifs, temp);
        }
        loaded[albumname] = nfo;
        nfo.artists.clear();
        nfo.songs.clear();
    }  
    ifs.close();
}

//looping through the map with each element of it having a key and a correspond albuminfo, we can count the number of elements in the map for total number of albums
//once inside an element of the, we can grab the song set size and loop through the artists set and add those artists in the unique artists set because artists can make multiple albums and if we add the same artist again, the set wont keep it
//once we are done looping through the map, we then can just get the size of the unique artists set and send it 
void printqueue(const map<string, albuminfo>& data){
    cout << "Overall Music Library Stats\n===========================" << endl;
    int albumcount =0;
    int songcount = 0;
    set<string> uniquearts;
    for(auto &x : data){
        albumcount++;
        for(auto &y: x.second.artists){
            uniquearts.insert(y);
        }
        songcount += x.second.songs.size();
    }
    cout << "Total Records: " << albumcount << "\nTotal Unique Artists: " << uniquearts.size() << "\nTotal Songs: " << songcount << "\n" << endl;
}

//calls printqueue so the print the total music library stats and then, it will will loop through each element of the map, with the key being the album name, then going in 2 seperate loops, it will print the set of artists first, then the set of songs in the album
void printlist(const map<string, albuminfo>& data){
    printqueue(data);
    cout << "Your Current Music Library Includes\n===================================" << endl;
    for(auto &x : data){
        cout << x.first << endl;
        for(auto &y: x.second.artists){
            cout << " " << y << endl;
        }
        for(auto &y: x.second.songs){
            cout << "   " << y << endl; 
        }
    }
    cout <<  endl;
}

//takes first the map with key album name and holds albuminfo that it will search through
//then a set of strings where it will store the relevent album name if the conditions are met
//then a string to determine what mode the search is in
//finally a string that might be in the map
//it will loop through the map looking for constricts within the specified domain mode states
//once constricts is found, it will be added the album to the set of strings
void searchlist(const map<string, albuminfo>& data, set<string>& storage, const string& mode, const string& constricts){
    string tempname;
    int found;
    if(mode == "album"){
        for(auto &x : data){
            found = -1;
            tempname = x.first;
            tolower(tempname);
            found = tempname.find(constricts);
            if(found >= 0){
                storage.insert(x.first);
            }
        }
    }
    else if(mode == "artist"){
        for(auto &x : data){
            found = -1;
            for(auto &y: x.second.artists){
                tempname = y;
                tolower(tempname);
                found = tempname.find(constricts);
                if(found >= 0){
                    break;
                }
            }
            if(found >= 0){
                storage.insert(x.first);
            }
        }
    }
    else if(mode == "song"){
        for(auto &x : data){
            found = -1;
            for(auto &y: x.second.songs){
                tempname = y;
                tolower(tempname);
                found = tempname.find(constricts);
                if(found >= 0){
                    break;
                }
            }
            if(found >= 0){
                storage.insert(x.first);
            }
        }
    }
    else{
        cout << "pick a correct search term" << endl;
    }
}


int main()
{
    string userEntry;
    string command, remains;

    // TODO: Declare any necessary variables for the music libary
    map<string, albuminfo> loaded;
    ifstream ifs;
    ofstream ofs;
    bool legal = false;
    bool islist = false;
    string searcher, addin;
    set<string> results;


    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
     cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        // Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            // clears the map and sets the flag of if there is data in the map to false
            islist = false;
            loaded.clear();

        }
        else if (command == "export")
         {
            // does basically the same thing print list except instead of printing to the terminal with cout, use output fstream to write to a file and if nothing was put for file name then use default text filename 
            if(islist){
               if(remains.size() <= 0){
                    remains = "musicdatabase.txt";
                }
                ofs.open(remains);
                for(auto &x : loaded){
                    ofs << x.first << endl;               
                    for(auto &y: x.second.artists){
                        ofs << y << endl;
                    }
                    for(auto &y: x.second.songs){
                        ofs << y << endl; 
                    }
                ofs << x.first << endl;
                }
            ofs.close();
            }
            else{
                cout << "export failed" << endl;
            }
        }
        else if (command == "load")
        {
            // if there is nothing in remains, load the default value, then check if the filename is correct
            //if it is set the flag that the file name is correct is true, call the function to parse the data, then set the flag false again so no subsequent false positives
            if(remains.size() <= 0){
                remains = "musicdatabase.txt";
            }
            ifs.open(remains);
            if(ifs.fail()){
                cout << "Error: Could not open music library file - " << remains << endl;
                ifs.close();
            }
            else{
                ifs.close();
                legal = true;
            }
            if(legal){
                loadding(remains, loaded);
                islist = true;
                legal = false;
            }
        }
        else if (command == "stats")
        {
            // if the -d flag is present, then it will print out the entire music library statistic with all the specifics of albums, artist, and songs, otherwise it will just print the numbers
            
            if(remains == "-d"){
                printlist(loaded);
            }
            else{
                printqueue(loaded);
            }
        }
        else if (command == "search")
        {
            // after splitting remains into a mode and what the user is looking for and both have values
            //it can call searchlist() inorder to see if any of the specified modes contain in the substring the user is looking for and if so then it will show the albums that contain that information
            //modeifers and multiplies have not been implemented
            splitFirstWord(remains, searcher, addin);
            tolower(searcher);
            tolower(addin);
            if((searcher.size() <= 0) || (addin.size() <= 0)){
                cout << "Error: Search terms cannot be empty.\nNo results found.\n" << endl;
            }
            else{
                results.clear();
                searchlist(loaded, results, searcher, addin);
                if(results.size() > 0){
                    cout << "Your search results exist in the following albums: " << endl;
                    for (auto &x: results){
                        cout << x << endl;
                    }
                    cout << endl;
                }
                else{
                    cout << "No results found." << endl;
                }
            }
            
        }

    }while(command != "exit");

    cout << "Thank you for using the Music Library App" << endl;



    return 0;
}
