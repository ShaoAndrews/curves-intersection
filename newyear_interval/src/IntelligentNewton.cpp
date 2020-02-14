#include "IntelligentNewton.h"

IntelligentNewton::IntelligentNewton(const GiNaC::ex &f1, const GiNaC::ex &f2, const GiNaC::ex &df1x, const GiNaC::ex &df1y, const GiNaC::ex &df2x, const GiNaC::ex &df2y, double precision) : m_f1(f1),
                                                                                                                                                                                               m_f2(f2),
                                                                                                                                                                                               m_df1_x(df1x),
                                                                                                                                                                                               m_df1_y(df1y),
                                                                                                                                                                                               m_df2_x(df2x),
                                                                                                                                                                                               m_df2_y(df2y),
                                                                                                                                                                                               m_precision(precision)
{
}

void IntelligentNewton::submit(const vector<Region> &region_list, const Point &init_point)
{
    finish_iter = false;
    Point x0 = init_point;
    Point x1(0, 0);
    double cur_eta = 0;
    //开始迭代一步得到x1和eta
    if (iter_one_step(x0, x1, cur_eta) && is_containedByRegionList(x1, region_list))
    {
        if (cur_eta < m_precision)
        {
            //说明已经小于给定的精度了，直接将x1拿出来作为解来判断就好了
            finish_iter = true;
            guess_solu = x1;
        }
        else
        {
            //说明还没有达到给定的精度，需要迭代
            bool stop_signal = false;
            while (true)
            {

                //将上一步的x1和eta记录下来
                x0 = x1;
                double last_eta = cur_eta;
                if (iter_one_step(x0, x1, cur_eta) && is_containedByRegionList(x1, region_list))
                {
                    if (cur_eta < m_precision)
                    {
                        //说明已经达到了精度，无需再迭代了
                        guess_solu = x1;
                        finish_iter = true;
                        break;
                    }
                    else
                    {

                        if (cur_eta >= 0.5 * last_eta)
                        {
                            //说明没有收敛的趋势
                            finish_iter = false;
                            break;
                        }
                    }
                }
                else
                {
                    //说明这步迭代出了问题，直接终止
                    finish_iter = false;
                    break;
                }
            }
        }
    }
    else
    {
        finish_iter = false; //没有完车任务
    }
}
void IntelligentNewton::submit(const Point &init_point)
{
    finish_iter = false;
    Point x0 = init_point;
    Point x1(0, 0);
    double cur_eta = 0;
    //开始迭代一步得到x1和eta
    if (iter_one_step(x0, x1, cur_eta))
    {
        if (cur_eta < m_precision)
        {
            //说明已经小于给定的精度了，直接将x1拿出来作为解来判断就好了
            finish_iter = true;
            guess_solu = x1;
        }
        else
        {
            //说明还没有达到给定的精度，需要迭代
            bool stop_signal = false;
            while (true)
            {

                //将上一步的x1和eta记录下来
                x0 = x1;
                double last_eta = cur_eta;
                if (iter_one_step(x0, x1, cur_eta))
                {
                    if (cur_eta < m_precision)
                    {
                        //说明已经达到了精度，无需再迭代了
                        guess_solu = x1;
                        finish_iter = true;
                        break;
                    }
                    else
                    {

                        if (cur_eta >= 0.5 * last_eta)
                        {
                            //说明没有收敛的趋势
                            finish_iter = false;
                            break;
                        }
                    }
                }
                else
                {
                    //说明这步迭代出了问题，直接终止
                    finish_iter = false;
                    break;
                }
            }
        }
    }
    else
    {
        finish_iter = false; //没有完车任务
    }
}
IntelligentNewton::~IntelligentNewton()
{
}
bool IntelligentNewton::iter_one_step(Point &x0, Point &p1, double &eta)
{
    bool status = true;
    double df1_value_x, df1_value_y;
    double df2_value_x, df2_value_y;
    double matrix_det;
    df1_value_x = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_df1_x.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
    df1_value_y = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_df1_y.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
    df2_value_x = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_df2_x.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
    df2_value_y = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_df2_y.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
    matrix_det = df1_value_x * df2_value_y - df1_value_y * df2_value_x;
    if (fabs(matrix_det) < PRECISION)
    {
        //此时矩阵奇异
        status = false;
    }
    else
    {
        //此时矩阵非奇异
        //计算eta
        double inv_a11, inv_a12, inv_a21, inv_a22;
        inv_a11 = df2_value_y / matrix_det;
        inv_a12 = -df1_value_y / matrix_det;
        inv_a21 = -df2_value_x / matrix_det;
        inv_a22 = df1_value_x / matrix_det;
        double f1_value, f2_value;
        f1_value = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_f1.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
        f2_value = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_f2.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
        double temp1, temp2;
        temp1 = inv_a11 * f1_value + inv_a12 * f2_value;
        temp2 = inv_a21 * f1_value + inv_a22 * f2_value;
        //两者取大计算eta的值
        eta = (fabs(temp1) > fabs(temp2)) ? fabs(temp1) : fabs(temp2);
        p1.rx = x0.rx - temp1;
        p1.ry = x0.ry - temp2;
    }
    return status;
}
bool IntelligentNewton::is_containedByRegionList(const Point &p, const std::vector<Region> &region_list)
{
    if (!region_list.empty())
    {
        bool is_contained = false;
        for (std::vector<Region>::const_iterator iter = region_list.begin(); iter != region_list.end(); ++iter)
        {
            if (region_contains_point(*iter, p))
            {
                is_contained = true;
                break;
            }
        }
        return is_contained;
    }
    else
    {
        return false;
    }
}
bool IntelligentNewton::region_contains_point(const Region &region, const Point &p)
{
    bool status = false;
    if (p.rx >= region.m_xl && p.rx <= region.m_xr && p.ry >= region.m_yl && p.ry <= region.m_yr)
    {
        status = true;
    }
    return status;
}