#include <iostream>
#include <vector>

#include "EState.h"

using std::cout;
using std::endl;
using std::vector;

vector<vector<EState>> ParseIntMapToEState(const vector<vector<int>> &map)
{
    vector<vector<EState>> parsedMap(map.size(), vector<EState>(map.front().size(), EState::Empty));
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == 1)
            {
                parsedMap[i][j] = EState::Obstacle;
            }
        }
    }
    return parsedMap;
}

int main()
{
    vector<vector<int>> originalMap{
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 1, 0}};

    vector<vector<EState>> parsedMap = ParseIntMapToEState(originalMap);

    for (auto v : parsedMap)
    {
        for (auto n : v)
        {
            cout << int(n) << " ";
        }
        cout << endl;
    }
}