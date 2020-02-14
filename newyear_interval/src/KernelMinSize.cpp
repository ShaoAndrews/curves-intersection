#include "KernelMinSize.h"
KernelMinSize::KernelMinSize(NodeBoundingBox *&p, bool disabled_) : Kernel(p)
{
    this->disabled = disabled_;
    if (this->disabled)
    {
        m_status = 1;
    }
    else
    {

        if (p->m_size < MINISIZE)
        {
            m_status = 0;
        }
        else
        {
            m_status = 1;
        }
    }
}
KernelMinSize::~KernelMinSize()
{
}