#include "IntervalMatrix.h"

IntervalMatrix::IntervalMatrix(int rows, int cols) : m_rows(rows), m_cols(cols), mymat(rows, cols)
{
}

IntervalMatrix::~IntervalMatrix()
{
}
bool IntervalMatrix::notcontains(double value)
{
    //首先得到第一个元素的正负号
    kv::interval<double> first = mymat(0, 0);
    if (first.upper() < value)
    {
        for (int i = 0; i < m_rows; ++i)
        {
            for (int j = 0; j < m_cols; ++j)
            {
                if (mymat(i, j).upper()  >= value)
                {
                    return false;
                }
            }
        }
    }
    else if (first.lower() > value)
    {
        //此时除非所有元素均大于value，否则返回false;
        for (int i = 0; i < m_rows; ++i)
        {
            for (int j = 0; j < m_cols; ++j)
            {

                if (mymat(i, j).lower() <= value)
                {
                    return false;
                }
            }
        }
    }
    else
    {
        //第一个元素的区间是含有０的，说明直接无法判断，此时不知道是否含有value的值，返回false;
        return false;
    }
    return true;
}
std::pair<double, double> IntervalMatrix::MumValue()
{
    double max_value;
    double min_value;
    max_value = mymat(0, 0).upper();
    min_value = mymat(0, 0).lower();
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            if (max_value < mymat(i, j).upper())
            {
                max_value = mymat(i, j).upper();
            }
            if (min_value > mymat(i, j).lower())
            {
                min_value = mymat(i, j).lower();
            }
        }
    }
    return std::make_pair(max_value, min_value);
}

kv::interval<double> &IntervalMatrix::operator()(int a, int b)
{
    assert(a < m_rows && b < m_cols && a >= 0 && b >= 0);
    return mymat(a, b);
}
void IntervalMatrix::print()
{
    std::cout.precision(17);
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            std::cout << mymat(i, j) << " ";
        }
        std::cout << "\n";
    }
}
IntervalMatrix::IntervalMatrix(const IntervalMatrix &src) : m_rows(src.m_rows), m_cols(src.m_cols), mymat(src.m_rows, src.m_cols)
{
    for (int i = 0; i < src.m_rows; ++i)
    {
        for (int j = 0; j < src.m_cols; ++j)
        {
            mymat(i, j) = src.mymat(i, j);
        }
    }
}
int IntervalMatrix::cols()
{
    return m_cols;
}
int IntervalMatrix::rows()
{
    return m_rows;
}
