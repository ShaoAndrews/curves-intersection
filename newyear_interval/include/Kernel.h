/*
 * 给定一个节点的指针，判断是否往下细分
*/

#pragma once

#include "Global.h"
#include "NodeBoundingBox.h"
#include <chrono>
using namespace chrono;
class Kernel
{
protected:
    system_clock::time_point start_time;
    system_clock::time_point end_time;
    bool disabled;                              //此模块是否启用判断
public:
    /* data */
    int m_status; //状态码，需要细分为1，不需要细分为0，判断至多一个根为2，初始化为-1
    double spend_time;

public:
    Kernel(NodeBoundingBox *&p);
    bool need_subdivision();
    virtual ~Kernel();
};
