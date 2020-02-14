#include "MatToBezier.h"
MatToBezier::MatToBezier(const Eigen::MatrixXd &f_mat_) : coffimat(f_mat_.rows(), f_mat_.cols())
{

    x_degree = f_mat_.rows() - 1;
    y_degree = f_mat_.cols() - 1;
    //将每一列系数初始化，除以一个组合数, i表示第i列，j表示第j行
    for (int i = 0; i <= y_degree; ++i)
    {
        for (int j = 0; j <= x_degree; ++j)
        {
            kv::interval<double> up, down;
            up = f_mat_(j, i);
            down = (GiNaC::ex_to<GiNaC::numeric>(GiNaC::binomial(x_degree, j)).to_double());
            coffimat(j, i) = up / down;
        }
    }  
    for (int j = 0; j <= y_degree; ++j)
    {
        int count = 0;
        kv::interval<double> temp[x_degree + 1];
        while (count <= x_degree)
        {
            temp[count] = coffimat(0, j);
            for (int i = 0; i < x_degree - count; ++i)
            {
                assert(i < f_mat_.rows() && j <f_mat_.cols());
                coffimat(i, j) = coffimat(i, j) + coffimat(i + 1, j);
            }
            count++;
        }
        for (int i = 0; i <= x_degree; ++i)
        {
            kv::interval<double> down;
            down = GiNaC::ex_to<GiNaC::numeric>(GiNaC::binomial(y_degree, j)).to_double();
            coffimat(i, j) = temp[i] / down;
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

MatToBezier::~MatToBezier()
{
}