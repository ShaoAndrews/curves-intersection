#include "AlgorithmBoundingBoxWolframScriptEngine.h"
AlgorithmBoundingBoxWolframScriptEngine::AlgorithmBoundingBoxWolframScriptEngine(const Eigen::MatrixXd &f1_mat, const Eigen::MatrixXd &f2_mat, double precision, double xl, double xr, double yl, double yr) : Algorithm(f1_mat, f2_mat, precision, xl, xr, yl, yr)
{
    auto start = system_clock::now();

    //得到f1和f2的bezier表示
    MatToBezier *mat2bezier1 = new MatToBezier(f1_mat);
    MatToBezier *mat2bezier2 = new MatToBezier(f2_mat);
    IntervalMatrix f1_mat_bezier = mat2bezier1->coffimat;
    IntervalMatrix f2_mat_bezier = mat2bezier2->coffimat;
    delete mat2bezier1;
    delete mat2bezier2;
    mat2bezier1 = nullptr;
    mat2bezier2 = nullptr;
    //计算f1和f2的最大系数
    double f1_max_coffi;
    double f2_max_coffi;
    double f1_temp_min;
    double f1_temp_max;
    double f2_temp_min;
    double f2_temp_max;
    std::pair<double, double> f1_mum_value = f1_mat_bezier.MumValue();
    std::pair<double, double> f2_mum_value = f2_mat_bezier.MumValue();
    f1_temp_min = fabs(f1_mum_value.second);
    f1_temp_max = fabs(f1_mum_value.first);
    f2_temp_min = fabs(f2_mum_value.second);
    f2_temp_max = fabs(f2_mum_value.first);
    f1_max_coffi = 0.01 * ((f1_temp_min > f1_temp_max) ? f1_temp_min : f1_temp_max);
    f2_max_coffi = 0.01 * ((f2_temp_min > f2_temp_max) ? f2_temp_min : f2_temp_max);
    // minus_f = f1 - f2
    GiNaC::ex minus_f = GiNaC::expand(m_f1 / f1_max_coffi - m_f2 / f2_max_coffi);
    //将相减的表达式转化为矩阵

    ExToMat *ex2mat = new ExToMat(minus_f);
    Eigen::MatrixXd f_minus_mat = ex2mat->mat;
    delete ex2mat;
    ex2mat = nullptr;

    //将矩阵转化为bezier表示形式
    MatToBezier *mat2bezier_f_minus = new MatToBezier(f_minus_mat);
    IntervalMatrix minus_bezier_mat = mat2bezier_f_minus->coffimat;
    delete mat2bezier_f_minus;
    mat2bezier_f_minus = nullptr;

    //启动wolframescript引擎
    //得到f1和f2的字符串表达式

    MatToString *mattostring1 = new MatToStringCFormat(f1_mat);
    string f1_string = mattostring1->get_expression();
    MatToString *mattostring2 = new MatToStringCFormat(f2_mat);
    string f2_string = mattostring2->get_expression();
    delete mattostring1;
    delete mattostring2;
    mattostring1 = nullptr;
    mattostring2 = nullptr;
    //计算结式部分
    string expr = "f1="+f1_string;
    expr += ";f2="+f2_string;
    mywolframengine = new MyWolframEngine(expr);
    //初始化树
    std::string root_id = "0";
    root = new NodeBoundingBox(0, 1, 0, 1, f1_mat_bezier, f2_mat_bezier, minus_bezier_mat, mywolframengine->bezier_res_x_interval, mywolframengine->bezier_res_y_interval, root_id, "");
    FSM_table = new pair<int, int>[32] { make_pair(1, 8), make_pair(0, 0), make_pair(3, 8), make_pair(2, 0),
                                             make_pair(1, 1), make_pair(0, 8), make_pair(3, 1), make_pair(2, 8),
                                             make_pair(2, 8), make_pair(3, 8), make_pair(0, 2), make_pair(1, 2),
                                             make_pair(2, 3), make_pair(3, 3), make_pair(0, 8), make_pair(1, 8),
                                             make_pair(3, 3), make_pair(2, 4), make_pair(1, 8), make_pair(0, 0),
                                             make_pair(3, 8), make_pair(2, 0), make_pair(1, 2), make_pair(0, 5),
                                             make_pair(3, 1), make_pair(2, 8), make_pair(1, 6), make_pair(0, 2),
                                             make_pair(3, 7), make_pair(2, 3), make_pair(1, 1), make_pair(0, 8) };

    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    init_time = double(duration.count()) * microseconds::period::num / microseconds::period::den;
    M_solver = new SubdivisionByPara(m_hessian_f1, m_hessian_f2, 0, 1, 0, 1);
    handle_connected_region = new HandleConnectedRegion();
    newton_method = new IntelligentNewton(m_f1, m_f2, m_df1_x, m_df1_y, m_df2_x, m_df2_y, m_precision);
    kantorovich_judge = new NewKantorovich(m_f1, m_f2, m_df1_x, m_df1_y, m_df2_x, m_df2_y);
    solu_container = new SoluContainer(0, 1, 0, 1);
    auto create_tree_start_time = system_clock::now();
    create_tree();
    auto create_tree_end_time = system_clock::now();
    auto create_duration = duration_cast<microseconds>(create_tree_end_time - create_tree_start_time);
    create_tree_time = double(create_duration.count()) * microseconds::period::num / microseconds::period::den;
}

