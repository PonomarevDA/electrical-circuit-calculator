/**
* @file matrix.hpp
* @brief Implementation of class Matrix
*/

#include <matrix.hpp>
#include <iostream>
#include <cstring>	// for memcpy()
#include <math.h>	// for pow()


/*********************Public area*********************/
/**
* @brief Constructor
*/
template <class T>
Matrix<T>::Matrix()
{

}
template Matrix<double>::Matrix();


/**
* @brief Constructor with arguments
*/
template <class T>
Matrix<T>::Matrix(uint8_t rows, uint8_t cols) : ROWS(rows), COLS(cols)
{
	Data = new T*[ROWS];
	for(uint8_t count = 0; count < ROWS; count++)
		Data[count] = new T[COLS];
}


/**
* @brief Initialization
* @param rows - number of rows
* @param cols - number of columns
*/
template <class T>
void Matrix<T>::Init(uint8_t rows, uint8_t cols)
{
	ROWS = rows;
	COLS = cols;
	Data = new T*[ROWS];
	for(uint8_t count = 0; count < ROWS; count++)
		Data[count] = new T[COLS];
}
template void Matrix<double>::Init(uint8_t rows, uint8_t cols);


/**
* @brief Show matrix in terminal
*/
template <class T>
void Matrix<T>::Show()
{
	for(uint8_t row = 0; row < ROWS; row++)
	{
		for(uint8_t col = 0; col < COLS; col++)
		{
			std::cout << Data[row][col] << " ";
		}
		std::cout << std::endl;
	}
}
template void Matrix<double>::Show();


/**
* @brief Multiply matrix's
* @param right - right matrix
* @return composition of 2 matrix
*/
template <class T>
Matrix<T> Matrix<T>::operator * (Matrix<T> right)
{
	// Check dimension:
	if(COLS != right.ROWS)
		Status = DIMENSION_ERROR;

	// Main algorithm:
	Matrix<T> result(this->ROWS, right.COLS);
	for(uint8_t row = 0; row < result.ROWS; row++)
	{
		for(uint8_t col = 0; col < result.COLS; col++)
		{
			result[row][col] = 0;
			for(uint8_t count = 0; count < COLS; count++)
				result[row][col] += (*this)[row][count]*right[count][col];
		}
	}
	return result;
}
template Matrix<double> Matrix<double>::operator * (Matrix<double> right);


/**
* @brief Multiply matrix with scalar
* @param right - scalar
* @return composition
*/
template <class T>
Matrix<T> Matrix<T>::operator * (T right)
{
	Matrix result(ROWS, COLS);
	for(uint8_t row = 0; row < ROWS; row++)
	{
		for(uint8_t col = 0; col < COLS; col++)
		{
			result[row][col] = (*this)[row][col] * right;
		}
	}
	return result;
}
template Matrix<double> Matrix<double>::operator * (double right);


/**
* @brief Addition of matrix's
* @param right - right matrix
* @return Sum of 2 matrix
*/
template <class T>
Matrix<T> Matrix<T>::operator + (Matrix<T> right)
{
	// Check dimension:
	if( (this->ROWS != right.ROWS) || (this->COLS != right.COLS) )
		Status = DIMENSION_ERROR;

	Matrix<T> result(ROWS, COLS);
	if (Status == OK)
	{
		// Add matrix:
		for(uint8_t row = 0; row < ROWS; row++)
		{
			for(uint8_t col = 0; col < COLS; col++)
				result[row][col] = (*this)[row][col] + right[row][col];
		}

	}
	return result;
}
template Matrix<double> Matrix<double>::operator + (Matrix<double> right);


/**
* @brief Transpose the matrix
* @return transpose matrix
*/
template <class T>
Matrix<T> Matrix<T>::transpose()
{
	Matrix result(this->COLS, this->ROWS);
	for(uint8_t row = 0; row < ROWS; row++)
	{
		for(uint8_t col = 0; col < COLS; col++)
			result[col][row] = (*this)[row][col];
	}
	return result;
}
template Matrix<double> Matrix<double>::transpose();


/**
* @brief Calculation of the matrix determinant
* @return matrix determinant
*/
template <class T>
T Matrix<T>::determ()
{
	// Check dimension - matrix should be square
	if(this->COLS != this->ROWS)
		return 0;

	// Main algorithm
	uint8_t size = COLS;
	uint8_t i,j;
	T det = 0;

	if( (this->COLS == 1) && (this->ROWS == 1))
		det = (*this)[0][0];

	else if( (this->COLS == 2) && (this->ROWS == 2))
	{
		det = (*this)[0][0]*(*this)[1][1] - (*this)[0][1]*(*this)[1][0];
	}
	else
	{
		Matrix<T> temp(size -1, size -1);
		for(i = 0; i < size; i++)
		{
			for(j = 0; j < size-1; j++)
			{
				if(j < i)
					temp.Data[j] = this->Data[j];
				else
					temp.Data[j] = this->Data[j+1];
			}
			det+= pow((T)-1, (i+j))*temp.determ()*(*this)[i][size-1];
		}
	}
	return det;
}
template double Matrix<double>::determ();


