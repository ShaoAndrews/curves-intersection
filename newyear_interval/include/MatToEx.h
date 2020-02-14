/*
 *convert matrix into expression
 * 
*/
#ifndef _MATTOEX_H_
#define _MATTOEX_H_
#include "Global.h"
#include <Eigen/Dense>
class MatToEx
{
public:
    GiNaC::ex f;
public:
    MatToEx(const Eigen::MatrixXd &f_mat);
    ~MatToEx();
};

#endif