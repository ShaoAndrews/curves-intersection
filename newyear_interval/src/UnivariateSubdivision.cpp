#include "UnivariateSubdivision.h"
UnivariateSubdivision::UnivariateSubdivision(std::deque<kv::interval<double>> &datalist)
{
    recursive(datalist);
}

UnivariateSubdivision::~UnivariateSubdivision()
{
}
void UnivariateSubdivision::recursive(std::deque<kv::interval<double>> &target_list)
{
    kv::interval<double> two(2);
    if (target_list.size() > 1)
    {
        left_list.push_back(target_list[0]);
        right_list.push_front(target_list[target_list.size() - 1]);
        for (std::deque<kv::interval<double>>::iterator iter = target_list.begin(); iter != target_list.end() - 1; ++iter)
        {
            *iter = (*iter + *(iter + 1))/two;
        }
        target_list.erase(target_list.end());
        recursive(target_list);
    }
    else
    {
        left_list.push_back(target_list[0]);
        right_list.push_front(target_list[0]);
    }
}