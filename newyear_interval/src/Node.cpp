#include "Node.h"

Node::Node(const string &id, double xl, double xr, double yl, double yr) : m_id(id),
               m_bezier(-1),
               m_resultant(-1),
               m_newton(-1),
               m_center((xl + xl)/2.0, (yl + yr)/2.0),
               m_size((xr - xl) > (yr - yl)? (xr - xl) : (yr - yl)),
               m_region(xl, xr, yl, yr)
{
}

Node::~Node()
{
}