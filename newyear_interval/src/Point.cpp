#include "Point.h"
Point::Point(double rx, double ry)
{
    this->rx = rx;
    this->ry = ry;
}
Point::Point()
{
    this->rx = 0.0;
    this->ry = 0.0;
}
Point::Point(const Point &p)
{
    this->rx = p.rx;
    this->ry = p.ry;
}
Point Point::operator+(const Point &b)
{
    Point new_point;
    new_point.rx = this->rx + b.rx;
    new_point.ry = this->ry + b.ry;
    return new_point;
}
Point::~Point()
{
}
double Point::get_distance(const Point &p)
{
    double distance;
    distance = (rx - p.rx) * (rx - p.rx) + (ry - p.ry) * (ry - p.ry);
    return sqrt(distance);
}
double Point::max_norm(const Point &p)
{
    double temp = (rx - p.rx) > (ry - p.ry) ? (rx - p.rx) : (ry - p.ry);
}