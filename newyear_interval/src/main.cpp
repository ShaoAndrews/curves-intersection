/*
 * created by SHAO Wenbing 2019-12-9
 * 
 * 基于stdcurvesTestNewtonOnestep程序修改的，将结式计算的部分改为了区间矩阵进行计算，避免了符号计算带来的误差
 * 改变了牛顿的开始时间，直接改成连通区域不再增加的时候使用连通区域的中心点来作为x0
 * 对最后的未定区域的处理，对求出来的解重新划定唯一性半径，希望将未定区域收入囊中
 * 连通区域不再增加可能难以界定，因此我们将每层都重新划定连通区域
*/

#include <iostream>
#include <getopt.h>
#include <string>
#include <fstream>
#include <vector>
#include <Eigen/Dense>
#include <chrono>
#include <jsoncpp/json/json.h>
#include "Global.h"
#include "MatToEx.h"
#include "MatToString.h"
#include "MatToStringPythonFormat.h"
#include "IntervalMatrix.h"
#include "MaxValueOfPoly.h"
#include "AlgorithmBoundingBoxWolframScriptEngine.h"
#include <unistd.h>
using namespace std;
using namespace chrono;
using namespace GiNaC;
template <typename T>
T load_csv(const std::string &path)
{
    std::ifstream in;
    in.open(path);
    std::string line;
    std::vector<double> values;
    uint rows = 0;
    while (std::getline(in, line))
    {
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ','))
        {
            double val = std::stod(cell);
            values.push_back(val);
        }
        ++rows;
    }
    return Eigen::Map<const Eigen::Matrix<
        typename T::Scalar,
        T::RowsAtCompileTime,
        T::ColsAtCompileTime,
        Eigen::RowMajor>>(values.data(), rows, values.size() / rows);
}
void usage()
{
    cout << "Options:" << endl;
    cout << " -a,  --f1=f1.csv           A csv file of polynomial about f1" << endl;
    cout << " -b,  --f2=f2.csv           A csv file of polynomial about f2" << endl;
    cout << " -m,  --f1_path=F1PATH      The path of csv file about f2" << endl;
    cout << "                            default value is './'" << endl;
    cout << " -n,  --f2_path=F2PATH      The path of csv file about f2" << endl;
    cout << "                            default value is './'" << endl;
    cout << " -o,  --out=OUT             The path of output file" << endl;
    cout << "                            default value is './curves@f1@f2.csv'" << endl;
    cout << "                            The fist line records the time, and " << endl;
    cout << "                            the second line records the number of roots" << endl;
    cout << " -t,  --tolerance           Set default precision 1e-6" << endl;
    cout << " -d,  --detail              Show the details for each layer" << endl;
    cout << " -l,  --layer               The json dir for each layer" << endl;
    cout << " -h,  --help(no value)      Print the message and exit" << endl;
}
int main(int argc, char *argv[])
{
    //定义输入的参数
    auto start = system_clock::now();
    string f1, f2;
    string f1_path = "./";
    string f2_path = "./";
    string output_path = "./";
    double precision = 1e-6;
    bool show_detail = false;
    string layer_dir = "./";
    int getopt_ret, option_index;
    bool status = true;
    static struct option long_options[] = {
        {"f1", required_argument, 0, 'a'},
        {"f2", required_argument, 0, 'b'},
        {"f1_path", optional_argument, 0, 'm'},
        {"f2_path", optional_argument, 0, 'n'},
        {"out", optional_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {"tolerance", optional_argument, 0, 't'},
        {"detail", optional_argument, 0, 'd'},
        {"layer", optional_argument, 0, 'l'},
        {0, 0, 0, 0}};
    while (1)
    {
        getopt_ret = getopt_long(argc, argv, "a:b:m::n::o::ht::dl::", long_options, &option_index);
        if (getopt_ret == -1)
            break;
        switch (getopt_ret)
        {
        case 0:
            status = false;
            break;
        case 'a':
            f1 = optarg;
            break;
        case 'b':
            f2 = optarg;
            break;
        case 'm':
            f1_path = optarg;
            break;
        case 'n':
            f2_path = optarg;
            break;
        case 'o':
            output_path = optarg;
            break;
        case 'l':
            layer_dir = optarg;
            break;
        case 'h':
            usage();
            status = false;
            break;
        case 't':
            precision = atof(optarg);
            break;
        case 'd':
            show_detail = true;
            break;
        case '?':
            status = false;
            break;
        default:
            status = false;
            break;
        }
    }
    if (status)
    {
        if (access((f1_path + f1).c_str(), F_OK) != 0)
        {
            cout << "The path" << f1_path + f1 << " not exist" << endl;
            return -1;
        }
        if (access((f2_path + f2).c_str(), F_OK) != 0)
        {
            cout << "The path" << f2_path + f2 << " not exist" << endl;
            return -1;
        }
        Eigen::MatrixXd f1_mat = load_csv<Eigen::MatrixXd>(f1_path + f1);
        Eigen::MatrixXd f2_mat = load_csv<Eigen::MatrixXd>(f2_path + f2);
        //将值传入进四叉树
        // AlgorithmBoundingBox *boundingbox = new AlgorithmBoundingBox(f1_mat, f2_mat, precision);
        AlgorithmBoundingBoxWolframScriptEngine *boundingbox = new AlgorithmBoundingBoxWolframScriptEngine(f1_mat, f2_mat, precision);
        //开始写入数据
        output_path += "BoundingBox.json";
        //
        Json::Value root;
        //首先注明表达式
        MatToString *mat2string1 = new MatToStringPythonFormat(f1_mat);
        MatToString *mat2string2 = new MatToStringPythonFormat(f2_mat);
        root["f1"] = Json::Value(mat2string1->get_expression());
        root["f2"] = Json::Value(mat2string2->get_expression());
        delete mat2string1;
        delete mat2string2;
        mat2string1 = nullptr;
        mat2string2 = nullptr;
        root["xl"] = Json::Value(0);
        root["xr"] = Json::Value(1);
        root["yl"] = Json::Value(0);
        root["yr"] = Json::Value(1);
        root["precision"] = Json::Value(precision);
        //记录根的数目
        int root_num = boundingbox->solu_container->m_solu_list.size();
        root["root_num"] = Json::Value(root_num);
        //记录根的信息
        Json::Value root_messange;
        if (!boundingbox->solu_container->m_solu_list.empty())
        {
            for (int i = 0; i < boundingbox->solu_container->m_solu_list.size(); ++i)
            {
                Json::Value node;
                node["x"] = boundingbox->solu_container->m_solu_list[i].rx;
                node["y"] = boundingbox->solu_container->m_solu_list[i].ry;
                root_messange.append(node);
            }
        }
        root["root_message"] = root_messange;
        root["unknown_region"] = unknown_region;
        Json::Value unknown;
        if (!boundingbox->m_real_unknown_region.empty())
        {
            Json::Value node;
            for (int i = 0; i < boundingbox->m_real_unknown_region.size(); ++i)
            {
                node["xl"] = boundingbox->m_real_unknown_region[i].m_xl;
                node["xr"] = boundingbox->m_real_unknown_region[i].m_xr;
                node["yl"] = boundingbox->m_real_unknown_region[i].m_yl;
                node["yr"] = boundingbox->m_real_unknown_region[i].m_yr;
                unknown.append(node);
            }
        }
        root["unknown"] = unknown;

        Json::Value retime;
        auto end = system_clock::now();
        auto create_duration = duration_cast<microseconds>(end - start);
        double total_time = double(create_duration.count()) * microseconds::period::num / microseconds::period::den;
        retime["bezier_time"] = Json::Value(bezier_time);
        retime["resultant_time"] = Json::Value(resultant_time);
        retime["newton_time"] = Json::Value(newton_time);
        retime["init_time"] = Json::Value(init_time);
        retime["connected_time"] = Json::Value(connected_time);
        retime["create_tree_time"] = Json::Value(create_tree_time);
        retime["total_time"] = Json::Value(total_time);
        root["time"] = retime;

        Json::Value kernel;
        kernel["bezier_kernel"] = Json::Value(bezier_kernel);
        kernel["resultant_kernel"] = Json::Value(resultant_kernel);
        kernel["newton_kernel"] = Json::Value(newton_kernel);
        root["kernel"] = kernel;

        root["precision"] = Json::Value(precision);
        Json::StyledWriter sw;
        ofstream os;
        os.open(output_path);
        os << sw.write(root);
        os.close();
        delete boundingbox;
        boundingbox = nullptr;
    }
    return 0;
}
