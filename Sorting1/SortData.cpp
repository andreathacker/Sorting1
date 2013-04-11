//
//  SortData.cpp
//  Sorting1
//
//  Created by David Thacker on 4/11/13.
//  Copyright (c) 2013 David Argyle Thacker. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

class SortData {
    
public:
    
    struct countyData {
        char city [25];
        char county [25];
        int population;
    };
    
    SortData(){
        
    }
    
    void textToByte ()
    {
        string inFilename;
        
        cout << "Enter text file name ";
        cin >> inFilename;
        
        fstream inputCountyFile(inFilename.c_str());
        
        if (!inputCountyFile)
        {
            cout << "text file could not be opened" << endl;
            return;
        }
        
        ofstream outputCountyFile ("records.dat", ios::binary | ios::app);
        fstream outputHashFile ("hashed_county.dat", ios::binary | ios::in | ios::out | ios::app);
        
        if (!outputCountyFile) {
            cout << "data file could not be opened" << endl;
            return;
        }
        
        if (!outputHashFile) {
            cout << "data file could not be opened" << endl;
            return;
        }
        
        int count = 0;
        
        // start reading from the given text input
        readCounty(inputCountyFile, "");
        
        while (!inputCountyFile.eof())	{
            
            outputCountyFile.write(reinterpret_cast<const char *>( &data ),
                                   sizeof( countyData ));            
            count++;
            
            cout << data.city << "    " << data.county << "    " << data.population << endl;
            
            readCounty(inputCountyFile, "");
        }
        
        // is pointer at the end of the file
        if (inputCountyFile.eof())
        {
            cout << "Number of cities listed = " << count << endl;
            cout << "Last city in list " << data.city << endl;
            cout << endl;
        }
        
        else
        {
            cout << "error in input file at city " << data.city << endl;
            cout << data.county << "  " << data.population << endl;
        }
        
        outputCountyFile.close();
        inputCountyFile.close();
    }

    // Recursive method to that reads each individual record from the fstream
    void readCounty(fstream &inStream, string prevIn){
        char buffer[25];
        string stringA = prevIn;
        string stringB;
        
        inStream >> buffer;
        stringB = buffer;
        
        if(stringB == "County")
        {
            strcpy(data.city, cityBuffer.c_str());
            cityBuffer = "";
            strcpy(data.county, (stringA + " " + stringB).c_str());
            inStream >> data.population;
            return;
        }
        else
        {
            if(cityBuffer.size() > 0)
            {
                // there is already one word store so place a space character
                // the next word is added
                cityBuffer += " " + stringA;
            }
            else
            {
                cityBuffer += stringA;
            }
            readCounty(inStream, stringB);
        }
        
    }
    
private:
    
    countyData data;
    countyData hashData;
    string cityBuffer;
    char countyBuffer[50];
    
    string strCity = "EMPTY_CITY_______________";
    string strCounty = "EMPTY_COUNTY_____________";
    
    char emptyCity[25];
    char emptyCounty[25];
};