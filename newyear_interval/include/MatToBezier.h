//从输入的csv文件中得到多项式的bezier表示矩阵
//其中csv文件类型应该如下
/*
 *1, 2, 4
 *2, 2, 0
 *3, 0, 0
 *具有上三角阵的形式，并且曲线的范围在[0, 1]*[0, 1]。
*/
#ifndef _MATTOBEZIER_H_
#define _MATTOBEZIER_H_
#include "Global.h"
#include <Eigen/Dense>
#include <vector>
#include "IntervalMatrix.h"
using namespace std;
class MatToBezier
{
private:
    int x_degree;
    int y_degree;

public:
    IntervalMatrix coffimat;
public:
    MatToBezier(const Eigen::MatrixXd &f_mat_);
    ~MatToBezier();
};

#endif