/**
* @brief Delete last row of matrix
*/
template <class T>
void Matrix<T>::deleteLastRow()
{
	ROWS--;
}
template void Matrix<double>::deleteLastRow();


/**
* @brief Transformation the matrix into the identity matrix
*/
template <class T>
void Matrix<T>::identity()
{
	error_t error = OK;
	uint8_t* bufferRow;
	bufferRow = new uint8_t[COLS];

	// Straight process
	for (uint8_t countConv = 0; countConv < ROWS; countConv++)
	{
		// Permutation of rows of the matrix, if necessary
		if((*this)[countConv][countConv] == 0)
		{
			uint8_t countSort;
			memcpy(bufferRow, (this->Data + countConv*COLS), COLS - countConv);
			for(countSort = countConv + 1; countSort < ROWS; countSort++)
			{
				if((*this)[countSort][countConv] != 0)
				{
					memcpy((this->Data + countConv*COLS), (this->Data  + countSort*COLS + countConv), COLS - countConv);
					memcpy((this->Data + countSort*COLS + countConv), bufferRow, COLS - countConv);
					break;
				}
			}
			if(countSort == ROWS - 1)
				error = IDENTITY_ERROR;
		}
		// Divide the entire string into her first element:
		if(error == OK)
		{
			if((*this)[countConv][countConv] != 1)
			{
				T firstElement = (*this)[countConv][countConv];
				for(uint8_t countDiv = countConv; countDiv < COLS; countDiv++)
					(*this)[countConv][countDiv] /= firstElement;
			}
		}

		// Reset elements to countConv numbers for the bottom rows:
		if(error == OK)
		{
			for(uint8_t rowZero = countConv + 1; rowZero < ROWS; rowZero++)
			{
				if((*this)[rowZero][countConv] != 0)
				{
					T coefficient = (*this)[rowZero][countConv];
					for(uint8_t colZero = countConv; colZero < COLS; colZero++)
						(*this)[rowZero][colZero] -= coefficient*(*this)[countConv][colZero];
				}
			}
		}
	}
	// Reverse process
	if(error == OK)
	{
		for (int8_t rowZero = ROWS - 2; rowZero >= 0; rowZero--)
		{
			for (uint8_t colZero = rowZero + 1; colZero < ROWS; colZero++)
			{
				if ((*this)[rowZero][colZero] != 0)
				{
					T coefficient = (*this)[rowZero][colZero];
					for (uint8_t colCount = colZero; colCount < COLS; colCount++)
						(*this)[rowZero][colCount] -= coefficient*(*this)[colZero][colCount];
				}
			}
		}
	}
	delete[] bufferRow;
	Status = OK;
}
template void Matrix<double>::identity();


/**
* @brief Get inverse matrix
*/
template <class T>
Matrix<T> Matrix<T>::inverse()
{
	// Check dimension - matrix must be square:
	if(ROWS != COLS)
		Status = DIMENSION_ERROR;

	// Check determinant
	if(determ() == 0)
		Status = DETERMINANT_ERROR;

	// Init buffer
	uint8_t bufferRows = ROWS;
	uint8_t bufferCols = COLS << 1;
	Matrix result(ROWS, COLS);
	Matrix buffer(bufferRows, bufferCols);

	// Main algorithm
	for(uint8_t row = 0; row < bufferRows; row++)
	{
		for(uint8_t col = 0; col < COLS; col++)
		{
			buffer[row][col] = (*this)[row][col];
		}

		for(uint8_t col = COLS; col < bufferCols; col++)
		{
			if( (col - COLS) == row)
				buffer[row][col] = 1;
			else
				buffer[row][col] = 0;
		}
	}
	buffer.identity();

	// Return
	for(uint8_t row = 0; row < ROWS; row++)
		memcpy(result.Data[row], buffer.Data[row] + COLS, COLS*sizeof(T));
	return result;
}
template Matrix<double> Matrix<double>::inverse();

/*********************Private area*********************/
/**
* @brief Get a link to an element of matrix
* @param row - index of row
* @param col - index of column
* @return link to an element
*/
template <class T>
T& Matrix<T>::at(uint8_t row, uint8_t col)
{
	return Data[row][col];
}
template double& Matrix<double>::at(uint8_t row, uint8_t col);
