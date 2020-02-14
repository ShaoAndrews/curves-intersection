#include "MyWolframEngine.h"

MyWolframEngine::MyWolframEngine(const string &expr)
{
    env = WSInitialize((WSEnvironmentParameter)0);
    int argc = 4;
    char *argv[5] = {(char *)"-linkname", (char *)"Resultant", (char *)"-linkmode", (char *)"connect", NULL};
    Link = WSOpen(argc, argv);
    if ((WSLINK)0 == Link)
    {
        cout << " Unable to create the link..." << endl;
    }
    WSPutFunction(Link, "EvaluatePacket", 1);
    WSPutFunction(Link, "ToExpression", 1);
    WSPutString(Link, expr.c_str());
    WSEndPacket(Link);
    //接受计算的结果
    double *data_x;
    int length_x;
    WSNewPacket(Link);
    WSGetReal64List(Link, &data_x, &length_x);
    kv::interval<double> temp_interval;
    for (int i = 0; i < length_x; ++i)
    {
        temp_interval = (kv::interval<double>)data_x[i];
        bezier_res_x_interval.push_back(temp_interval);
    }
    WSReleaseReal64List(Link, data_x, length_x);
    WSEndPacket(Link);

    double *data_y;
    int length_y;
    WSNewPacket(Link);
    WSGetReal64List(Link, &data_y, &length_y);
    for (int i = 0; i < length_y; ++i)
    {
        temp_interval = (kv::interval<double>)data_y[i];
        bezier_res_y_interval.push_back(temp_interval);
    }
    WSReleaseReal64List(Link, data_y, length_y);
    WSEndPacket(Link);
}

MyWolframEngine::~MyWolframEngine()
{
    WSClose(Link);
    WSDeinitialize(env);
}
