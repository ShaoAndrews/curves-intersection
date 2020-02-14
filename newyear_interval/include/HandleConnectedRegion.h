/*
    处理连通区域，希望得到连通区域的中心点（要求中心点必须在连通区域内部）,以及半径
    ref: 
        [1] https://blog.mapbox.com/a-new-algorithm-for-finding-a-visual-center-of-a-polygon-7c77e6492fbc
        [2] https://github.com/mapbox/polylabel                                                                   polylabel algorithm


    选取中心点的时候，我们会首先计算包含此连通区域的最大矩形[x_min, x_max, y_min, y_max]和中心点center_point
    计算这个最大矩形的面积max_area，与连通区域的面积connected_area进行比较
    IF max_area>connected_area:
    THEN
        遍历所有的连通区域，选取距离center_point最近的那个区域的中心点作为牛顿迭代的初始点
    ELSE
        直接将center_point作为迭代的初始点
*/
#pragma once
#include <vector>
#include "Region.h"
#include "Point.h"
#include<assert.h>
using namespace std;
class HandleConnectedRegion
{
private:
    double m_x_min;
    double m_x_max;
    double m_y_min;
    double m_y_max;
    double m_size;


private:
    inline double getDistance(const Point &p1, const Point &p2);

public:
    double m_square_size;        //正方形包围壳的半径
    Point center_point; //矩形包围壳的中点
    Point newton_initial;                  //newton迭代的初始点
    Region BoundingBox(); //返回x和y方向的外围包含壳，与里面的连通区域的padding大小为每个连通区域的边长
public:
    HandleConnectedRegion();
    void submit(const vector<Region> &srcdata, bool cal_square_size = false);

    ~HandleConnectedRegion();
};
