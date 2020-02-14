/*
    使用mathematica引擎进行计算
    使用boundingbox算法
*/
#pragma once
#include "Algorithm.h"
#include "NodeBoundingBox.h"
#include <queue>
#include "ExToMat.h"
#include "MatToString.h"
#include "MatToStringCFormat.h"
#include "PowerCoffi.h"
#include "Kernel.h"
#include "KernelBezier.h"
#include "KernelResultant.h"
#include "KernelMinSize.h"
#include "Subdivision.h"
#include "UnivariateSubdivision.h"
#include <iostream>
#include "Region.h"
#include "KernelCovered.h"
#include <map>
#include "VectorSort.h"
#include "HandleConnectedRegion.h"
#include "IntelligentNewton.h"
#include "SubdivisionByPara.h"
#include "NewKantorovich.h"
#include "SoluContainer.h"
#include "MyWolframEngine.h"
#include <assert.h>
struct MapKeySort
{
    bool operator()(const string &k1, const string &k2) const
    {
        int cur = 0;
        bool status;
        while (cur < k1.size())
        {
            if ((k1.at(cur) - '0') < (k2.at(cur) - '0'))
            {
                status = true;
                break;
            }
            else if ((k1.at(cur) - '0') > (k2.at(cur) - '0'))
            {
                status = false;
                break;
            }
            else
            {
                status = false;
                cur++;
            }
        }
        return status;
    }
};
class AlgorithmBoundingBoxWolframScriptEngine : public Algorithm
{
private:
    NodeBoundingBox *root;
    void create_tree();
    void destory_tree(NodeBoundingBox *&p);
    void subdivision(NodeBoundingBox *&current, queue<NodeBoundingBox *> &queue_);
    pair<int, int> *FSM_table;
    string get_neighbor(const string &src, int direction);
    int get_label(const pair<string, Region> &p); //获取当前区域的标签
    void unionElements(int p, int q, int parent[]);
    int find_root(int p, int parent[]);
    SubdivisionByPara *M_solver;                    //用于估计任意子区域的M大小
    HandleConnectedRegion *handle_connected_region; //处理连通区域，给出连通区域的包围壳并且给出连通区域的参考初始点
    NewKantorovich *kantorovich_judge;              //使用newton-kantorovich定理判断
    vector<pair<string, Region>> m_unknown_regions; //记录未确定区域
    IntelligentNewton *newton_method;

    MyWolframEngine *mywolframengine;

public:
    AlgorithmBoundingBoxWolframScriptEngine(const Eigen::MatrixXd &f1_mat, const Eigen::MatrixXd &f2_mat, double precision, double xl = 0.0, double xr = 1.0, double yl = 0.0, double yr = 1.0);
    ~AlgorithmBoundingBoxWolframScriptEngine();
    vector<Region> m_real_unknown_region;
    SoluContainer *solu_container;
};
