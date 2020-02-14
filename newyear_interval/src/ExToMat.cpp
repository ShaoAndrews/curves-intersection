#include"ExToMat.h"

ExToMat::ExToMat(const GiNaC::ex  &f_)
{
    int x_degree = f_.degree(x);
    int y_degree = f_.degree(y);
    mat = Eigen::MatrixXd(x_degree+1, y_degree+1);
    for(int i = 0; i<= x_degree; ++i){
        for(int j=0; j<=y_degree;++j){
            mat(i, j) = GiNaC::ex_to<GiNaC::numeric>(f_.coeff(x, i).coeff(y, j)).to_double();
        }
    }
}

ExToMat::~ExToMat()
{
}
