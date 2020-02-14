#include "KernelBezier.h"
KernelBezier::KernelBezier(NodeBoundingBox *&p, bool disabled_) : Kernel(p)
{
    this->disabled = disabled_;
    bezier_kernel++;
    if (this->disabled)
    {
        m_status = 1;
    }
    else
    {
        // //三个条件有一个不含有０就是真，需要细分的时候是三个都含有０
        // if ((!p->m_minus_mat.notcontains(0)) &&  (!p->m_f1_mat.notcontains(0)) && (!p->m_f2_mat.notcontains(0)))
        // {
        //     m_status = 1;
        // }
        // else
        // {

        //     m_status = 0;
        // }
                //三个条件有一个不含有０就是真，需要细分的时候是三个都含有０
        if (p->m_minus_mat.notcontains(0) ||  p->m_f1_mat.notcontains(0) || p->m_f2_mat.notcontains(0))
        {
            m_status = 0;
        }
        else
        {

            m_status = 1;
        }
    }
}

KernelBezier::~KernelBezier()
{
    end_time = system_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    bezier_time += double(duration.count()) * microseconds::period::num / microseconds::period::den;
}