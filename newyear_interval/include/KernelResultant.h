#pragma once
#include "Kernel.h"
#include"kv/interval.hpp"
#include"kv/rdouble.hpp"
class KernelResultant : public Kernel
{
private:
    int NumChangeSign(const std::deque<GiNaC::numeric> &datalist);
    void get_nonzero_index(const std::deque<GiNaC::numeric> &datalist, int &index); //得到双向链表非零的指标
public:
    KernelResultant(NodeBoundingBox *&p, bool disabled_ = false);
    ~KernelResultant();
};
