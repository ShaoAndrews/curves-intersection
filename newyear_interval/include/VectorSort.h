
#pragma once
#include "Region.h"
#include <string>
using namespace std;
class VectorSort
{
public:
    bool operator()(const pair<string, Region> &t1, const pair<string, Region> &t2)
    {
        if ((t1.second.m_yr + t1.second.m_yl) < (t2.second.m_yl + t2.second.m_yr))
        {
            return false;
        }
        else if ((t1.second.m_yr + t1.second.m_yl) > (t2.second.m_yl + t2.second.m_yr))
        {
            return true;
        }
        else
        {
            if ((t1.second.m_xr + t1.second.m_xl) < (t2.second.m_xl + t2.second.m_xr))
            {
                return true;
            }else
            {
                return false;
            }
            
        }
    }
};
