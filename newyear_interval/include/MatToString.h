#ifndef _MAT_TO_STRING_
#define _MAT_TO_STRING_
#include <Eigen/Dense>
#include <string>
using namespace std;
class MatToString
{
protected:
    Eigen::MatrixXd m_f_mat;
public:
    virtual string get_expression() = 0;

public:
    MatToString(const Eigen::MatrixXd &f_mat);
    virtual ~MatToString();
};

#endif