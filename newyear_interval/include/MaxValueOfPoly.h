/*
 *　使用区间计算来估计多项式在某个区间的范围

*/
#pragma once
#include "Global.h"
#include "kv/interval.hpp"
#include "kv/rdouble.hpp"
class MaxValueOfPoly
{
private:
    /* data */
public:
    kv::interval<double> image;

public:
    MaxValueOfPoly(const GiNaC::ex &f, const kv::interval<double> &x_region, const kv::interval<double> &y_region);
    ~MaxValueOfPoly();
};
