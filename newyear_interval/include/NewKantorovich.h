/*
 * 新的kantorovich方法，只对点进行判断
 * 我们会给出区域内的判断点以及区域大小以及关于该区域M的上界估计
 * 需要给出以下信息
 *      [1] is_singular        在当前点是否是奇异的
 *      [2] is_convergent      是否是收敛的
 *      [3] 如果收敛给出唯一性区域是哪一块
*/
#pragma once
#include "Global.h"
#include "Point.h"
#include "Region.h"
#include <assert.h>
class NewKantorovich
{
private:
    GiNaC::ex m_f1;
    GiNaC::ex m_f2;
    GiNaC::ex m_df1x;
    GiNaC::ex m_df1y;
    GiNaC::ex m_df2x;
    GiNaC::ex m_df2y;

    bool iter_one_step(const Point &x0, double &eta_, double &beta_);

public:
    NewKantorovich(const GiNaC::ex &f1_, const GiNaC::ex &f2_, const GiNaC::ex &df1x_, const GiNaC::ex &df1y_, const GiNaC::ex &df2x_, const GiNaC::ex &df2y_);
    void submit(const Point &init_point, const Region &region, double M, bool is_square = true); //用于提交任务
    ~NewKantorovich();
    bool is_singular;
    bool is_convergent;
    Region covered_region;
    double rho2star;
    double h;
};
