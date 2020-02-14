//判断当前区域是否已经被覆盖
#pragma once
#include "Kernel.h"
#include <vector>
using namespace std;
class KernelCovered : public Kernel
{
private:
    /* data */
public:
    KernelCovered(NodeBoundingBox *&p, const vector<Region> &covered_region, bool disabled_ = false);
    ~KernelCovered();
};
