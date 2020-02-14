//记录每块区域

#pragma once
#include "Point.h"
class Region
{
public:
    /* data */
    double m_xl;
    double m_xr;
    double m_yl;
    double m_yr;
    int index;

public:
    Region();
    Region(double xl_, double xr_, double yl_, double yr_);
    ~Region();

    //判断是否是target的子集
    bool is_subsetof(const Region &target) const;
    //检查是否与target有公共部分，如果有公共部分返回tre，没有公共部分返回false
    bool checkIntersect(const Region &target) const;
};
