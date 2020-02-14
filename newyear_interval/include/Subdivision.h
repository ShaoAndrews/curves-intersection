#ifndef _SUBDIVISION_H_
#define _SUBDIVISION_H_
#include "IntervalMatrix.h"
#include <iostream>
using namespace std;
/*
 *根据Bernstein系数矩阵得到四个子区域的Bernstein系数
 *采用de casteljau 分割算法来进行计算
 *@para: Eigen::MatrixXd source_matrix  输入的系数矩阵
 *@para: int numRows                    输入的矩阵的行数
 *@para: int numCols                    输入的矩阵的列数

 *return:
    variables:
        Eigen::MatrixXd child1_matrix   第一象限的系数矩阵
        Eigen::MatrixXd child2_matrix   第二象限的系数矩阵
        Eigen::MatrixXd child3_matrix   第三象限的系数矩阵
        Eigen::MatrixXd child4_matrix   第四象限的系数矩阵
*/
class Subdivision
{
private:
    /* data */
    int x_num;
    int y_num;
    IntervalMatrix source_matrix;

public:
    IntervalMatrix child1_matrix;
    IntervalMatrix child2_matrix;
    IntervalMatrix child3_matrix;
    IntervalMatrix child4_matrix;

public:
    Subdivision(const IntervalMatrix &source_matrix_);
    ~Subdivision();

private:
    //按x方向切分成两部分，变成[0, 1/2]*[0, 1]和[1/2, 1]*[0, 1]
    void divide_by_x(IntervalMatrix &source, IntervalMatrix &left_mat, IntervalMatrix &right_mat, int &numRows);
    //按y方向切分成两部分，变成[0, 1]*[0, 1/2]和[0, 1]*[1/2, 1]
    void divide_by_y(IntervalMatrix &source, IntervalMatrix &top_mat, IntervalMatrix &bottom_mat, int &numCols);
};

#endif