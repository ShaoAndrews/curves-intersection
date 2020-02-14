#include "Region.h"
Region::Region(double xl_, double xr_, double yl_, double yr_) : m_xl(xl_),
                                                                 m_xr(xr_),
                                                                 m_yl(yl_),
                                                                 m_yr(yr_),
                                                                 index(-1)
{
}
Region::Region() : m_xl(0),
                   m_xr(1),
                   m_yl(0),
                   m_yr(1),
                   index(-1)
{
}
Region::~Region()
{
}
bool Region::is_subsetof(const Region &target) const
{
    bool status = false;
    if (m_xl >= target.m_xl && m_xr <= target.m_xr && m_yl >= target.m_yl && m_yr <= target.m_yr)
    {
        status = true;
    }
    return status;
}
// bool Region::contains(const Point &p) const
// {

//     bool status = false;
//     if (p.rx >= m_xl && p.rx <= m_xr && p.ry >= m_yl && p.ry <= m_yr)
//     {
//         status = true;
//     }
//     return status;
// }
bool Region::checkIntersect(const Region &target) const
{
    bool nonIntersect = false;
    if(target.m_xr < m_xl || target.m_xl > m_xr || target.m_yl > m_yr || target.m_yr < m_yl){
        nonIntersect = true;
    }
    bool Intersect = !nonIntersect;
    return Intersect;
}