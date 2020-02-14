#pragma once
#include "Global.h"
#include "Point.h"
#include "Node.h"
#include "MatToEx.h"
#include "MatToBezier.h"
#include <vector>
#include <Eigen/Dense>
#include "ExToMat.h"
#include "MatToStringCFormat.h"
class Algorithm
{
protected:
    GiNaC::ex m_f1, m_f2;
    double m_xl, m_xr, m_yl, m_yr;
    double m_precision;
    int m_f1_x_deg;
    int m_f1_y_deg;
    int m_f2_x_deg;
    int m_f2_y_deg;
    GiNaC::ex m_df1_x;
    GiNaC::ex m_df1_y;
    GiNaC::ex m_df2_x;
    GiNaC::ex m_df2_y;
    GiNaC::ex m_hessian_f1;
    GiNaC::ex m_hessian_f2;
    string real_hessian_f1;
    string real_hessian_f2;

    string df1_xx_str;
    string df1_xy_str;

    string df1_yy_str;

    string df2_xx_str;

    string df2_xy_str;

    string df2_yy_str;

    string hessian_f1_str;
    string hessian_f2_str;

public:
    Algorithm(const Eigen::MatrixXd &f1_mat, const Eigen::MatrixXd &f2_mat, double precision, double xl, double xr, double yl, double yr);
    virtual ~Algorithm();
};
