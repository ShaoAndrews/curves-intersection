  /*
    输入表达式，将表达式转化为矩阵
 */
#ifndef _EXTOMAT_H_
#define _EXTOMAT_H_
#include"Global.h"
#include<Eigen/Dense>
class ExToMat
{
private:
    /* data */
public:
    ExToMat(const GiNaC::ex & f_);
    ~ExToMat();
public:
    Eigen::MatrixXd mat;
};

#endif