AlgorithmBoundingBoxWolframScriptEngine::~AlgorithmBoundingBoxWolframScriptEngine()
{
    destory_tree(root);
    delete[] FSM_table;
    delete M_solver;
    M_solver = nullptr;
    delete handle_connected_region;
    handle_connected_region = nullptr;
    delete newton_method;
    newton_method = nullptr;
    delete kantorovich_judge;
    kantorovich_judge = nullptr;
    delete solu_container;
    solu_container = nullptr;
    delete mywolframengine;
    mywolframengine = nullptr;
}
void AlgorithmBoundingBoxWolframScriptEngine::destory_tree(NodeBoundingBox *&p)
{
    if (p)
    {
        destory_tree(p->child1);
        destory_tree(p->child2);
        destory_tree(p->child3);
        destory_tree(p->child4);
        delete p;
        p = nullptr;
    }
}

void AlgorithmBoundingBoxWolframScriptEngine::create_tree()
{
    queue<NodeBoundingBox *> queue_;
    NodeBoundingBox *current = nullptr;
    queue_.push(root);
    Kernel *kernel_point = nullptr;
    int level, cur_level;
    vector<pair<string, Region>> connected_data; //记录当前层的所有data数据, string 是对应的id号，Region是对应的区域，而且Region有一个指标index，用来记录在排序前的位置
    map<string, int> id2index_map;               //记录id与编号的关系，此时id对应的编号是排序前的编号
    map<string, int> unknown_id2index_map;       //记录计算最后unknown区域的id与编号的关系，此时id对应的编号是排序前的编号
    int map_index = 0;                           //记录id对应的指标
    int unknown_map_index = 0;
    level = 1;                                 //记录层数
    int has_computed_layers = 1;               //已经计算的层数
    int *connected_layers_num = new int[2]{0}; //记录上两层连通区域的个数
    bool start_newton_kernel = false;          //是否开启newton模块
    while (!queue_.empty())
    {
        current = queue_.front();
        queue_.pop();
        //开始判断当前层数
        cur_level = current->m_id.size();
        auto start_time = system_clock::now();
        if (!solu_container->ContainsCurrentRegion(current->m_region))
        {
            if (cur_level == level)
            {
                //属于同一层
                /*
             *
             * 首先判断当前的这个节点是否被标记了，如果被标记了，那么此时这个节点对应的连通区域已经被设定好了
             * 此时无需进行连通区域判断
            */
                current->m_region.index = map_index;
                connected_data.push_back(make_pair(current->m_id, current->m_region));
                id2index_map.insert(make_pair(current->m_id, map_index));
                map_index++;
            }
            else
            {
                /*
                    此时需要进行two pass算法
                */
                auto start_connected = system_clock::now();
                sort(connected_data.begin(), connected_data.end(), VectorSort());
                const int size = connected_data.size();

                //获取排序之后和排序之前的坐标对应
                int *src2current = new int[size]{0}; //将原来的编号映射成当前的编号
                for (int i = 0; i < size; ++i)
                {
                    src2current[connected_data[i].second.index] = i;
                }
                //初始化parent标号
                int *m_parent = new int[size]{-1};

                for (int i = 0; i < size; ++i)
                {

                    //得到上左邻居
                    /*
                    R  -> 0         right
                    L  -> 1         left
                    D  -> 2         down
                    U  -> 3         up
                    RU -> 4         right + up
                    RD -> 5         right + down
                    LD -> 6         left + down
                    LU -> 7         left + up
                    halt -> 8       no direction
                */
                    string cur_id = connected_data[i].first;
                    string U_neighbor = get_neighbor(cur_id, 3);
                    string L_neighbor = get_neighbor(cur_id, 1);
                    int l_msg = -1, u_msg = -1;
                    int l_msg_index = -1;
                    int u_msg_index = -1;
                    map<string, int>::iterator neighbor_iter;
                    neighbor_iter = id2index_map.find(L_neighbor);
                    if (neighbor_iter != id2index_map.end())
                    {
                        assert(neighbor_iter->second < size);
                        assert(src2current[neighbor_iter->second] < size);
                        l_msg_index = src2current[neighbor_iter->second];
                        l_msg = m_parent[l_msg_index]; //先找到string 对应的原来的编号，再找到原来编号在当前的位置，最后查找当前位置的根节点
                    }
                    neighbor_iter = id2index_map.find(U_neighbor);
                    if (neighbor_iter != id2index_map.end())
                    {
                        u_msg_index = src2current[neighbor_iter->second];
                        u_msg = m_parent[u_msg_index];
                    }
                    if (l_msg >= 0 || u_msg >= 0)
                    {
                        if (l_msg >= 0 && u_msg >= 0)
                        {
                            assert(l_msg < size);
                            assert(u_msg < size);
                            m_parent[i] = l_msg < u_msg ? l_msg : u_msg;
                            if (l_msg <= u_msg)
                            {
                                unionElements(u_msg_index, l_msg_index, m_parent);
                            }
                            else
                            {
                                unionElements(l_msg_index, u_msg_index, m_parent);
                            }
                        }
                        else
                        {
                            int temp = l_msg > u_msg ? l_msg : u_msg;
                            m_parent[i] = temp;
                        }
                    }
                    else
                    {
                        m_parent[i] = i;
                    }
                }
                map<string, vector<Region>> classify_data;
                classify_data.clear();
                map<string, vector<Region>>::iterator classify_iter;
                for (int i = 0; i < size; ++i)
                {
                    classify_iter = classify_data.find(to_string(m_parent[i]));
                    if (classify_iter != classify_data.end())
                    {
                        //说明里面有这个标签
                        classify_data[to_string(m_parent[i])].push_back(connected_data[i].second);
                    }
                    else
                    {
                        //说明里面没有这个标签
                        vector<Region> newlabellist;
                        newlabellist.clear();
                        classify_data.insert(make_pair(to_string(m_parent[i]), newlabellist));
                        classify_data[to_string(m_parent[i])].push_back(connected_data[i].second);
                    }
                }
                /*
                 * 判断当前的连通区域个数
                 * 当连通区域不再增加的时候我们才开始考虑使用newton模块进行计算
                 * 考虑的是前两层的情况，如果上两层连通区域个数>=上一层连通区域个数>=当前层连通区域个数，那么说明连通区域个数不再增加，此时才开始使用牛顿
                 * 因为要考虑上两层的情况，所以计算的层数必须首先大于２才能进行判断
                */
                if (has_computed_layers > 2)
                {
                    //计算超过两层
                    //判断连通区域的个数是否不再增加
                    int current_connected_component = classify_data.size();
                    if ((connected_layers_num[0] >= connected_layers_num[1]) && (connected_layers_num[1] >= current_connected_component) && has_computed_layers >= 8)
                    {
                        //说明two_pass算法可以不需要再使用了
                        start_newton_kernel = true;
                    }
                    else
                    {
                        // PRINT("----------------------------------------");
                        // PRINT(connected_layers_num[0]);
                        // PRINT(connected_layers_num[1]);
                        // PRINT("----------------------------------------");
                        connected_layers_num[0] = connected_layers_num[1];
                        connected_layers_num[1] = current_connected_component;
                    }
                }
                else if (has_computed_layers == 2)
                {
                    //计算了两层
                    connected_layers_num[1] = classify_data.size();
                }
                else
                {
                    //只计算了第一层
                    connected_layers_num[0] = classify_data.size();
                }
                auto end_connected = system_clock::now();
                auto connected_duration = duration_cast<microseconds>(end_connected - start_connected);
                connected_time += double(connected_duration.count()) * microseconds::period::num / microseconds::period::den;
                if (start_newton_kernel)
                {
                    auto newton_start_time2 = system_clock::now();
                    for (map<string, vector<Region>>::iterator conclusion_iter = classify_data.begin(); conclusion_iter != classify_data.end(); ++conclusion_iter)
                    {
                        newton_kernel++;
                        if (!conclusion_iter->second.empty())
                        {
                            handle_connected_region->submit(conclusion_iter->second);
                            //开始使用牛顿方法进行判断
                            newton_method->submit(conclusion_iter->second, handle_connected_region->newton_initial);
                            if (newton_method->finish_iter && !solu_container->ContainsCurrentSolu(newton_method->guess_solu))
                            {
                                //可以判断是否收敛了
                                bool level2 = true; //第二粗糙的标准，当前区域的包围壳
                                bool level3 = true; //当两者都无法奏效的时候，再次缩短壳大小，以该近似点为中心，精度为半径内进行估计
                                if (level2)
                                {
                                    //启用第二粗糙的标准
                                    //获取当前区域大小
                                    double cur_size = conclusion_iter->second[0].m_xr - conclusion_iter->second[0].m_xl;
                                    Region cur_region(newton_method->guess_solu.rx - cur_size,
                                                      newton_method->guess_solu.rx + cur_size,
                                                      newton_method->guess_solu.ry - cur_size,
                                                      newton_method->guess_solu.ry + cur_size);
                                    M_solver->submit(cur_region);
                                    //重新判断
                                    kantorovich_judge->submit(newton_method->guess_solu, cur_region, M_solver->getMaxValue());
                                    if (kantorovich_judge->is_convergent)
                                    {
                                        solu_container->SubmitSoluAndCoveredRegion(newton_method->guess_solu, kantorovich_judge->covered_region);
                                    }
                                    else
                                    {
                                        level2 = false;
                                        if (level3)
                                        {
                                            double cur_size = 2 * m_precision;
                                            Region cur_region1(newton_method->guess_solu.rx - cur_size,
                                                               newton_method->guess_solu.rx + cur_size,
                                                               newton_method->guess_solu.ry - cur_size,
                                                               newton_method->guess_solu.ry + cur_size);
                                            M_solver->submit(cur_region1);
                                            //重新判断
                                            kantorovich_judge->submit(newton_method->guess_solu, cur_region1, M_solver->getMaxValue());
                                            if (kantorovich_judge->is_convergent)
                                            {
                                                solu_container->SubmitSoluAndCoveredRegion(newton_method->guess_solu, kantorovich_judge->covered_region);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    auto newton_end_time2 = system_clock::now();
                    auto newton_duration2 = duration_cast<microseconds>(newton_end_time2 - newton_start_time2);
                    newton_time += double(newton_duration2.count()) * microseconds::period::num / microseconds::period::den;
                }

                delete[] m_parent;
                delete[] src2current;
                m_parent = nullptr;
                src2current = nullptr;
                current->m_region.index = 0;
                connected_data.clear();
                id2index_map.clear();
                connected_data.push_back(make_pair(current->m_id, current->m_region));
                id2index_map.insert(make_pair(current->m_id, current->m_region.index));
                map_index = 1;
                level = cur_level;
                //已经计算的层数增加
                has_computed_layers++;
                //对新一层的进行总结
            }
            kernel_point = new KernelBezier(current);
            if (kernel_point->need_subdivision())
            {
                delete kernel_point;
                kernel_point = nullptr;
                kernel_point = new KernelResultant(current);
                if (kernel_point->need_subdivision())
                {
                    delete kernel_point;
                    kernel_point = nullptr;
                    kernel_point = new KernelMinSize(current);
                    if (kernel_point->need_subdivision())
                    {
                        //还未达到最小，需要进一步操作-----------------------------------subdivision
                        delete kernel_point;
                        kernel_point = nullptr;
                        subdivision(current, queue_);
                    }
                    else
                    {
                        delete kernel_point;
                        kernel_point = nullptr;
                        unknown_region++;
                        m_unknown_regions.push_back(make_pair(current->m_id, current->m_region));
                        unknown_id2index_map.insert(make_pair(current->m_id, unknown_map_index));
                        unknown_map_index++;
                        continue;
                    }
                }
                else
                {
                    delete kernel_point;
                    kernel_point = nullptr;
                    continue;
                }
            }
            else
            {
                delete kernel_point;
                kernel_point = nullptr;
                continue;
            }
        }
    }
    delete connected_layers_num;
    connected_layers_num = nullptr;
    //对最后的未定区域进行最后一次判断
    if (!m_unknown_regions.empty() && !solu_container->m_solu_list.empty())
    {
        sort(m_unknown_regions.begin(), m_unknown_regions.end(), VectorSort());
        const int unknown_size = m_unknown_regions.size();

        //获取排序之后和排序之前的坐标对应
        int *src2current = new int[unknown_size]{0}; //将原来的编号映射成当前的编号
        for (int i = 0; i < unknown_size; ++i)
        {
            src2current[connected_data[i].second.index] = i;
        }
        //初始化parent标号
        int *m_parent = new int[unknown_size]{-1};

        for (int i = 0; i < unknown_size; ++i)
        {

            //得到上左邻居
            /*
                    R  -> 0         right
                    L  -> 1         left
                    D  -> 2         down
                    U  -> 3         up
                    RU -> 4         right + up
                    RD -> 5         right + down
                    LD -> 6         left + down
                    LU -> 7         left + up
                    halt -> 8       no direction
                */
            string cur_id = connected_data[i].first;
            string U_neighbor = get_neighbor(cur_id, 3);
            string L_neighbor = get_neighbor(cur_id, 1);
            int l_msg = -1, u_msg = -1;
            int l_msg_index = -1;
            int u_msg_index = -1;
            map<string, int>::iterator neighbor_iter;
            neighbor_iter = unknown_id2index_map.find(L_neighbor);
            if (neighbor_iter != unknown_id2index_map.end())
            {
                assert(neighbor_iter->second < unknown_size);
                assert(src2current[neighbor_iter->second] < unknown_size);
                l_msg_index = src2current[neighbor_iter->second];
                l_msg = m_parent[l_msg_index]; //先找到string 对应的原来的编号，再找到原来编号在当前的位置，最后查找当前位置的根节点
            }
            neighbor_iter = unknown_id2index_map.find(U_neighbor);
            if (neighbor_iter != unknown_id2index_map.end())
            {
                u_msg_index = src2current[neighbor_iter->second];
                u_msg = m_parent[u_msg_index];
            }
            if (l_msg >= 0 || u_msg >= 0)
            {
                if (l_msg >= 0 && u_msg >= 0)
                {
                    assert(l_msg < unknown_size);
                    assert(u_msg < unknown_size);
                    m_parent[i] = l_msg < u_msg ? l_msg : u_msg;
                    if (l_msg <= u_msg)
                    {
                        unionElements(u_msg_index, l_msg_index, m_parent);
                    }
                    else
                    {
                        unionElements(l_msg_index, u_msg_index, m_parent);
                    }
                }
                else
                {
                    int temp = l_msg > u_msg ? l_msg : u_msg;
                    m_parent[i] = temp;
                }
            }
            else
            {
                m_parent[i] = i;
            }
        }
        map<string, vector<Region>> classify_data;
        classify_data.clear();
        map<string, vector<Region>>::iterator classify_iter;
        for (int i = 0; i < unknown_size; ++i)
        {
            classify_iter = classify_data.find(to_string(m_parent[i]));
            if (classify_iter != classify_data.end())
            {
                //说明里面有这个标签
                classify_data[to_string(m_parent[i])].push_back(connected_data[i].second);
            }
            else
            {
                //说明里面没有这个标签
                vector<Region> newlabellist;
                newlabellist.clear();
                classify_data.insert(make_pair(to_string(m_parent[i]), newlabellist));
                classify_data[to_string(m_parent[i])].push_back(connected_data[i].second);
            }
        }
        for (map<string, vector<Region>>::iterator conclusion_iter = classify_data.begin(); conclusion_iter != classify_data.end(); ++conclusion_iter)
        {
            newton_kernel++;
            if (!conclusion_iter->second.empty())
            {
                handle_connected_region->submit(conclusion_iter->second, true);
                //开始使用牛顿方法进行判断
                newton_method->submit(handle_connected_region->newton_initial);
                if (newton_method->finish_iter)
                {
                    int soluindex = solu_container->getSoluIndexByContainCurrentSolu(newton_method->guess_solu);
                    if (soluindex >= 0)
                    {
                        //说明得到的是一个旧解
                        assert(soluindex < solu_container->m_solu_list.size());
                        Point old_solu = solu_container->m_solu_list.at(soluindex);
                        //再把这个连通区域包含进去
                        double cur_size = old_solu.max_norm(handle_connected_region->center_point) + handle_connected_region->m_square_size;
                        Region cur_region(old_solu.rx - cur_size,
                                          old_solu.rx + cur_size,
                                          old_solu.ry - cur_size,
                                          old_solu.ry + cur_size);
                        M_solver->submit(cur_region);
                        kantorovich_judge->submit(old_solu, cur_region, M_solver->getMaxValue());
                        if (kantorovich_judge->is_convergent)
                        {
                            if (kantorovich_judge->rho2star < cur_size)
                            {

                                //说明没有覆盖这块区域，这块区域为未定区域
                                for (vector<Region>::iterator unknowniter = conclusion_iter->second.begin(); unknowniter != conclusion_iter->second.end(); ++unknowniter)
                                {
                                    m_real_unknown_region.push_back(*unknowniter);
                                }
                            }
                        }
                    }
                }
            }
        }
        delete[] m_parent;
        delete[] src2current;
        m_parent = nullptr;
        src2current = nullptr;
    }
}
void AlgorithmBoundingBoxWolframScriptEngine::subdivision(NodeBoundingBox *&current, queue<NodeBoundingBox *> &queue_)
{
    auto bezier_start = system_clock::now();
    //实施分割算法
    Subdivision *sub_solver = new Subdivision(current->m_minus_mat);
    Subdivision *sub_solver1 = new Subdivision(current->m_f1_mat);
    Subdivision *sub_solver2 = new Subdivision(current->m_f2_mat);
    auto bezier_end = system_clock::now();
    auto bezier_duration = duration_cast<microseconds>(bezier_end - bezier_start);
    bezier_time += double(bezier_duration.count()) * microseconds::period::num / microseconds::period::den;

    auto resultant_start = system_clock::now();
    std::deque<kv::interval<double>> left;
    std::deque<kv::interval<double>> right;
    std::deque<kv::interval<double>> top;
    std::deque<kv::interval<double>> bottom;
    UnivariateSubdivision *univar_subdicision1 = new UnivariateSubdivision(current->m_bezier_x);
    UnivariateSubdivision *univar_subdicision2 = new UnivariateSubdivision(current->m_bezier_y);
    left = univar_subdicision1->left_list;
    right = univar_subdicision1->right_list;
    bottom = univar_subdicision2->left_list;
    top = univar_subdicision2->right_list;
    auto resultant_end = system_clock::now();
    auto resultant_duration = duration_cast<microseconds>(resultant_end - resultant_start);
    resultant_time += double(resultant_duration.count()) * microseconds::period::num / microseconds::period::den;
    current->child1 = new NodeBoundingBox((current->m_region.m_xl + current->m_region.m_xr) / 2,
                                          current->m_region.m_xr,
                                          (current->m_region.m_yl + current->m_region.m_yr) / 2,
                                          current->m_region.m_yr,
                                          sub_solver->child1_matrix,
                                          sub_solver1->child1_matrix,
                                          sub_solver2->child1_matrix,
                                          right,
                                          top,
                                          current->m_id + "1",
                                          current->m_label);
    current->child2 = new NodeBoundingBox(current->m_region.m_xl,
                                          (current->m_region.m_xl + current->m_region.m_xr) / 2,
                                          (current->m_region.m_yr + current->m_region.m_yl) / 2,
                                          current->m_region.m_yr,
                                          sub_solver->child2_matrix,
                                          sub_solver1->child2_matrix,
                                          sub_solver2->child2_matrix,
                                          left,
                                          top,
                                          current->m_id + "0",
                                          current->m_label);
    current->child3 = new NodeBoundingBox(current->m_region.m_xl,
                                          (current->m_region.m_xl + current->m_region.m_xr) / 2,
                                          current->m_region.m_yl,
                                          (current->m_region.m_yl + current->m_region.m_yr) / 2,
                                          sub_solver->child3_matrix,
                                          sub_solver1->child3_matrix,
                                          sub_solver2->child3_matrix,
                                          left,
                                          bottom,
                                          current->m_id + "2",
                                          current->m_label);
    current->child4 = new NodeBoundingBox((current->m_region.m_xl + current->m_region.m_xr) / 2,
                                          current->m_region.m_xr,
                                          current->m_region.m_yl,
                                          (current->m_region.m_yl + current->m_region.m_yr) / 2,
                                          sub_solver->child4_matrix,
                                          sub_solver1->child4_matrix,
                                          sub_solver2->child4_matrix,
                                          right,
                                          bottom,
                                          current->m_id + "3",
                                          current->m_label);
    queue_.push(current->child1);
    queue_.push(current->child2);
    queue_.push(current->child3);
    queue_.push(current->child4);
    delete sub_solver;
    delete sub_solver1;
    delete sub_solver2;
    delete univar_subdicision1;
    delete univar_subdicision2;
    sub_solver = nullptr;
    sub_solver1 = nullptr;
    sub_solver2 = nullptr;
    univar_subdicision1 = nullptr;
    univar_subdicision2 = nullptr;
}
string AlgorithmBoundingBoxWolframScriptEngine::get_neighbor(const string &src, int direction)
{
    string result(src);
    int index = src.size() - 1;
    pair<int, int> current;

    while (index >= 0)
    {
        current = FSM_table[direction * 4 + src.at(index) - '0'];
        result[index] = current.first + '0';
        if (current.second == 8)
        {
            break;
        }
        else
        {
            direction = current.second;
            index--;
        }
    }
    return result;
}
void AlgorithmBoundingBoxWolframScriptEngine::unionElements(int p, int q, int parent[])
{
    // int i = p;
    // int j = q;
    // while (0 != parent[i])
    // {
    //     i = parent[i];
    // }
    // while (0 != parent[j])
    // {
    //     j = parent[j];
    // }
    // if (i != j)
    // {
    //     parent[i] = j;
    // }
    int pRoot = find_root(p, parent);
    int qRoot = find_root(q, parent);
    if (pRoot == qRoot)
        return;
    parent[pRoot] = qRoot;
}
int AlgorithmBoundingBoxWolframScriptEngine::find_root(int p, int parent[])
{
    // int i = p;
    // while (0 != parent[i])
    // {
    //     i = parent[i];
    // }
    // return i;
    if (p != parent[p])
        parent[p] = find_root(parent[p], parent);
    return parent[p];
}