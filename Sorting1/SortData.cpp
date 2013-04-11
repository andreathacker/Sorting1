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
#include <vector>
using namespace std;

class SortData {
    
public:
    
    struct countyData {
        char city [25];
        char county [25];
        int population;
        
        inline bool operator < ( const countyData &c) const {
            return population < c.population;
        }
    };
    
    SortData(){
        
    }
    
    void textToByte (int size)
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
        
        int count = 0;
        recordNum = (size + 1) / 2;
        heapSize = 0;
        heapArray = vector<countyData>(recordNum * sizeof(countyData));
  
        // start reading from the given text input
        readCounty(inputCountyFile, "");
        
        while (!inputCountyFile.eof())	{
            
            if( count < recordNum ){
                insertHeap(data);
            }else if ( count == recordNum){
                writeHeapToBinary(heapArray, "records_a.dat");
                makeEmpty();
                insertHeap(data);
            }else if ( count > recordNum){
                insertHeap(data);
            }
            
            
            
            readCounty(inputCountyFile, "");
            count++;
        }
        writeHeapToBinary(heapArray, "records_b.dat");
        
        cout << count << " number of files read."<< endl;
        cout << "" << endl;      
        
        inputCountyFile.close();
    }
    
    void writeHeapToBinary(vector<countyData> heapVector, string fileName){
        ofstream outputCountyFile (fileName, ios::binary | ios::app);
        
        if (!outputCountyFile) {
            cout << "data file could not be opened" << endl;
            return;
        }
        
        int i = 1;
        while(i < recordNum){
            countyData mData =  heapVector[i];
            //cout << mData.city << mData.population << endl;
            outputCountyFile.write(reinterpret_cast<const char *>( &mData ),
                                   sizeof( countyData ));
            i++;
        }
        
        cout << "Heap A has inserted "<< i << " records" << endl;
        outputCountyFile.close();
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
    
    void dataToText ()
    {
        ifstream inAcctsFileA("records_a.dat", ios::binary);
        
        if (!inAcctsFileA)
        {
            cout << "binary file could not be opened" << endl;
            return;
        }
        
        ifstream inAcctsFileB("records_b.dat", ios::binary);
        
        if (!inAcctsFileB)
        {
            cout << "binary file could not be opened" << endl;
            return;
        }
        
        
        ofstream outAcctsFileA ("county_records_a.txt");
        
        if (!outAcctsFileA) {
            cout << "report file could not be opened" << endl;
            return;
        }
        
        ofstream outAcctsFileB ("county_records_b.txt");
        
        if (!outAcctsFileB) {
            cout << "report file could not be opened" << endl;
            return;
        }
        
        
        string city = "";
        int totalCount = 0;
        
        countyData dataA;
        int count = 1;
        
        // print out a header '\t' = tab character
        //outAcctsFile << "City: \t\tCounty: \t\tPopulation:" << endl << endl;
        
        // the first time
        inAcctsFileA.read(reinterpret_cast<char *> (&data), sizeof(countyData));
        
        while (inAcctsFileA && !inAcctsFileA.eof()  )
        {
            count++;
            outAcctsFileA << dataA.city << "\t" << dataA.county <<
			"\t" << dataA.population << endl;
            
			//lastItemData = data;
            
            // do it again and start the loop again
            inAcctsFileA.read(reinterpret_cast<char *> (&dataA), sizeof(countyData));
        }
        
        cout << "Read " << count << " number of cities from heap A." << endl;
        
        countyData dataB;
        totalCount += count;
        count = 1;
        
        // the first time
        inAcctsFileB.read(reinterpret_cast<char *> (&data), sizeof(countyData));
        
        while (inAcctsFileB && !inAcctsFileB.eof()  )
        {
            count++;
            outAcctsFileB << dataB.city << "\t" << dataB.county <<
			"\t" << dataB.population << endl;
            
			//lastItemData = data;
            
            // do it again and start the loop again
            inAcctsFileB.read(reinterpret_cast<char *> (&dataB), sizeof(countyData));
        }
        
        cout << "Read " << count << " number of cities from heap B." << endl;
        
        totalCount += count;
        cout << "Number of cities listed total: " << totalCount << endl;
        
    }
    
    void insertHeap( const countyData & x )
    {
        if( isHeapFull( ) ){
            return;
        }
        
        // Percolate up
        int hole = ++heapSize;
        for( ; hole > 1 && x < heapArray[ hole / 2 ]; hole /= 2 )
            heapArray[ hole ] = heapArray[ hole / 2 ];
        heapArray[ hole ] = x;
    }
    
    bool isHeapFull( ) const
    {
        return heapSize == heapArray.size( ) - 1;
    }
    
    bool isEmpty( ) const
    {
        return heapSize == 0;
    }
    
    void makeEmpty( )
    {
        heapSize = 0;
        heapArray = vector<countyData>(recordNum * sizeof(countyData));
    }
    
    void deleteHeapMin( )
    {
        if( isEmpty( ) ){
            return;
        }
        heapArray[ 1 ] = heapArray[ heapSize-- ];
        percolateDown( 1 );
    }
    
    void percolateDown( int hole )
    {
        int child;
        countyData tmp = heapArray[ hole ];
        
        for( ; hole * 2 <= heapSize; hole = child )
        {
            child = hole * 2;
            if( child != heapSize && heapArray[ child + 1 ] < heapArray[ child ] )
                child++;
            if( heapArray[ child ] < tmp )
                heapArray[ hole ] = heapArray[ child ];
            else{
                break;
            }
        }
        heapArray[ hole ] = tmp;
    }
    
    
    
private:
    
    countyData data;
    countyData hashData;
    vector<countyData> heapArray;
    int heapSize;
    int recordNum = 0;
    
    string cityBuffer;
    char countyBuffer[50];
    
    string strCity = "EMPTY_CITY_______________";
    string strCounty = "EMPTY_COUNTY_____________";
    
    char emptyCity[25];
    char emptyCounty[25];
};