#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "EState.h"
#include "Point.h"

using std::cout;
using std::endl;
using std::sort;
using std::string;
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

bool comp(const Point *a, const Point *b)
{
    int fVal1 = a->GetGValue() + a->GetHValue();
    int fVal2 = b->GetGValue() + b->GetHValue();
    return fVal1 > fVal2;
}

Point *GetNextPoint(vector<Point *> &openList)
{
    sort(openList.begin(), openList.end(), comp);
    Point *nextPoint = openList.back();
    openList.pop_back();
    return nextPoint;
}

int CalculateHValue(const int endY, const int endX, const int startY, const int startX)
{
    return abs(endY - startY) + abs(endX - startX);
}

void AddNeighbors(const vector<vector<EState>> &map, vector<Point *> &points, vector<Point *> &openList, const Point *current, const int endY, const int endX)
{
    constexpr int delta[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    for (int i = 0; i < 4; i++)
    {
        int nextY = current->GetY() - delta[i][0];
        int nextX = current->GetX() - delta[i][1];
        if (nextY >= 0 && nextY < map.size() &&
            nextX >= 0 && nextX < map.back().size() &&
            map[nextY][nextX] == EState::Empty)
        {
            Point *point = new Point(
                current,
                nextY,
                nextX,
                current->GetGValue() + 1,
                CalculateHValue(endY, endX, current->GetY(), current->GetX()));
            points.push_back(point);
            openList.push_back(points.back());
        }
    }
}

void MarkFinalPath(vector<vector<EState>> &map, const Point *finish)
{
    const Point *point = finish;
    while (point->GetParent() != nullptr)
    {
        map[point->GetY()][point->GetX()] = EState::Path;
        point = point->GetParent();
    }
    map[point->GetY()][point->GetX()] = EState::Starting;
    map[finish->GetY()][finish->GetX()] = EState::Finish;
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

    vector<Point *> points;
    Point *startPoint = new Point(0, 0, 0, END_Y + END_X);
    points.push_back(startPoint);

    vector<Point *> openList{points.back()};
    while (openList.size() > 0)
    {
        Point *nextPoint = GetNextPoint(openList);
        parsedMap[nextPoint->GetY()][nextPoint->GetX()] = EState::Visited;

        if (nextPoint->GetY() == END_Y && nextPoint->GetX() == END_X)
        {
            MarkFinalPath(parsedMap, nextPoint);
            break;
        }

        AddNeighbors(parsedMap, points, openList, nextPoint, END_Y, END_X);
    }

    for (auto v : parsedMap)
    {
        for (auto state : v)
        {
            cout << int(state) << " ";
        }
        cout << endl;
    }

    for (Point *p : points)
    {
        delete p;
    }
}