#include "MatToEx.h"
MatToEx::MatToEx(const Eigen::MatrixXd &f_mat)
{
    int rows = f_mat.rows();
    int cols = f_mat.cols();
    f = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            f += f_mat(i, j)*GiNaC::pow(x, i)*GiNaC::pow(y, j);
        }
    }
}
MatToEx::~MatToEx()
{
}