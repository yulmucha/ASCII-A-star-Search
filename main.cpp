#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>

#include "EState.h"
#include "Point.h"

using std::cout;
using std::endl;
using std::sort;
using std::string;
using std::vector;

vector<vector<int>> ReadMapFile(const string &path)
{
    vector<vector<int>> map;
    string line;
    std::ifstream filestream(path);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            vector<int> row;
            int n;
            char c;
            std::istringstream linestream(line);
            while (linestream >> n >> c && c == ',')
            {
                if (n == 0)
                {
                    row.push_back(0);
                }
                else
                {
                    row.push_back(1);
                }
            }
            map.push_back(row);
        }
    }
    return map;
}

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

bool comp(const std::shared_ptr<Point> a, const std::shared_ptr<Point> b)
{
    int fVal1 = a->GetGVal() + a->GetHVal();
    int fVal2 = b->GetGVal() + b->GetHVal();
    return fVal1 > fVal2;
}

std::shared_ptr<Point> GetNextPoint(vector<std::shared_ptr<Point>> &openList)
{
    sort(openList.begin(), openList.end(), comp);
    auto nextPoint = openList.back();
    openList.pop_back();
    return nextPoint;
}

int CalculateHValue(const int endY, const int endX, const int startY, const int startX)
{
    return abs(endY - startY) + abs(endX - startX);
}

void AddNeighbors(const vector<vector<EState>> &map, vector<std::shared_ptr<Point>> &openList, const std::shared_ptr<Point> current, const int endY, const int endX)
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
            auto point = std::make_shared<Point>(
                current,
                nextY,
                nextX,
                current->GetGVal() + 1,
                CalculateHValue(endY, endX, current->GetY(), current->GetX()));
            openList.push_back(point);
        }
    }
}

void MarkFinalPath(vector<vector<EState>> &map, const Point *finish)
{
    auto tempPoint = finish;
    while (tempPoint->GetParent() != nullptr)
    {
        map[tempPoint->GetY()][tempPoint->GetX()] = EState::Path;
        tempPoint = tempPoint->GetParent().get();
    }
    map[tempPoint->GetY()][tempPoint->GetX()] = EState::Starting;
    map[finish->GetY()][finish->GetX()] = EState::Finish;
}

void PrintMap(const vector<vector<EState>> &map)
{
    SetConsoleOutputCP(CP_UTF8);
    for (auto &v : map)
    {
        for (auto state : v)
        {
            string s;
            switch (state)
            {
            case EState::Obstacle:
                s = "🗻";
                break;
            case EState::Path:
                s = "🚑";
                break;
            case EState::Starting:
                s = "🚦";
                break;
            case EState::Finish:
                s = "🏥";
                break;
            default:
                s = "🔲";
                break;
            }
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    // vector<vector<int>> originalMap{
    //     {0, 1, 0, 0, 0, 0, 0},
    //     {0, 1, 0, 0, 0, 0, 0},
    //     {0, 1, 0, 0, 0, 0, 0},
    //     {0, 1, 0, 1, 1, 1, 0},
    //     {0, 0, 0, 0, 0, 1, 0}};

    auto originalMap = ReadMapFile("map");

    vector<vector<EState>> parsedMap = ParseIntMapToEState(originalMap);

    constexpr int START_Y = 0;
    constexpr int START_X = 0;
    const int END_Y = originalMap.size() - 1;
    const int END_X = originalMap.back().size() - 1;

    auto startPoint = std::make_shared<Point>(
        START_Y,
        START_X,
        0,              // G-Value
        END_Y + END_X); // H-Value

    vector<std::shared_ptr<Point>> openList{startPoint};
    while (openList.size() > 0)
    {
        auto nextPoint = GetNextPoint(openList);
        parsedMap[nextPoint->GetY()][nextPoint->GetX()] = EState::Visited;

        if (nextPoint->GetY() == END_Y && nextPoint->GetX() == END_X)
        {
            MarkFinalPath(parsedMap, nextPoint.get());
            break;
        }

        AddNeighbors(parsedMap, openList, nextPoint, END_Y, END_X);
    }

    PrintMap(parsedMap);
}