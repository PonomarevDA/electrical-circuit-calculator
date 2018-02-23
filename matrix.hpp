#ifndef MATRIX
#define MATRIX

#include <types.hpp>
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>

// Вернуть значение определителя квадратной матрицы
template <typename T>
T determ(T** arr, const uint8_t size)
{
	uint8_t i,j;
	T det = 0;
	T** matr;
	if(size == 1)
	{
		det = arr[0][0];
	}
	else if(size == 2)
	{
		det = arr[0][0]*arr[1][1] - arr[0][1]*arr[1][0];
	}
	else
	{
		matr = new T*[size - 1];
		for(i = 0; i < size; i++)
		{
			for(j = 0; j < size-1; j++)
			{
				if(j < i)
					matr[j] = arr[j];
				else
					matr[j] = arr[j+1];
			}
			det+= pow((double)-1, (i+j))*determ(matr, size-1)*arr[i][size-1];
		}
		delete[] matr;
	}
	return det;
}


// Преобразование матрицы в единичную
template <typename T>
error_t identity(T** matrix, const uint8_t ROWS, const uint8_t COLS)
{
	error_t error = OK;
	uint8_t* bufferRow;
	bufferRow = new uint8_t[COLS];

	// Прямой ход
	for (uint8_t countConv = 0; countConv < ROWS; countConv++)
	{
		// Перестановка строк матрицы, если необходимо:
		if(matrix[countConv][countConv] == 0)
		{
			uint8_t countSort;
			memcpy(bufferRow, (*matrix + countConv*COLS), COLS - countConv);
			for(countSort = countConv + 1; countSort < ROWS; countSort++)
			{
				if(matrix[countSort][countConv] != 0)
				{
					memcpy((*matrix + countConv*COLS), (*matrix + countSort*COLS + countConv), COLS - countConv);
					memcpy((*matrix + countSort*COLS + countConv), bufferRow, COLS - countConv);
					break;
				}
			}
			if(countSort == ROWS - 1)
				error = IDENTITY_ERROR;
		}
		// Делим всю строку на первый её элемент:
		if(error == OK)
		{
			if(matrix[countConv][countConv] != 1)
			{
				T firstElement = matrix[countConv][countConv];
				for(uint8_t countDiv = countConv; countDiv < COLS; countDiv++)
					matrix[countConv][countDiv] /= firstElement;
			}
		}

		// Обнуляем элменты под номерами countConv для нижних строк:
		if(error == OK)
		{
			for(uint8_t rowZero = countConv + 1; rowZero < ROWS; rowZero++)
			{
				if(matrix[rowZero][countConv] != 0)
				{
					T coefficient = matrix[rowZero][countConv];
					for(uint8_t colZero = countConv; colZero < COLS; colZero++)
						matrix[rowZero][colZero] -= coefficient*matrix[countConv][colZero];
				}
			}
		}
	}
	// Обратный ход
	if(error == OK)
	{
		for (int8_t rowZero = ROWS - 2; rowZero >= 0; rowZero--)
		{
			for (uint8_t colZero = rowZero + 1; colZero < ROWS; colZero++)
			{
				if (matrix[rowZero][colZero] != 0)
				{
					T coefficient = matrix[rowZero][colZero];
					for (uint8_t colCount = colZero; colCount < COLS; colCount++)
						matrix[rowZero][colCount] -= coefficient*matrix[colZero][colCount];
				}
			}
		}
	}

	delete[] bufferRow;
	return OK;
}


// Умножение матриц
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
template <typename T>
error_t transpose(const T primary, T& total)
{
	uint8_t primaryRows = primary.size();
	uint8_t primaryCols = primary[0].size();
	uint8_t totalRows = total.size();
	uint8_t totalCols = total[0].size();

	// Check dimension:
	if(primaryRows  != totalCols || primaryCols  != totalRows)
		return DIMENSION_ERROR;

	// Transpose matrix:
	// Multiply matrix:
	for(uint8_t row = 0; row < totalRows; row++)
	{
		for(uint8_t col = 0; col < totalCols; col++)
			total[row][col] = primary[col][row];
	}
	return OK;
}

// Обратная матрица
template <typename T>
error_t inverse(T** matrix, const uint8_t ROWS, const uint8_t COLS)
{
	// Check dimension:
	if(ROWS != COLS)
		return DIMENSION_ERROR;

	// Check determinant
	if(determ(matrix, ROWS) == 0)
		return DETERMINANT_ERROR;

	// Init buffer
	uint8_t bufferRows = ROWS;
	uint8_t bufferCols = COLS << 1;
	double** buffer = new double*[bufferRows];
	for(uint8_t row = 0; row < ROWS; row++)
		buffer[row] = new double[bufferCols];

	// Main algorithm
	for(uint8_t row = 0; row < bufferRows; row++)
	{
		for(uint8_t col = 0; col < COLS; col++)
		{
			buffer[row][col] = matrix[row][col];
		}

		for(uint8_t col = COLS; col < bufferCols; col++)
		{
			if( (col - COLS) == row)
				buffer[row][col] = 1;
			else
				buffer[row][col] = 0;
		}
	}

	// Вывод буфера на экран
	for(uint8_t row = 0; row < bufferRows; row++)
	{
		for(uint8_t col = 0; col < bufferCols; col++)
			std::cout << buffer[row][col] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	identity(buffer, bufferRows, bufferCols);

	// Вывод буфера на экран
	for(uint8_t row = 0; row < bufferRows; row++)
	{
		for(uint8_t col = 0; col < bufferCols; col++)
			std::cout << buffer[row][col] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// Return
	for(uint8_t row = 0; row < ROWS; row++)
		memcpy(matrix[row], buffer[row] + COLS, COLS);
	return OK;
}



#endif // MATRIX

