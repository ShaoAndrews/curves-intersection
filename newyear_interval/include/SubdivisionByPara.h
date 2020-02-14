/*
    对于全局矩阵来说，每一列对应的是x方向的bezier系数
                　　每一行对应的是y方向的bezier系数
    通过全局大区域上的bezier系数来计算局部小区域上的bezier系数
    需要给定全局矩阵，(top_x, bottom_x, left_y, right_y)
    假设大区域范围为[xl, xr]*[yl, yr]
    当前区域的范围为[c_xl, c_xr]*[c_yl, c_yr]
    则
        top_x = (c_xl - xl)/(xr - xl)
        bottom_x = (c_xr - c_xl)/(xr - c_xl)
        left_y = (c_yl - yl)/(yr - yl)
        right_y = (c_yr - c_yl)/(yr - c_yl)
*/
#pragma once
#include <Eigen/Dense>
#include "Global.h"
#include "ExToMat.h"
#include "Region.h"
#include "MatToBezierFloatingPoint.h"
using namespace std;
class SubdivisionByPara
{
private:
    const double g_xl;
    const double g_xr;
    const double g_yl;
    const double g_yr;
    Eigen::MatrixXd g_mat1;
    Eigen::MatrixXd g_mat2;
    double M;
    void SubdivisionByX(Eigen::MatrixXd &source, Eigen::MatrixXd &left_mat, Eigen::MatrixXd &right_mat, int &numRows, double rate);
    void SubdivisionByY(Eigen::MatrixXd &source, Eigen::MatrixXd &top_mat, Eigen::MatrixXd &bottom_mat, int &numCols, double rate);

public:
    SubdivisionByPara(const GiNaC::ex &f1, const GiNaC::ex &f2, double g_xl_, double g_xr_, double g_yl_, double g_yr_);
    void submit(double c_xl, double c_xr, double c_yl, double c_yr);
    void submit(const Region &region);
    double getMaxValue();
    ~SubdivisionByPara();
};
