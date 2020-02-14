#include "KernelCovered.h"

KernelCovered::KernelCovered(NodeBoundingBox *&p, const vector<Region> &covered_region, bool disabled_) : Kernel(p)
{
    this->disabled = disabled_;
    if (this->disabled)
    {
        m_status = 1;
    }
    else
    {

        m_status = 1;
        if (!covered_region.empty())
        {
            for (vector<Region>::const_iterator iter = covered_region.begin(); iter != covered_region.end(); ++iter)
            {
                if (p->m_region.is_subsetof(*iter))
                {
                    m_status = 0;
                    break;
                }
            }
        }
    }
}

KernelCovered::~KernelCovered()
{
}