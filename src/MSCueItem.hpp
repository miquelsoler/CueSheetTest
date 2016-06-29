//
//  MSCueItem.hpp
//  BMATMediaDataExtractor
//
//  Created by Miquel Àngel Soler on 29/6/16.
//
//

#ifndef MSCueItem_hpp
#define MSCueItem_hpp

#include <string>
#include <iostream>

using namespace std;

class MSCueItem
{
public:
    string ID;
    string title;

    void print()
    {
        cout << "ID: " << ID << " - Title:" << title;
    }
};

#endif /* MSCueItem_hpp */
