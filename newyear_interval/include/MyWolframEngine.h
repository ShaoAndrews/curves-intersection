/*
 * 使用c++跟mathematica进行通信
*/
#pragma once
#include "wstp.h"
#include <string>
#include "kv/interval.hpp"
#include "kv/rdouble.hpp"
#include <deque>
using namespace std;
class MyWolframEngine
{
public:
    deque<kv::interval<double>> bezier_res_x_interval;
    deque<kv::interval<double>> bezier_res_y_interval;

private:
    WSLINK Link;
    WSENV env;
public:
    MyWolframEngine(const string &expr);
    ~MyWolframEngine();
};
