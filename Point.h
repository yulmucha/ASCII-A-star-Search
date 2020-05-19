#pragma once

class Point
{
public:
    Point(const int y, const int x, const int gVal, const int hVal);
    Point(const Point* parent, const int y, const int x, const int gVal, const int hVal);
    int GetY() const { return mY; }
    int GetX() const { return mX; }
    int GetGVal() const { return mGValue; }
    int GetHVal() const { return mHValue; }
    const Point* GetParent() const { return mParent; }

private:
    const Point* mParent;
    const int mY;
    const int mX;
    const int mGValue;
    const int mHValue;
};

Point::Point(const int y, const int x, const int gVal, const int hVal)
    : mParent(nullptr)
    , mY(y)
    , mX(x)
    , mGValue(gVal)
    , mHValue(hVal)
{
}

Point::Point(const Point* parent, const int y, const int x, const int gVal, const int hVal)
    : mParent(parent)
    , mY(y)
    , mX(x)
    , mGValue(gVal)
    , mHValue(hVal)
{
}