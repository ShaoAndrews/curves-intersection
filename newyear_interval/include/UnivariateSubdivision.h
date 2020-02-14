#ifndef _UNIVARIATESUBDIVISION_H_
#define _UNIVARIATESUBDIVISION_H_
#include "Global.h"
#include <deque>
#include <iostream>
#include "kv/interval.hpp"
#include "kv/rdouble.hpp"
using namespace std;
class UnivariateSubdivision
{
private:
    void recursive(std::deque<kv::interval<double>> &target_list);

public:
    std::deque<kv::interval<double>> left_list;
    std::deque<kv::interval<double>> right_list;

public:
    UnivariateSubdivision(std::deque<kv::interval<double>> &datalist);
    ~UnivariateSubdivision();
};

#endif