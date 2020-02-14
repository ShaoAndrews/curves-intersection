#include "Subdivision.h"
Subdivision::Subdivision(const IntervalMatrix &source_matrix_) : child1_matrix(source_matrix_.m_rows, source_matrix_.m_cols),
                                                                 child2_matrix(source_matrix_.m_rows, source_matrix_.m_cols),
                                                                 child3_matrix(source_matrix_.m_rows, source_matrix_.m_cols),
                                                                 child4_matrix(source_matrix_.m_rows, source_matrix_.m_cols),
                                                                 source_matrix(source_matrix_),
                                                                 x_num(source_matrix_.m_rows),
                                                                 y_num(source_matrix_.m_cols)
{
    //首先对行进行操作，将x坐标一分为二
    IntervalMatrix left_mat(source_matrix.m_rows, source_matrix.m_cols);
    IntervalMatrix right_mat(source_matrix.m_rows, source_matrix.m_cols);
    int control_nums = this->x_num;

    divide_by_x(source_matrix, left_mat, right_mat, control_nums);
    //再对y进行分割
    control_nums = this->y_num;
    divide_by_y(left_mat, child2_matrix, child3_matrix, control_nums);
    control_nums = this->y_num;
    divide_by_y(right_mat, child1_matrix, child4_matrix, control_nums);
}

Subdivision::~Subdivision()
{
}
void Subdivision::divide_by_x(IntervalMatrix &source, IntervalMatrix &left_mat, IntervalMatrix &right_mat, int &numRows)
{
    int totalRows = left_mat.rows();
    kv::interval<double> two;
    two = 2;
    if (numRows == 1)
    {
        // left_mat.row(totalRows - 1) = source.row(0);
        // right_mat.row(0) = source.row(0);
        //行赋值
        for (int j = 0; j < y_num; ++j)
        {
            left_mat(totalRows - 1, j) = source(0, j);
            right_mat(0, j) = source(0, j);
        }
    }
    else
    {
        // left_mat.row(totalRows - numRows) = source.row(0);
        // right_mat.row(numRows - 1) = source.row(numRows - 1);
        //行赋值
        for (int j = 0; j < y_num; ++j)
        {
            left_mat(totalRows - numRows, j) = source(0, j);
            right_mat(numRows - 1, j) = source(numRows - 1, j);
        }
        for (int i = 0; i < numRows - 1; i++)
        {
            // source.row(i) = (source.row(i) + source.row(i + 1)) / 2.0;
            for (int j = 0; j < y_num; ++j)
            {
                source(i, j) = (source(i, j) + source(i + 1, j)) / two;
            }
        }
        numRows = numRows - 1;
        divide_by_x(source, left_mat, right_mat, numRows);
    }
}
void Subdivision::divide_by_y(IntervalMatrix &source, IntervalMatrix &top_mat, IntervalMatrix &bottom_mat, int &numCols)
{
    int totalCols = top_mat.cols();
    kv::interval<double> two;
    two = 2;
    if (numCols == 1)
    {
        // bottom_mat.col(totalCols - 1) = source.col(0);
        // top_mat.col(0) = source.col(0);
        for (int j = 0; j < x_num; ++j)
        {
            bottom_mat(j, totalCols - 1) = source(j, 0);
            top_mat(j, 0) = source(j, 0);
        }
    }
    else
    {
        // bottom_mat.col(totalCols - numCols) = source.col(0);
        // top_mat.col(numCols - 1) = source.col(numCols - 1);
        for (int j = 0; j < x_num; ++j)
        {
            bottom_mat(j, totalCols - numCols) = source(j, 0);
            top_mat(j, numCols - 1) = source(j, numCols - 1);
        }
        for (int i = 0; i < numCols - 1; i++)
        {
            // source.col(i) = (source.col(i) + source.col(i + 1)) / 2.0;
            for (int j = 0; j < x_num; ++j)
            {
                source(j, i) = (source(j, i) + source(j, i + 1)) / two;
            }
        }
        numCols = numCols - 1;
        divide_by_y(source, top_mat, bottom_mat, numCols);
    }
}