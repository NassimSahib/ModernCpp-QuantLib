#pragma once

struct Node {
    double underlying{ 0.0 }; // Le prix du sous-jacent (ex: grid_[i][j].underlying)
    double payoff{ 0.0 };     // Le gain/valeur de l'option (ex: grid_[i][j].payoff)
};

