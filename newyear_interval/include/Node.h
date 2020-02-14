#pragma once
#include <string>
#include "Point.h"
#include "Region.h"
using namespace std;
class Node
{

public:
    Node(const string &id, double xl, double xr, double yl, double yr);
    virtual ~Node();

public:
    const Point m_center;                                  //中心点
    const string m_id;                                     //id
    Region m_region;
    int m_bezier;
    int m_resultant;
    int m_newton;
    const double m_size;
};
