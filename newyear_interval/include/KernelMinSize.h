/*
    鉴别是否达到的最大分割次数
*/

#pragma once

#include "Kernel.h"
class KernelMinSize : public Kernel
{
private:
    /* data */
public:
    KernelMinSize(NodeBoundingBox *&p, bool disabled_ = false);
    ~KernelMinSize();
};
