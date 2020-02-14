#pragma once
#include "Global.h"
#include <math.h>
#include<vector>
#include "Region.h"
class Point
{
  public:
    /* data */
    double rx;
    double ry;

  public:
    Point();
    Point(double rx, double ry);
    Point(const Point &p);
    Point operator+(const Point &b);
    double max_norm(const Point&p);
    double get_distance(const Point &p);
    ~Point();
};