#include "MaxValueOfPoly.h"

MaxValueOfPoly::MaxValueOfPoly(const GiNaC::ex &f, const kv::interval<double> &x_region, const kv::interval<double> &y_region):image(0., 0.)
{
    int x_degree = f.degree(x);
    int y_degree = f.degree(y);
    kv::interval<double> coffi;
    for (int i = 0; i <= x_degree; ++i)
    {
        for (int j = 0; j <= y_degree; ++j)
        {
            coffi = (kv::interval<double>)GiNaC::ex_to<GiNaC::numeric>(f.coeff(x, i).coeff(y, j).evalf()).to_double();
            image += coffi * pow(x_region, i)*pow(y_region, j);
        }
    }
}

MaxValueOfPoly::~MaxValueOfPoly()
{
}