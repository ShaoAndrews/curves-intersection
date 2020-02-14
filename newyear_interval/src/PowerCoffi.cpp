#include "PowerCoffi.h"
PowerCoffi::PowerCoffi(const GiNaC::ex &f, const GiNaC::numeric &p, double max_value)
{
    x_star = p;
    int deg = f.degree(x);
    std::deque<GiNaC::numeric> init_coffi;
    for (int i = 0; i <= deg; ++i)
    {
        init_coffi.push_back(GiNaC::ex_to<GiNaC::numeric>(f.coeff(x, deg - i))/max_value);
    }
    Recursive(init_coffi);
}

PowerCoffi::~PowerCoffi()
{
}
GiNaC::numeric PowerCoffi::Calculate_xstar(std::deque<GiNaC::numeric> &coffi_vector)
{
    GiNaC::numeric value = coffi_vector.front();
    int count = coffi_vector.size()-1;
    coffi_vector.front() = value * count;
    for (std::deque<GiNaC::numeric>::iterator iter = coffi_vector.begin() + 1; iter != coffi_vector.end(); ++iter)
    {
        count--;
        value = value * x_star + *(iter);
        *(iter) = *(iter)*count;
    }
    return value;
}
void PowerCoffi::Recursive(std::deque<GiNaC::numeric> &coffi_vector)
{
    if (coffi_vector.size()>1)
    {
        GiNaC::numeric temp = Calculate_xstar(coffi_vector);
        result.push_back(temp);
        //删除最后的元素
        coffi_vector.pop_back();
        Recursive(coffi_vector);
    }else
    {
        result.push_back(coffi_vector.back());
    }
    
}