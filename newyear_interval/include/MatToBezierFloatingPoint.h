//从输入的csv文件中得到多项式的bezier表示矩阵
//其中csv文件类型应该如下
/*
 *1, 2, 4
 *2, 2, 0
 *3, 0, 0
 *具有上三角阵的形式，并且曲线的范围在[0, 1]*[0, 1]。
 使用浮点小数计算
*/
#pragma once
#include "Global.h"
#include <Eigen/Dense>
#include <vector>
using namespace std;
class MatToBezierFloatingPoint
{
private:
    int x_degree;
    int y_degree;

public:
    Eigen::MatrixXd coffimat;
public:
    MatToBezierFloatingPoint(const Eigen::MatrixXd &f_mat_);
    ~MatToBezierFloatingPoint();
};
