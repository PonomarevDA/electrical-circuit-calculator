#ifndef TESTECC
#define TESTECC

#include <ECC.hpp>
#include <matrix.hpp>

void CalculatedExample();
void testMultiplyMatrix();
void testTransposeMatrix();
void testIdentityMatrix();
void testInverseMatrix();

// Вывод значний двумерного вектора в терминал
template <typename T>
void showMatrix(T matrix);

// Вывод значний двумерного массива в терминал
template <typename T>
void showMatrix(T** matrix, uint8_t ROWS, uint8_t COLS);

#endif // TESTECC

