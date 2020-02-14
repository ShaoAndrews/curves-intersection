#include "NewKantorovich.h"

NewKantorovich::NewKantorovich(const GiNaC::ex &f1_, const GiNaC::ex &f2_, const GiNaC::ex &df1x_, const GiNaC::ex &df1y_, const GiNaC::ex &df2x_, const GiNaC::ex &df2y_) : m_f1(f1_),
                                                                                                                                                                             m_f2(f2_),
                                                                                                                                                                             m_df1x(df1x_),
                                                                                                                                                                             m_df1y(df1y_),
                                                                                                                                                                             m_df2x(df2x_),
                                                                                                                                                                             m_df2y(df2y_)
{
}

NewKantorovich::~NewKantorovich()
{
}
void NewKantorovich::submit(const Point &init_point, const Region &region, double M, bool is_square)
{
    is_singular = false;
    is_convergent = false;
    double eta;
    double beta;
    if (!iter_one_step(init_point, eta, beta))
    {
        //说明奇异
        is_singular = true;
    }
    else
    {
        //说明eta, beta, 计算完毕
        double two_over_M_beta = 2/(beta*M);
        h = eta * beta * M;
        if (h < 0.5)
        {
            is_convergent = true;
            rho2star = (1 + sqrt(1 - 2 * h)) * eta / h;
            //test
            double test = region.m_xr - region.m_xl;
            Region rho2star_region(init_point.rx - rho2star, init_point.rx + rho2star, init_point.ry - rho2star, init_point.ry + rho2star);
            //对两个集合取交
            covered_region.m_xl = rho2star_region.m_xl > region.m_xl ? rho2star_region.m_xl : region.m_xl;
            covered_region.m_xr = rho2star_region.m_xr < region.m_xr ? rho2star_region.m_xr : region.m_xr;
            covered_region.m_yl = rho2star_region.m_yl > region.m_yl ? rho2star_region.m_yl : region.m_yl;
            covered_region.m_yr = rho2star_region.m_yr < region.m_yr ? rho2star_region.m_yr : region.m_yr;
            if (!is_square)
            {
                //交集区域挖出最大的一个正方形
                double left = init_point.rx - covered_region.m_xl;
                double right = covered_region.m_xr - init_point.rx;
                double top = covered_region.m_yr - init_point.ry;
                double bottom = init_point.ry - covered_region.m_yl;
                double temp1, temp2, temp3;
                temp1 = left < right ? left : right;
                temp2 = top < bottom ? top : bottom;
                temp3 = temp1 < temp2 ? temp1 : temp2;
                covered_region.m_xl = init_point.rx - temp3;
                covered_region.m_xr = init_point.rx + temp3;
                covered_region.m_yl = init_point.ry - temp3;
                covered_region.m_yr = init_point.ry + temp3;
            }
        }
    }
}
bool NewKantorovich::iter_one_step(const Point &x0, double &eta_, double &beta_)
{
    bool status = true;
    double df1_value_x, df1_value_y;
    double df2_value_x, df2_value_y;
    double matrix_det;
    df1_value_x = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_df1x.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
    df1_value_y = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_df1y.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
    df2_value_x = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_df2x.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
    df2_value_y = GiNaC::ex_to<GiNaC::numeric>(GiNaC::evalf(m_df2y.subs(GiNaC::lst{x == x0.rx, y == x0.ry}))).to_double();
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
        eta_ = (fabs(temp1) > fabs(temp2)) ? fabs(temp1) : fabs(temp2);
        double tempbeta1, tempbeta2;
        tempbeta1 = fabs(inv_a11) + fabs(inv_a12);
        tempbeta2 = fabs(inv_a21) + fabs(inv_a22);
        beta_ = tempbeta1 > tempbeta2 ? tempbeta1 : tempbeta2;
    }
    return status;
}