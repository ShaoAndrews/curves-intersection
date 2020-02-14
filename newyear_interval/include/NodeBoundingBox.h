#pragma once
#include "Node.h"
#include "Global.h"
#include <deque>
#include<Eigen/Dense>
#include"IntervalMatrix.h"
class NodeBoundingBox : public Node
{
private:
    /* data */
public:
    NodeBoundingBox(double xl, double xr, double yl, double yr, const IntervalMatrix &coffi_mat_f1, const IntervalMatrix &coffi_mat_f2, const IntervalMatrix &minus_mat, const std::deque<kv::interval<double>>&bezier_x, const std::deque<kv::interval<double>>&bezier_y, const std::string &id_, const string &label);
    ~NodeBoundingBox();
public:
    IntervalMatrix m_f1_mat;
    IntervalMatrix m_f2_mat;
    IntervalMatrix m_minus_mat;
    std::deque<kv::interval<double>> m_bezier_x;
    std::deque<kv::interval<double>> m_bezier_y;
    NodeBoundingBox *child1;
    NodeBoundingBox *child2;
    NodeBoundingBox *child3;
    NodeBoundingBox *child4;
    string m_label;                                  //用来记录当前节点属于哪一块区域
};
