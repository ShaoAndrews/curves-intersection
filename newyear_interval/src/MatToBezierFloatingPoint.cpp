#include "MatToBezierFloatingPoint.h"
MatToBezierFloatingPoint::MatToBezierFloatingPoint(const Eigen::MatrixXd &f_mat_)
{
    x_degree = f_mat_.rows() - 1;
    y_degree = f_mat_.cols() - 1;
    //初始化矩阵
    coffimat = Eigen::MatrixXd::Zero(x_degree + 1, y_degree + 1);
    //将每一列系数初始化，除以一个组合数, i表示第i列，j表示第j行
    for (int i = 0; i <= y_degree; ++i)
    {
        for (int j = 0; j <= x_degree; ++j)
        {
            coffimat(j, i) = f_mat_(j, i) / (GiNaC::ex_to<GiNaC::numeric>(GiNaC::binomial(x_degree, j)).to_double());
        }
    }
    for (int j = 0; j <= y_degree; ++j)
    {
        int count = 0;
        Eigen::VectorXd temp(x_degree + 1);
        while (count <= x_degree)
        {
            temp[count] = coffimat(0, j);
            for (int i = 0; i < x_degree - count; ++i)
            {
                coffimat(i, j) = coffimat(i, j) + coffimat(i + 1, j);
            }
            count++;
        }
        for (int i = 0; i <= x_degree; ++i)
        {
            coffimat(i, j) = temp(i) / GiNaC::ex_to<GiNaC::numeric>(GiNaC::binomial(y_degree, j)).to_double();
        }
    }
    for (int i = 0; i <= x_degree; ++i)
    {
        int count = 0;
        while (count < y_degree)
        {
            for (int j = y_degree; j > count; --j)
            {
                coffimat(i, j) = coffimat(i, j) + coffimat(i, j - 1);
            }
            count++;
        }
    }
}

MatToBezierFloatingPoint::~MatToBezierFloatingPoint()
{
}