#pragma once

struct Point
{
    const Point* parent;
    int y;
    int x;
    int gValue;
    int hValue;
};

// class Point
// {
// public:
//     Point(const int y, const int x, const int gVal, const int hVal);
//     int GetY() const { return mY; }
//     int GetX() const { return mX; }
//     int GetGValue() const { return mGValue; }
//     int GetHValue() const { return mHValue; }
//     Point *GetParent() const { return parent; }

// private:
//     Point *parent;
//     int mY;
//     int mX;
//     int mGValue;
//     int mHValue;
// };