#include "Kernel.h"

Kernel::Kernel(NodeBoundingBox *&p) : m_status(-1), disabled(false)
{
    start_time = system_clock::now();
}
Kernel::~Kernel()
{
   

}
bool Kernel::need_subdivision()
{
    bool status = false;
    if (m_status == 1)
    {
        status = true;
    }
    return status;
}