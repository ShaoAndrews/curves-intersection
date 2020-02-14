#include "HandleConnectedRegion.h"

HandleConnectedRegion::HandleConnectedRegion()
{
}

HandleConnectedRegion::~HandleConnectedRegion()
{
}
void HandleConnectedRegion::submit(const vector<Region> &srcdata, bool cal_square_size)
{

    m_x_min = 1;
    m_x_max = 0;
    m_y_min = 1;
    m_y_max = 0;
    center_point.rx = 0;
    center_point.ry = 0;
    double first_x = srcdata[0].m_xr - srcdata[0].m_xl;
    double first_y = srcdata[0].m_yr - srcdata[0].m_yl;
    m_size = first_x > first_y ? first_x : first_y;
    vector<Point> center_point_list;
    center_point_list.clear();
    for (vector<Region>::const_iterator iter = srcdata.begin(); iter != srcdata.end(); ++iter)
    {
        if (iter->m_xl < m_x_min)
        {
            m_x_min = iter->m_xl;
        }
        if (iter->m_xr > m_x_max)
        {
            m_x_max = iter->m_xr;
        }
        if (iter->m_yl < m_y_min)
        {
            m_y_min = iter->m_yl;
        }
        if (iter->m_yr > m_y_max)
        {
            m_y_max = iter->m_yr;
        }
        Point temp_center((iter->m_xl + iter->m_xr) / 2.0, (iter->m_yl + iter->m_yr) / 2.0);
        center_point_list.push_back(temp_center);
        center_point = center_point + temp_center;
    }
    center_point.rx = center_point.rx / srcdata.size();
    center_point.ry = center_point.ry / srcdata.size();
    if (cal_square_size)
    {
        double top, bottom, left, right;
        top = m_y_max - center_point.ry;
        bottom = center_point.ry - m_y_min;
        left = center_point.rx - m_x_min;
        right = m_x_max - center_point.rx;
        double temp1, temp2;
        temp1 = top > bottom ? top : bottom;
        temp2 = left > right ? left : right;
        m_square_size = temp1 > temp2 ? temp1 : temp2;
    }

    double max_area = (m_y_max - m_y_min) * (m_x_max - m_x_min);
    double connected_area = first_x * first_y * srcdata.size();
    if (max_area > connected_area)
    {
        double max_dis = 100;
        int index = -1;
        for (int i = 0; i < center_point_list.size(); ++i)
        {
            double cur_dis = getDistance(center_point, center_point_list[i]);
            if (cur_dis < max_dis)
            {
                index = i;
                max_dis = cur_dis;
            }
        }
        newton_initial.rx = center_point_list[index].rx;
        newton_initial.ry = center_point_list[index].ry;
    }
    else
    {
        newton_initial.rx = center_point.rx;
        newton_initial.ry = center_point.ry;
    }
}
double HandleConnectedRegion::getDistance(const Point &p1, const Point &p2)
{
    return (p1.rx - p2.rx) * (p1.rx - p2.rx) + (p1.ry - p2.ry) * (p1.ry - p2.ry);
}
Region HandleConnectedRegion::BoundingBox()
{
    return Region(m_x_min, m_x_max, m_y_min, m_y_max);
}