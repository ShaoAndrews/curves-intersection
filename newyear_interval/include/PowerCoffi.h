#ifndef _POWERCOFFI_H_
#define _POWERCOFFI_H_
#include <deque>
#include "Global.h"
/*
计算在给定点的各阶导数值
*/
class PowerCoffi
{
public:
    std::deque<GiNaC::numeric> result;

private:
    GiNaC::numeric x_star;

private:
    /* data */
    //根据递推公式求在x^*处的值，返回一个vector,vector为导函数的系数
    GiNaC::numeric Calculate_xstar(std::deque<GiNaC::numeric> &coffi_vector);
    void Recursive(std::deque<GiNaC::numeric> &coffi_vector);

public:
    PowerCoffi(const GiNaC::ex &f, const GiNaC::numeric &p, double max_value = 1);
    ~PowerCoffi();
};

#endif