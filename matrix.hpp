#ifndef MATRIX
#define MATRIX

#include <types.hpp>
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>
#include <iomanip>

class matrix
{
private:
	uint8_t ROWS;					// количество строк
	uint8_t COLS;					// количество столбцов
	double&  at(int row, int col);	// Получить ссылку на элемент

	// Вспомогательная структура для оператора индексирования
	struct proxy
	{
	private:
		matrix& m_;
		int i_;
	public:
		proxy (matrix& m, uint8_t i): m_(m), i_(i) {}
		double & operator [] (int j)
		{
			return m_.at(i_, j);
		}
		proxy& operator = (const proxy& right)
		{
			if (this == &right)
			{
				return *this;
			}
			m_.data[i_] = right.m_.data[right.i_];
			return *this;
		}
	};
public:
	double** data;					// Массив данных
	error_t status;			// Статус последнего выполненного метода

	matrix();						// Конструктор без аргументов
	matrix(uint8_t r, uint8_t c);	// Конструктор
	void init(uint8_t r, uint8_t c);// Инициализация
	proxy operator[] (uint8_t i)	// Перегрузка оператора индексации
	{
		return proxy(*this, i);
	}


	double determ();					// Расчет определителя матрицы
	void identity();					// Преобразование матрицы в единичную
	matrix inverse();					// Обратная матрица
	matrix operator * (matrix right);	// Умножение матриц
	matrix operator * (double right);	// Умножение матриц
	matrix operator + (matrix right);	// Сложение матриц
	matrix transpose();					// Транспонирование матрицы
	void deleteLastRow();				// Удалить последюю строку
	void show();						// Вывод матрицы в терминал


};

#endif // MATRIX

