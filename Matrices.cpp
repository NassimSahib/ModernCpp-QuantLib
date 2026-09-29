#include <iostream>
#include <Eigen/Dense>

// Créez une fonction (par exemple "testMatrices" ou utilisez votre "main")
void executerCalculs()
{
    // 1. Déclaration et remplissage de la matrice 3x3 (double)
    Eigen::Matrix3d dbl_mtx;
    dbl_mtx << 10.64, 41.28, 21.63,
        41.95, 87.45, 13.68,
        22.47, 57.34, 8.631;

    // 2. Déclaration et remplissage de la matrice 4x4 (int)
    Eigen::Matrix4i int_mtx;
    int_mtx << 24, 0, 23, 13,
        8, 75, 0, 98,
        11, 60, 1, 3,
        422, 55, 11, 55;

    // Optionnel : Afficher la matrice dans la console pour vérifier
    std::cout << "Matrice double :\n" << dbl_mtx << "\n";
}
