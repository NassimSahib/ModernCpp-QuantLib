#include <Eigen/Dense>

using Eigen::MatrixXd;

MatrixXd corr_mtx
{
    {1.0, 0.5, 0.25},
    {0.5, 1.0, -0.7},
    {0.25, -0.7, 1.0}
};
VectorXd vols{ {0.2, 0.1, 0.4} };

MatrixXd cov_mtx = vols.asDiagonal() * corr_mtx * vols.asDiagonal();

VectorXd fund_weights{ {0.6, -0.3, 0.7} };

double port_vol = std::sqrt(fund_weights.transpose() * cov_mtx * fund_weights);
