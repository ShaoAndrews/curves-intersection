#pragma once
#include "MatToString.h"

class MatToStringPythonFormat : public MatToString
{
public:
    virtual string get_expression();
    /* data */
public:
    MatToStringPythonFormat(const Eigen::MatrixXd &f_mat);
    ~MatToStringPythonFormat();
};
