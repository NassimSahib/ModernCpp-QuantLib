#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <random>
#include <iterator>
#include <vector>
#include <array>

int main() {
    std::vector<double> pnl
    {
        -149'299.30,  -673'165.13,   3'891'123.79,  1'061'346.21,   -578'464.00,
        -260'855.99,   1'102'167.76,   509'764.96,   -276'786.46,    -11'947.13,
        -277'781.70,  -318'603.05,      57'747.06,    151'336.99,    267'826.14,
        -198'132.05,  -175'232.84,      -5'973.61,    177'224.51,   -711'878.76,
        -276'786.56,   116'489.15,    -185'188.30,  1'183'810.47,  1'563'147.38,
         -83'632.98,   527'687.09,    -307'650.96,   -321'589.76, -1'434'711.00,
         742'743.96,  -245'921.36,    -198'131.26,    399'250.02,   -311'632.90,
         326'569.83,   437'084.67,    -297'694.80,   -379'336.81,   -173'240.27,
         -62'724.74,  -363'407.14,    -142'375.97,   -103'546.19,    187'179.27,
        -161'293.14,  -131'423.78,   1'195'759.19,    198'131.95,   -229'991.59,
        -109'519.00,  -148'348.69,   1'447'621.95
    };

    // 1. Calcul du Backtest Initial
    std::vector<double> cum_pl(pnl.size());
    std::partial_sum(pnl.begin(), pnl.end(), cum_pl.begin());

    double peak = cum_pl.front();
    std::vector<double> drawdowns;
    drawdowns.reserve(cum_pl.size());
    drawdowns.push_back(0.0);

    for (auto pos = cum_pl.begin() + 1; pos != cum_pl.end(); ++pos) {
        if (peak < *pos) {
            peak = *pos;
        }
        drawdowns.push_back(peak - *pos);
    }

    double max_dd = *std::ranges::max_element(drawdowns);
    double net_pl_over_max_dd = cum_pl.back() / max_dd;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Max DD from backtest = $" << max_dd
        << ", Net P/L = $" << cum_pl.back()
        << ", (Net P/L)/MaxDD = " << net_pl_over_max_dd << "\n\n";

    // 2. Définition de la Lambda conforme au livre
    auto max_dd_lam = [](const std::vector<double>& v) {
        std::vector<double> local_cum_pl;
        local_cum_pl.reserve(v.size());
        double local_max_dd = 0.0;

        std::partial_sum(v.begin(), v.end(), std::back_inserter(local_cum_pl));
        double local_peak = local_cum_pl.front();

        for (auto pos = local_cum_pl.cbegin() + 1; pos != local_cum_pl.cend(); ++pos) {
            if (*pos < local_peak) {
                local_max_dd = std::max(local_peak - *pos, local_max_dd);
            }
            else if (local_peak < *pos) {
                local_peak = *pos;
            }
        }
        return local_max_dd;
        };

    // 3. Simulations de Monte Carlo
    unsigned n = 100;
    std::vector<double> max_drawdowns;
    max_drawdowns.reserve(n);
    max_drawdowns.push_back(max_dd);

    std::mt19937_64 mt{ 10 };

    for (unsigned k = 0; k < n - 1; ++k) {
        std::ranges::shuffle(pnl, mt);
        max_drawdowns.push_back(max_dd_lam(pnl));
    }

    // 4. Affichage des résultats des simulations
    auto print_dec_form = [](double x) {
        std::cout << std::fixed << std::setprecision(2) << x << " ";
        };
    std::ranges::for_each(max_drawdowns, print_dec_form);
    std::cout << "\n";

    auto norm_params = [](const std::vector<double>& v) -> std::array<double, 2>
        {
            double mean = (1.0 / v.size()) * std::accumulate(v.begin(), v.end(), 0.0);
            double sum_sq = 0.0;
            for (double val : v)
            {
                sum_sq += (val - mean) * (val - mean);
            }
            return std::array<double, 2>
            {mean, (1.0 / std::sqrt(v.size()))* std::sqrt(sum_sq)};
        };
    double mean = norm_params(max_drawdowns)[0];
    double sd = norm_params(max_drawdowns)[1];

    const double z_val_95 = 1.64485;
    double upper_conf_intvl_norm = mean + sd * z_val_95;

    // Monte Carlo risk modelling
    std::ranges::sort(max_drawdowns);

    // Assume alpha is taken in as user input:
    double alpha = 0.05;        // Upper 5%-tile
    // Rounding function from C++11:
    long upper_loc = std::lround(alpha * max_drawdowns.size());
    double max_dd_conf_lev = *(max_drawdowns.end() - upper_loc - 1);

}
