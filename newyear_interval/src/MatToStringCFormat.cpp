#include "MatToStringCFormat.h"

MatToStringCFormat::MatToStringCFormat(const Eigen::MatrixXd &f_mat) : MatToString(f_mat)
{
}

MatToStringCFormat::~MatToStringCFormat()
{
}
string MatToStringCFormat::get_expression()
{
    int rows = m_f_mat.rows();
    int cols = m_f_mat.cols();

    string f = "";
    int is_positive = -1; //记录第一个不为零的数是正的还是负的，正的为１负的为0
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (m_f_mat(i, j) != 0)
            {
                if (m_f_mat(i, j) > 0)
                {
                    f += "+";
                    if (is_positive < 0)
                    {
                        is_positive = 1;
                    }
                }
                else
                {
                    if (is_positive < 0)
                    {
                        is_positive = 0;
                    }
                }
                f += to_string(int(m_f_mat(i, j)));
                if (i != 0)
                {
                    f += "*x^";
                    f += to_string(i);
                }
                if (j != 0)
                {
                    f += "*y^";
                    f += to_string(j);
                }
            }
        }
    }

    if (is_positive > 0)
    {
        string add_string = "+";
        int length = strlen(add_string.c_str());
        f = f.substr(length);
    }
    return f;
}