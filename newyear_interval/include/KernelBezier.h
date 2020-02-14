

#pragma once
#include "Kernel.h"

class KernelBezier : public Kernel
{
private:
    /* data */
public:
    KernelBezier(NodeBoundingBox *&p, bool disabled_ = false);
    ~KernelBezier();
};
