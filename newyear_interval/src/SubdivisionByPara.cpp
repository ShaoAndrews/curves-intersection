#include "SubdivisionByPara.h"

SubdivisionByPara::SubdivisionByPara(const GiNaC::ex &f1, const GiNaC::ex &f2, double g_xl_, double g_xr_, double g_yl_, double g_yr_) : g_xl(g_xl_),
                                                                                                                                         g_xr(g_xr_),
                                                                                                                                         g_yl(g_yl_),
                                                                                                                                         g_yr(g_yr_)
{
    ExToMat *ex2mat1 = new ExToMat(f1);
    MatToBezierFloatingPoint *mat2bezier1 = new MatToBezierFloatingPoint(ex2mat1->mat);
    g_mat1 = mat2bezier1->coffimat;
    delete mat2bezier1;
    mat2bezier1 = nullptr;
    delete ex2mat1;
    ex2mat1 = nullptr;

    ExToMat *ex2mat2 = new ExToMat(f2);
    MatToBezierFloatingPoint *mat2bezier2 = new MatToBezierFloatingPoint(ex2mat2->mat);
    g_mat2 = mat2bezier2->coffimat;
    delete mat2bezier2;
    mat2bezier2 = nullptr;
    delete ex2mat2;
    ex2mat2 = nullptr;
}

