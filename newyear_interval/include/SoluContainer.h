/*
 * 盛放根的容器，记录根并且每次加入新根的时候会进行判断
 * [1] 是否在某个唯一性区域内
 * [2] 是否跑到整个求解区域外面
 * 
 * 
 * 用途
 *      [1] 盛放根
 *      [2] 盛放根确定的唯一性区域
 *      [3] 判断给定区域是否在唯一性区域内部
 * 
*/

#pragma once
#include "Region.h"
#include "Point.h"
#include <vector>
class SoluContainer
{
private:
    //记录整个求解区域
    double m_xl;
    double m_xr;
    double m_yl;
    double m_yr;
    std::vector<Region> m_covered_region_list;                         //记录覆盖的区域
public:
    std::vector<Point> m_solu_list;                                    //记录所有的解

public:
    SoluContainer(double xl_, double xr_, double yl_, double yr_);
    //提交根以及其确定的唯一性区域，一旦发现提交的根在已经存在的唯一性区域内部，那么这个根将不会被保留，其确定的唯一性区域也会被丢弃
    void SubmitSoluAndCoveredRegion(const Point &solu, const Region &covered_region);
    //给定一个区域，判断是否被某个唯一性区域包含，如果包含返回true
    bool ContainsCurrentRegion(const Region &current_region);
    bool ContainsCurrentRegion(double xl, double xr, double yl, double yr);
    //是否含有当前解
    bool ContainsCurrentSolu(const Point &p);
    //找出当前点是否包含在唯一性区域内部
    //如果不包含则返回-1
    //如果包含的话返回对应的唯一性区域的解的指标
    int getSoluIndexByContainCurrentSolu(const Point &p);
    ~SoluContainer();
};
