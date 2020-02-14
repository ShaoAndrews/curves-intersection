#include "Algorithm.h"

Algorithm::Algorithm(const Eigen::MatrixXd &f1_mat, const Eigen::MatrixXd &f2_mat, double precision, double xl_, double xr_, double yl_, double yr_) : m_precision(precision)
{
    //先转化为表达式类型
    MatToEx *mat2ex_f1 = new MatToEx(f1_mat);
    MatToEx *mat2ex_f2 = new MatToEx(f2_mat);
    m_f1 = mat2ex_f1->f;
    m_f2 = mat2ex_f2->f;
    delete mat2ex_f1;
    delete mat2ex_f2;
    mat2ex_f1 = nullptr;
    mat2ex_f2 = nullptr;
    //初始化次数
    m_f1_x_deg = m_f1.degree(x);
    m_f1_y_deg = m_f1.degree(y);
    m_f2_x_deg = m_f2.degree(x);
    m_f2_y_deg = m_f2.degree(y);
    //计算二阶导数和一阶导数
    m_df1_x = m_f1.diff(x);
    m_df1_y = m_f1.diff(y);
    m_df2_x = m_f2.diff(x);
    m_df2_y = m_f2.diff(y);
    GiNaC::ex df1_xx = m_f1.diff(x, 2);
    GiNaC::ex df1_xy = m_f1.diff(x).diff(y);
    GiNaC::ex df1_yy = m_f1.diff(y, 2);
    GiNaC::ex df2_xx = m_f2.diff(x, 2);
    GiNaC::ex df2_xy = m_f2.diff(x).diff(y);
    GiNaC::ex df2_yy = m_f2.diff(y, 2);
    m_hessian_f1 = GiNaC::expand(GiNaC::pow(df1_xx, 2) + 2 * GiNaC::pow(df1_xy, 2) + GiNaC::pow(df1_yy, 2));
    m_hessian_f2 = GiNaC::expand(GiNaC::pow(df2_xx, 2) + 2 * GiNaC::pow(df2_xy, 2) + GiNaC::pow(df2_yy, 2));
    //将六个转化为字符串的形式
    ExToMat *df1_xx_solver = new ExToMat(df1_xx);
    ExToMat *df1_xy_solver = new ExToMat(df1_xy);
    ExToMat *df1_yy_solver = new ExToMat(df1_yy);

    ExToMat *df2_xx_solver = new ExToMat(df2_xx);

    ExToMat *df2_xy_solver = new ExToMat(df2_xy);

    ExToMat *df2_yy_solver = new ExToMat(df2_yy);
    Eigen::MatrixXd df1_xx_mat = df1_xx_solver->mat;
    Eigen::MatrixXd df1_xy_mat = df1_xy_solver->mat;
    Eigen::MatrixXd df1_yy_mat = df1_yy_solver->mat;
    Eigen::MatrixXd df2_xx_mat = df2_xx_solver->mat;
    Eigen::MatrixXd df2_xy_mat = df2_xy_solver->mat;
    Eigen::MatrixXd df2_yy_mat = df2_yy_solver->mat;
    delete df1_xx_solver;
    df1_xx_solver = nullptr;
    delete df1_xy_solver;
    df1_xy_solver = nullptr;
    delete df1_yy_solver;
    df1_yy_solver = nullptr;
    delete df2_xx_solver;
    df2_xx_solver = nullptr;
    delete df2_xy_solver;
    df2_xy_solver = nullptr;
    delete df2_yy_solver;
    df2_xy_solver = nullptr;
    MatToString *df1_xx_str_solver = new MatToStringCFormat(df1_xx_mat);
    MatToString *df1_xy_str_solver = new MatToStringCFormat(df1_xy_mat);
    MatToString *df1_yy_str_solver = new MatToStringCFormat(df1_yy_mat);
    MatToString *df2_xx_str_solver = new MatToStringCFormat(df2_xx_mat);
    MatToString *df2_xy_str_solver = new MatToStringCFormat(df2_xy_mat);
    MatToString *df2_yy_str_solver = new MatToStringCFormat(df2_yy_mat);

    df1_xx_str = df1_xx_str_solver->get_expression();
    df1_xy_str = df1_xy_str_solver->get_expression();

    df1_yy_str = df1_yy_str_solver->get_expression();

    df2_xx_str = df2_xx_str_solver->get_expression();

    df2_xy_str = df2_xy_str_solver->get_expression();

    df2_yy_str = df2_yy_str_solver->get_expression();
    delete df1_xx_str_solver;
    df1_xx_str_solver = nullptr;
    delete df1_xy_str_solver;
    df1_xy_str_solver = nullptr;
    delete df1_yy_str_solver;
    df1_yy_str_solver = nullptr;
    delete df2_xx_str_solver;
    df2_xx_str_solver = nullptr;
    delete df2_xy_str_solver;
    df2_xy_str_solver = nullptr;
    delete df2_yy_str_solver;
    df2_yy_str_solver = nullptr;

    ExToMat *hessian1 = new ExToMat(m_hessian_f1);
    ExToMat *hessian2 = new ExToMat(m_hessian_f2);
    Eigen::MatrixXd hessian1_mat = hessian1->mat;
    Eigen::MatrixXd hessian2_mat = hessian2->mat;

    MatToString *hessian_str_solver1 = new MatToStringCFormat(hessian1_mat);
    MatToString *hessian_str_solver2 = new MatToStringCFormat(hessian2_mat);
    hessian_f1_str = hessian_str_solver1->get_expression();
    hessian_f2_str = hessian_str_solver2->get_expression();
    delete hessian1;
    delete hessian2;
    delete hessian_str_solver1;
    delete hessian_str_solver2;
    hessian1 = nullptr;
    hessian2 = nullptr;
    hessian_str_solver1 = nullptr;
    hessian_str_solver2 = nullptr;
}

Algorithm::~Algorithm()
{
}
