/**
* @file matrix.hpp
* @brief Declaration of class Matrix
*/

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <types.hpp>

/**
* @brief Declaration of class Matrix
*/
class Matrix
{
	private:
		double&  at(int row, int col);				/// Get a link to an element
		/**
		* @brief Supporning structure for the index operator []
		*/
		struct Proxy
		{
			private:
				Matrix& Matr;
				int Row;
			public:
				Proxy (Matrix& matr, uint8_t index): Matr(matr), Row(index) {}
				double & operator [] (int j)
				{
					return Matr.at(Row, j);
				}
				Proxy& operator = (const Proxy& right)
				{
					if (this == &right)
					{
						return *this;
					}
					Matr.Data[Row] = right.Matr.Data[right.Row];
					return *this;
				}
		};

		uint8_t ROWS;								///< Number of rows
		uint8_t COLS;								///< Number of columns
	public:
		Matrix();									/// Constructor
		Matrix(uint8_t rows, uint8_t cols);			/// Constructor with arguments
		void Init(uint8_t rows, uint8_t cols);		/// Initialization
		Proxy operator[] (uint8_t i)				/// Overload the indexing operator
		{
			return Proxy(*this, i);
		}
		double determ();							/// Calculation of the matrix determinant
		void identity();							/// Transformation the matrix into the identity matrix
		Matrix inverse();							/// Inverse matrix
		Matrix operator * (Matrix right);			/// Multiply matrix's
		Matrix operator * (double right);			/// Multiply matrix with scalar
		Matrix operator + (Matrix right);			/// Addition of matrix's
		Matrix transpose();							/// Transpose matrix
		void deleteLastRow();						/// Delete last row
		void Show();								/// Show matrix in terminal

		double** Data;								///< Data array
		error_t Status;								///< Status of the last method performed
};

#endif // MATRIX_HPP

