#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "EState.h"
#include "Point.h"

using std::cout;
using std::endl;
using std::sort;
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

bool comp(const Point &a, const Point &b)
{
    return (a.gValue + a.hValue) > (b.gValue + b.hValue);
}

Point GetNextPoint(vector<Point> &openList)
{
    sort(openList.begin(), openList.end(), comp);
    Point nextPoint = openList.back();
    openList.pop_back();
    return nextPoint;
}

int CalculateHValue(const int endY, const int endX, const int startY, const int startX)
{
    return abs(endY - startY) + abs(endX - startX);
}

void AddNeighbors(const vector<vector<EState>> &map, vector<Point> &openList, const Point &current, const int endY, const int endX)
{
    constexpr int delta[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    for (int i = 0; i < 4; i++)
    {
        int nextY = current.y - delta[i][0];
        int nextX = current.x - delta[i][1];
        if (nextY >= 0 && nextY < map.size() &&
            nextX >= 0 && nextX < map.back().size() &&
            map[nextY][nextX] == EState::Empty)
        {
            Point point;
            point.parent = &current;
            point.y = nextY;
            point.x = nextX;
            point.gValue = current.gValue + 1;
            point.hValue = CalculateHValue(endY, endX, current.y, current.x);
            openList.push_back(point);
        }
    }
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

    const int END_Y = originalMap.size() - 1;
    const int END_X = originalMap.back().size() - 1;

    Point startPoint;
    startPoint.parent = nullptr;
    startPoint.y = 0;
    startPoint.x = 0;
    startPoint.gValue = 0;
    startPoint.hValue = END_Y + END_X;

    vector<Point> openList{startPoint};
    vector<vector<int>> allPath;
    while (openList.size() > 0)
    {
        Point nextPoint = GetNextPoint(openList);
        parsedMap[nextPoint.y][nextPoint.x] = EState::Visited;
        allPath.push_back(vector<int>{nextPoint.y, nextPoint.x});
        if (nextPoint.y == END_Y && nextPoint.x == END_X)
        {
            cout << "Finish!" << endl;
            break;
        }

        AddNeighbors(parsedMap, openList, nextPoint, END_Y, END_X);
    }

    vector<vector<int>> answer{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {3, 2}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 6}, {4, 6}};
    assert(allPath == answer);
}