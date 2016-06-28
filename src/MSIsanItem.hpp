//
//  MSIsanItem.hpp
//  BMATMediaDataExtractor
//
//  Created by Miquel Àngel Soler on 28/6/16.
//
//

#ifndef MSIsanItem_hpp
#define MSIsanItem_hpp

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class MSIsanItem
{
public:
    vector<string> titles;
    string ID;

    void print()
    {
        cout << "ID: " << ID << endl;
        for (int i=0; i<titles.size(); ++i) {
            cout << "Title: " << titles[i] << endl;
        }
    }
};

#endif /* MSIsanItem_hpp */
