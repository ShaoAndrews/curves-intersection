#pragma once
#include "MatToString.h"
#include "Global.h"

class MatToStringCFormat : public MatToString
{
public:
    virtual string get_expression();
public:
    MatToStringCFormat(const Eigen::MatrixXd &f_mat);
    ~MatToStringCFormat();
};

