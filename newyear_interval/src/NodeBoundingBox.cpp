#include "NodeBoundingBox.h"

NodeBoundingBox::NodeBoundingBox(double xl, double xr, double yl, double yr, const IntervalMatrix &coffi_mat_f1, const IntervalMatrix &coffi_mat_f2, const IntervalMatrix &minus_mat, const std::deque<kv::interval<double>>&bezier_x, const std::deque<kv::interval<double>>&bezier_y, const std::string &id_, const string &label) : Node(id_, xl, xr, yl, yr), m_f1_mat(coffi_mat_f1), m_f2_mat(coffi_mat_f2), m_bezier_x(bezier_x), m_bezier_y(bezier_y), m_minus_mat(minus_mat), m_label(label)
{
    child1 = nullptr;
    child2 = nullptr;
    child3 = nullptr;
    child4 = nullptr;
}

NodeBoundingBox::~NodeBoundingBox()
{
    delete child1;
    delete child2;
    delete child3;
    delete child4;
    child1 = nullptr;
    child2 = nullptr;
    child3 = nullptr;
    child4 = nullptr;
}
