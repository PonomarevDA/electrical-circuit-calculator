#ifndef MATRIX
#define MATRIX

#include <types.hpp>
#include <vector>


// Умножение матриц
// param T left - левая матрица
// param T right - правая матрица
// param T& result - результат матрица
// return error - номер ошибки
template <typename T>
error_t multiply(T left, T right, T& result)
{
	uint8_t resultRows = result.size();
	uint8_t resultCols = result[0].size();
	uint8_t leftCols = right.size();

	// Check dimension:
	if(left[0].size() != right.size())
		return DIMENSION_ERROR;

	// Multiply matrix:
	for(uint8_t row = 0; row < resultRows; row++)
	{
		for(uint8_t col = 0; col < resultCols; col++)
		{
			result[row][col] = 0;
			for(uint8_t count = 0; count < leftCols; count++)
				result[row][col] += left[row][count]*right[count][col];
		}
	}
	return OK;
}

// Транспонирование матрицы

// Обратная матрица

#endif // MATRIX

