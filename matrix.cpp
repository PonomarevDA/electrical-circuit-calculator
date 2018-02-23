#include <matrix.hpp>
#include <iostream>
#include <iomanip>

using namespace std;

/*********************Private area*********************/
// Получить ссылку на элемент
double& matrix::at(int row, int col)
{
	return data[row][col];
}

/*********************Public area*********************/
// Конструктор без аргументов
matrix::matrix() {}

// Конструктор
matrix::matrix(uint8_t r, uint8_t c) : ROWS(r), COLS(c)
{
	data = new double*[ROWS];
	for(uint8_t count = 0; count < ROWS; count++)
		data[count] = new double[COLS];
}

// Инициализация
void matrix::init(uint8_t r, uint8_t c)
{
	ROWS = r;
	COLS = c;
	data = new double*[ROWS];
	for(uint8_t count = 0; count < ROWS; count++)
		data[count] = new double[COLS];
}

// Вывод матрицы в терминал
void matrix::show()
{
	for(uint8_t row = 0; row < ROWS; row++)
	{
		for(uint8_t col = 0; col < COLS; col++)
		{
			cout << data[row][col] << " ";
		}
		cout << endl;
	}
}

// Умножение матриц
matrix matrix::operator * (matrix right)
{
	// Check dimension:
	if(this->COLS != right.ROWS)
		status = DIMENSION_ERROR;

	// Multiply matrix:
	matrix result(this->ROWS, right.COLS);
	for(uint8_t row = 0; row < result.ROWS; row++)
	{
		for(uint8_t col = 0; col < result.COLS; col++)
		{
			result[row][col] = 0;
			for(uint8_t count = 0; count < this->COLS; count++)
				result[row][col] += (*this)[row][count]*right[count][col];
		}
	}
	return result;
}

// Умножение матриц
matrix matrix::operator * (double right)
{
	// Multiply matrix:
	matrix result(ROWS, COLS);
	for(uint8_t row = 0; row < ROWS; row++)
	{
		for(uint8_t col = 0; col < COLS; col++)
		{
			result[row][col] = (*this)[row][col] * right;
		}
	}
	return result;
}

// Сложение матриц
matrix matrix::operator + (matrix right)
{
	// Check dimension:
	if( (this->ROWS != right.ROWS) || (this->COLS != right.COLS) )
		status = DIMENSION_ERROR;
	matrix result(ROWS, COLS);
	if (status == OK)
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

// Транспонирование матрицы
matrix matrix::transpose()
{
	// Transpose matrix:
	matrix result(this->COLS, this->ROWS);
	for(uint8_t row = 0; row < ROWS; row++)
	{
		for(uint8_t col = 0; col < COLS; col++)
			result[col][row] = (*this)[row][col];
	}
	return result;
}

// Вернуть значение определителя квадратной матрицы
double matrix::determ()
{
	// Check dimension - matrix should be square
	if(this->COLS != this->ROWS)
		return 0;

	// Main algorithm
	uint8_t size = COLS;
	uint8_t i,j;
	double det = 0;

	if( (this->COLS == 1) && (this->ROWS == 1))
		det = (*this)[0][0];

	else if( (this->COLS == 2) && (this->ROWS == 2))
	{
		det = (*this)[0][0]*(*this)[1][1] - (*this)[0][1]*(*this)[1][0];
	}
	else
	{
		matrix temp(size -1, size -1);
		for(i = 0; i < size; i++)
		{
			for(j = 0; j < size-1; j++)
			{
				if(j < i)
					temp.data[j] = this->data[j];
				else
					temp.data[j] = this->data[j+1];
			}
			det+= pow((double)-1, (i+j))*temp.determ()*(*this)[i][size-1];
		}
	}
	return det;
}

void matrix::deleteLastRow()
{
	ROWS--;
}

// Преобразование матрицы в единичную
void matrix::identity()
{
	error_t error = OK;
	uint8_t* bufferRow;
	bufferRow = new uint8_t[COLS];

	// Прямой ход
	for (uint8_t countConv = 0; countConv < ROWS; countConv++)
	{
		// Перестановка строк матрицы, если необходимо:
		if((*this)[countConv][countConv] == 0)
		{
			uint8_t countSort;
			memcpy(bufferRow, (this->data + countConv*COLS), COLS - countConv);
			for(countSort = countConv + 1; countSort < ROWS; countSort++)
			{
				if((*this)[countSort][countConv] != 0)
				{
					memcpy((this->data + countConv*COLS), (this->data  + countSort*COLS + countConv), COLS - countConv);
					memcpy((this->data + countSort*COLS + countConv), bufferRow, COLS - countConv);
					break;
				}
			}
			if(countSort == ROWS - 1)
				error = IDENTITY_ERROR;
		}
		// Делим всю строку на первый её элемент:
		if(error == OK)
		{
			if((*this)[countConv][countConv] != 1)
			{
				double firstElement = (*this)[countConv][countConv];
				for(uint8_t countDiv = countConv; countDiv < COLS; countDiv++)
					(*this)[countConv][countDiv] /= firstElement;
			}
		}

		// Обнуляем элменты под номерами countConv для нижних строк:
		if(error == OK)
		{
			for(uint8_t rowZero = countConv + 1; rowZero < ROWS; rowZero++)
			{
				if((*this)[rowZero][countConv] != 0)
				{
					double coefficient = (*this)[rowZero][countConv];
					for(uint8_t colZero = countConv; colZero < COLS; colZero++)
						(*this)[rowZero][colZero] -= coefficient*(*this)[countConv][colZero];
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
				if ((*this)[rowZero][colZero] != 0)
				{
					double coefficient = (*this)[rowZero][colZero];
					for (uint8_t colCount = colZero; colCount < COLS; colCount++)
						(*this)[rowZero][colCount] -= coefficient*(*this)[colZero][colCount];
				}
			}
		}
	}
	delete[] bufferRow;
	status = OK;
}

// Обратная матрица
matrix matrix::inverse()
{
	// Check dimension - matrix must be square:
	if(ROWS != COLS)
		status = DIMENSION_ERROR;

	// Check determinant
	if(determ() == 0)
		status = DETERMINANT_ERROR;

	// Init buffer
	uint8_t bufferRows = ROWS;
	uint8_t bufferCols = COLS << 1;
	matrix result(ROWS, COLS);
	matrix buffer(bufferRows, bufferCols);

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
		memcpy(result.data[row], buffer.data[row] + COLS, COLS*sizeof(double));
	return result;
}
