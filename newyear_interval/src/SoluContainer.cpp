#include "SoluContainer.h"

SoluContainer::SoluContainer(double xl_, double xr_, double yl_, double yr_) : m_xl(xl_),
                                                                               m_xr(xr_),
                                                                               m_yl(yl_),
                                                                               m_yr(yr_)
{
}

SoluContainer::~SoluContainer()
{
}
void SoluContainer::SubmitSoluAndCoveredRegion(const Point &solu, const Region &covered_region)
{
    //首先判断是否跑出求解区域
    if (solu.rx >= m_xl && solu.rx <= m_xr && solu.ry >= m_yl && solu.ry <= m_yr)
    {
        //判断当前解是否存在于当前覆盖的区域
        if (!m_covered_region_list.empty())
        {
            bool is_new_solu = true; //判断是否是新产生的解
            for (std::vector<Region>::iterator iter = m_covered_region_list.begin(); iter != m_covered_region_list.end(); ++iter)
            {
                if (solu.rx >= iter->m_xl && solu.rx <= iter->m_xr && solu.ry >= iter->m_yl && solu.ry <= iter->m_yr)
                {
                    //说明已经包含在某个唯一性区域内部，无需将此解添加
                    is_new_solu = false;
                    break;
                }
            }
            if (is_new_solu)
            {
                m_solu_list.push_back(solu);
                m_covered_region_list.push_back(covered_region);
            }
        }
        else
        {
            m_solu_list.push_back(solu);
            m_covered_region_list.push_back(covered_region);
        }
    }
    else
    {
        //跑出了求解区域
        //判断唯一性区域是否与当前区域有交
        if (covered_region.checkIntersect(Region(0, 1, 0, 1)))
        {
            m_covered_region_list.push_back(covered_region);
        }
    }
}

bool SoluContainer::ContainsCurrentRegion(const Region &current_region)
{
    if (!m_covered_region_list.empty())
    {
        bool is_subset = false;
        for (std::vector<Region>::iterator iter = m_covered_region_list.begin(); iter != m_covered_region_list.end(); ++iter)
        {
            if (current_region.is_subsetof(*iter))
            {
                //是某个覆盖区域的子集
                is_subset = true;
                break;
            }
        }
        return is_subset;
    }
    else
    {
        return false;
    }
}
bool SoluContainer::ContainsCurrentRegion(double xl, double xr, double yl, double yr)
{
    if (!m_covered_region_list.empty())
    {
        bool is_subset = false;
        for (std::vector<Region>::iterator iter = m_covered_region_list.begin(); iter != m_covered_region_list.end(); ++iter)
        {
            if (xl >= iter->m_xl && xr <= iter->m_xr && yl >= iter->m_yl && yr <= iter->m_yr)
            {
                //是某个覆盖区域的子集
                is_subset = true;
                break;
            }
        }
        return is_subset;
    }
    else
    {
        return false;
    }
}
bool SoluContainer::ContainsCurrentSolu(const Point &p)
{
    if (!m_covered_region_list.empty())
    {
        bool isin = false;
        for (std::vector<Region>::iterator iter = m_covered_region_list.begin(); iter != m_covered_region_list.end(); ++iter)
        {
            if (p.rx >= iter->m_xl && p.rx <= iter->m_xr && p.ry >= iter->m_yl && p.ry <= iter->m_yr)
            {
                isin = true;
                break;
            }
        }
        return isin;
    }
    else
    {
        return false;
    }
}
int SoluContainer::getSoluIndexByContainCurrentSolu(const Point &p)
{
    int index = -1;
    if (!m_solu_list.empty() && !m_covered_region_list.empty())
    {
        for (int i = 0; i < m_covered_region_list.size(); ++i)
        {
            if (p.rx >= m_covered_region_list[i].m_xl && p.rx <= m_covered_region_list[i].m_xr && p.ry >= m_covered_region_list[i].m_yl && p.ry <= m_covered_region_list[i].m_yr)
            {
                index = i;
                break;
            }
        }
    }
    return index;
}