SubdivisionByPara::~SubdivisionByPara()
{
}
void SubdivisionByPara::submit(double c_xl, double c_xr, double c_yl, double c_yr)
{
    M = 0;
    double leftx, rightx, lefty, righty;
    /*
        [g_xl, c_xl, g_xr]         Left, Right
        [c_xl, c_xr, g_xr]         RightLeft, RightRight
        [g_yl, c_yl, g_yr]         Top, Bottom
        [c_yl, c_yr, g_xr]         BottomTop, BottomBottom
    */
    leftx = (c_xl - g_xl) / (g_xr - g_xl);
    rightx = (c_xr - c_xl) / (g_xr - c_xl);
    lefty = (c_yl - g_yl) / (g_yr - g_yl);
    righty = (c_yr - c_yl) / (g_xr - c_yl);
    double M1, M2;
    Eigen::MatrixXd src1 = g_mat1;
    int num_count;
    Eigen::MatrixXd Left1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd Right1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd RightLeft1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd RightRight1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd Top1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd Bottom1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd TopTop1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd TopBottom1(g_mat1.rows(), g_mat1.cols());
    //计算关于f1的系数
    num_count = g_mat1.rows();
    SubdivisionByX(src1, Left1, Right1, num_count, leftx);
    num_count = g_mat1.rows();
    SubdivisionByX(Right1, RightLeft1, RightRight1, num_count, rightx);
    num_count = g_mat1.cols();
    SubdivisionByY(RightLeft1, Top1, Bottom1, num_count, lefty);
    num_count = g_mat1.cols();
    SubdivisionByY(Top1, TopTop1, TopBottom1, num_count, righty);
    M1 = TopBottom1.maxCoeff();
    //计算关于f2的系数
    Eigen::MatrixXd src2 = g_mat2;
    Eigen::MatrixXd Left2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd Right2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd RightLeft2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd RightRight2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd Top2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd Bottom2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd TopTop2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd TopBottom2(g_mat2.rows(), g_mat2.cols());
    num_count = g_mat2.rows();
    SubdivisionByX(src2, Left2, Right2, num_count, leftx);
    num_count = g_mat2.rows();
    SubdivisionByX(Right2, RightLeft2, RightRight2, num_count, rightx);
    num_count = g_mat2.cols();
    SubdivisionByY(RightLeft2, Top2, Bottom2, num_count, lefty);
    num_count = g_mat2.cols();
    SubdivisionByY(Top2, TopTop2, TopBottom2, num_count, righty);
    M2 = TopBottom2.maxCoeff();
    M = (M1 > M2) ? M1 : M2;
}
void SubdivisionByPara::submit(const Region &region)
{
    M = 0;
    double c_xl = region.m_xl;
    double c_xr = region.m_xr;
    double c_yl = region.m_yl;
    double c_yr = region.m_yr;
    double leftx, rightx, lefty, righty;
    /*
        [g_xl, c_xl, g_xr]         Left, Right
        [c_xl, c_xr, g_xr]         RightLeft, RightRight
        [g_yl, c_yl, g_yr]         Top, Bottom
        [c_yl, c_yr, g_xr]         BottomTop, BottomBottom
    */
    leftx = (c_xl - g_xl) / (g_xr - g_xl);
    rightx = (c_xr - c_xl) / (g_xr - c_xl);
    lefty = (c_yl - g_yl) / (g_yr - g_yl);
    righty = (c_yr - c_yl) / (g_xr - c_yl);
    double M1, M2;
    Eigen::MatrixXd src1 = g_mat1;
    int num_count;
    Eigen::MatrixXd Left1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd Right1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd RightLeft1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd RightRight1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd Top1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd Bottom1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd TopTop1(g_mat1.rows(), g_mat1.cols());
    Eigen::MatrixXd TopBottom1(g_mat1.rows(), g_mat1.cols());
    //计算关于f1的系数
    num_count = g_mat1.rows();
    SubdivisionByX(src1, Left1, Right1, num_count, leftx);
    num_count = g_mat1.rows();
    SubdivisionByX(Right1, RightLeft1, RightRight1, num_count, rightx);
    num_count = g_mat1.cols();
    SubdivisionByY(RightLeft1, Top1, Bottom1, num_count, lefty);
    num_count = g_mat1.cols();
    SubdivisionByY(Top1, TopTop1, TopBottom1, num_count, righty);
    M1 = TopBottom1.maxCoeff();
    //计算关于f2的系数
    Eigen::MatrixXd src2 = g_mat2;
    Eigen::MatrixXd Left2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd Right2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd RightLeft2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd RightRight2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd Top2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd Bottom2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd TopTop2(g_mat2.rows(), g_mat2.cols());
    Eigen::MatrixXd TopBottom2(g_mat2.rows(), g_mat2.cols());
    num_count = g_mat2.rows();
    SubdivisionByX(src2, Left2, Right2, num_count, leftx);
    num_count = g_mat2.rows();
    SubdivisionByX(Right2, RightLeft2, RightRight2, num_count, rightx);
    num_count = g_mat2.cols();
    SubdivisionByY(RightLeft2, Top2, Bottom2, num_count, lefty);
    num_count = g_mat2.cols();
    SubdivisionByY(Top2, TopTop2, TopBottom2, num_count, righty);
    M2 = TopBottom2.maxCoeff();
    M = (M1 > M2) ? M1 : M2;
}
void SubdivisionByPara::SubdivisionByX(Eigen::MatrixXd &source, Eigen::MatrixXd &left_mat, Eigen::MatrixXd &right_mat, int &numRows, double rate)
{
    int totalRows = left_mat.rows();
    if (numRows == 1)
    {
        left_mat.row(totalRows - 1) = source.row(0);
        right_mat.row(0) = source.row(0);
    }
    else
    {
        left_mat.row(totalRows - numRows) = source.row(0);
        right_mat.row(numRows - 1) = source.row(numRows - 1);
        for (int i = 0; i < numRows - 1; i++)
        {
            source.row(i) = source.row(i) * (1 - rate) + source.row(i + 1) * rate;
        }
        numRows = numRows - 1;
        SubdivisionByX(source, left_mat, right_mat, numRows, rate);
    }
}
void SubdivisionByPara::SubdivisionByY(Eigen::MatrixXd &source, Eigen::MatrixXd &top_mat, Eigen::MatrixXd &bottom_mat, int &numCols, double rate)
{
    int totalCols = top_mat.cols();
    if (numCols == 1)
    {
        bottom_mat.col(totalCols - 1) = source.col(0);
        top_mat.col(0) = source.col(0);
    }
    else
    {
        bottom_mat.col(totalCols - numCols) = source.col(0);
        top_mat.col(numCols - 1) = source.col(numCols - 1);
        for (int i = 0; i < numCols - 1; i++)
        {
            source.col(i) = source.col(i) * (1 - rate) + source.col(i + 1) * rate;
        }
        numCols = numCols - 1;
        SubdivisionByY(source, top_mat, bottom_mat, numCols, rate);
    }
}
double SubdivisionByPara::getMaxValue()
{
    return M;
}