#include "KernelResultant.h"
KernelResultant::KernelResultant(NodeBoundingBox *&p, bool disabled_) : Kernel(p)
{
    this->disabled = disabled_;
    if (this->disabled)
    {
        m_status = 1;
    }
    else
    {
        resultant_kernel++;
        // int changeSign_x = NumChangeSign(p->m_bezier_x);
        // int changeSign_y = NumChangeSign(p->m_bezier_y);
        // if (changeSign_x == 0 || changeSign_y == 0)
        // {
        //     m_status = 0;
        // }
        // else if (changeSign_x == 1 && changeSign_y == 1)
        // {
        //     m_status = 2;
        // }
        // else
        // {
        //     m_status = 1;
        // }

        /*
         *　此时结式的每一个系数都是一个区间，我们直接采用结式判断区域内是否含有根，当x或者y不含有根的时候这块区域就可以删掉
        */
        bool judge_x = true;
        bool judge_y = true;
        kv::interval<double> first = p->m_bezier_x[0];
        if (first.lower() > 0)
        {
            for (deque<kv::interval<double>>::iterator iter = p->m_bezier_x.begin(); iter != p->m_bezier_x.end(); ++iter)
            {
                //此时判断每个下界是否都大于零
                if (iter->lower() <= 0)
                {
                    judge_x = false;
                    break;
                }
            }
        }
        else if (first.upper() < 0)
        {
            for (deque<kv::interval<double>>::iterator iter = p->m_bezier_x.begin(); iter != p->m_bezier_x.end(); ++iter)
            {
                if (iter->upper() >= 0)
                {
                    judge_x = false;
                    break;
                }
            }
        }
        else
        {
            //此时第一个就没法判断，x方向就不用判断了
            judge_x = false;
        }
        if (!judge_x)
        {
            //在x方向没法判断的时候转而看y方向
            first = p->m_bezier_y[0];
            if (first.lower() > 0)
            {
                for (deque<kv::interval<double>>::iterator iter = p->m_bezier_y.begin(); iter != p->m_bezier_y.end(); ++iter)
                {
                    if (iter->lower() <= 0)
                    {
                        judge_y = false;
                        break;
                    }
                }
            }
            else if (first.upper() < 0)
            {
                for (deque<kv::interval<double>>::iterator iter = p->m_bezier_y.begin(); iter != p->m_bezier_y.end(); ++iter)
                {
                    if (iter->upper() >= 0)
                    {
                        judge_y = false;
                        break;
                    }
                }
            }
            else
            {
                judge_y = false;
            }
        }
        if(judge_x || judge_y){
            //只要有一个方向判断成功没有根，那么就能说明这个区域内没有跟
            m_status = 0;
        }else
        {
            m_status = 1;
        }
        
    }
}
KernelResultant::~KernelResultant()
{
    end_time = system_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time);
    resultant_time += double(duration.count()) * microseconds::period::num / microseconds::period::den;
}

int KernelResultant::NumChangeSign(const std::deque<GiNaC::numeric> &datalist)
{
    // for (deque<GiNaC::numeric>::const_iterator iter =datalist.begin(); iter != datalist.end(); ++iter)
    // {
    //     if(iter->is_negative()){
    //         cout << "-";
    //     }else
    //     {
    //         if(iter->is_positive()){
    //             cout << "+";
    //         }else
    //         {
    //             cout << "0";
    //         }

    //     }

    // }
    // cout << ""<<endl;
    int VSN = 0; //记录变号数
    int nonzero_index = 0;
    get_nonzero_index(datalist, nonzero_index);
    if (nonzero_index != 0)
    {
        //说明首节点是0，必有一个根
        VSN++;
    }
    int TOKEN = 1; //记录的是正负号，正的为1，负的为-1
    if (datalist[nonzero_index].is_negative())
    {
        TOKEN = -1;
    }
    for (int j = nonzero_index + 1; j < datalist.size(); ++j)
    {
        if (TOKEN > 0 && datalist[j].is_negative())
        {
            TOKEN = -1;
            VSN++;
        }
        if (TOKEN < 0 && datalist[j].is_positive())
        {
            TOKEN = 1;
            VSN++;
        }
    }
    if (datalist[datalist.size() - 1].is_zero())
    {
        VSN++;
    }
    return VSN;
}

void KernelResultant::get_nonzero_index(const std::deque<GiNaC::numeric> &datalist, int &index)
{
    if (datalist[index].is_zero())
    {
        index++;
        get_nonzero_index(datalist, index);
    }
}