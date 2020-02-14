/*
    based on 
    [1] http://www.page.sannet.ne.jp/d_takahashi/boost/ublas/index.html                  boost matrix
    [2] http://verifiedby.me/kv/kv-intro-e.pdf                                           interval arithmetic

*/

#pragma once
#include <boost/numeric/ublas/matrix.hpp>
#include "kv/interval.hpp"
#include "kv/rdouble.hpp"
#include <assert.h>
#include "Global.h"
typedef kv::interval<double> interval;
typedef boost::numeric::ublas::matrix<interval> dmatrix;

class IntervalMatrix
{
public:
    dmatrix mymat;

public:
    IntervalMatrix(int rows, int cols);
    IntervalMatrix(const IntervalMatrix &src);
    bool notcontains(double value);
    std::pair<double, double> MumValue(); //得到最值点，返回的是pair<double, double> make_pair(maximum, minimum);
    kv::interval<double> &operator()(int a, int b);
    void print(); //just for print;
    int rows();
    int cols();
    ~IntervalMatrix();

public:
    const int m_rows;
    const int m_cols;
};
