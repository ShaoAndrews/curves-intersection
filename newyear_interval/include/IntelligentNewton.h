/*
 * 对原来的牛顿法进行修正
 * 我们输入当前的连通区域以及需要满足的精度，所以当每次迭代的残差小于给定的精度的时候，那么精确解将不会跑出这个精度之外
 * 我们需要每次验证雅克比行列式是否奇异，以及本次残查是否比上一次衰减至少二分之一
*/

#pragma once
#include "Region.h"
#include "Point.h"
#include "Global.h"
#include <vector>
#include <assert.h>
using namespace std;
class IntelligentNewton
{
private:
    GiNaC::ex m_f1;
    GiNaC::ex m_f2;
    GiNaC::ex m_df1_x;
    GiNaC::ex m_df1_y;
    GiNaC::ex m_df2_x;
    GiNaC::ex m_df2_y;
    double m_precision;
    bool iter_one_step(Point &x0, Point &p1, double &eta);
    //判断当前点是否在这块区域内部
    bool is_containedByRegionList(const Point &p, const std::vector<Region> &region_list);
    bool region_contains_point(const Region& region, const Point&p);
public:
    bool finish_iter;
    Point guess_solu; //newton预测的解，并且满足给定的精度，需要使用newton-kantorovich进行精确判断

public:
    IntelligentNewton(const GiNaC::ex &f1, const GiNaC::ex &f2, const GiNaC::ex &df1x, const GiNaC::ex &df1y, const GiNaC::ex &df2x, const GiNaC::ex &df2y, double precision);
    //需要保证迭代的每一步点都不能出现在region_list内
    void submit(const vector<Region> &region_list, const Point &init_point);
    //只要每一次衰减满足0.5并且不奇异就迭代
    void submit(const Point &init_point);
    ~IntelligentNewton();
};
