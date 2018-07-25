#include <testECC.hpp>

using namespace std;

ECC circuit;

void CalculatedExample()
{
	cout << "1. Ввод данных об электрической схеме в правильном формате:\n";
	circuit.input_data();

	cout << "\n2. Сортировка введенных данных:\n";
	circuit.sort_data();

	cout << "\n3. Создание матрицы смежности:\n";
	circuit.create_adjacency_matrix();

	cout << "\n4. Поиск остовного дерева графа:\n";
	circuit.find_spanning_tree();

	cout << "\n5. Составление уравнений напряжения ветвей дерева:\n";
	circuit.create_equations_voltage_of_branches();

	cout << "\n6. Составление уравнений токов хорд:\n";
	circuit.create_equations_current_of_chords();

	cout << "\n7. Создание ориентированного графа:\n";
	circuit.create_oriented_graph();

	cout << "\n8. Устранение матричной зависимости:\n";
	circuit.elimination_of_matrix_dependency();

	cout << "\n9. Выделение фундаментальной матрицы:\n";
	circuit.allocate_fundamental_matrix();

	cout << "\n10. Ответ:\n";
	circuit.calculate();
}


void testMultiplyMatrix()
{
	// Init Left Matrix:
	uint8_t leftRows = 3;
	uint8_t leftCols = 4;
	vector < vector <double> > left( leftRows, vector <double> (leftCols) );
	for(uint8_t row = 0; row < leftRows; row++)
	{
		for(uint8_t col = 0; col < leftCols; col++)
			left[row][col] = col + row;
	}

	// Init Right Matrix:
	uint8_t rightRows = 4;
	uint8_t rightCols = 2;
	vector < vector <double> > right( rightRows, vector <double> (rightCols) );
	for(uint8_t row = 0; row < rightRows; row++)
	{
		for(uint8_t col = 0; col < rightCols; col++)
			right[row][col] = col + row;
	}

	// Result Matrix:
	uint8_t resultRows = leftRows;
	uint8_t resultCols = rightCols;
	vector < vector <double> > result( resultRows, vector <double> (resultCols) );
	for(uint8_t row = 0; row < resultRows; row++)
	{
		for(uint8_t col = 0; col < resultCols; col++)
			result[row][col] = 0;
	}

	//multiply(left, right, result);

	// Output:
}


void testTransposeMatrix()
{
	// Initial Matrix:
	uint8_t matrixRows = 3;
	uint8_t matrixCols = 4;
	vector < vector <double> > primaryMatrix( matrixRows, vector <double> (matrixCols) );
	for(uint8_t row = 0; row < matrixRows; row++)
	{
		for(uint8_t col = 0; col < matrixCols; col++)
			primaryMatrix[row][col] = col + row;
	}


	// Tranponse Matrix:
	uint8_t newMatrixRows = matrixCols;
	uint8_t newMatrixCols = matrixRows;
	vector < vector <double> > totalMatrix( newMatrixRows, vector <double> (newMatrixCols) );
	//transpose(primaryMatrix, totalMatrix);
}

void testIdentityMatrix()
{
	// Initial Matrix:
	uint8_t matrixRows = 3;
	uint8_t matrixCols = 4;
	vector < vector <double> > primaryMatrix( matrixRows, vector <double> (matrixCols) );
	for(uint8_t row = 0; row < matrixRows; row++)
	{
		for(uint8_t col = 0; col < matrixCols; col++)
			primaryMatrix[row][col] = col + row;
	}
}

void testInverseMatrix()
{
	// Initial Matrix:
	uint8_t matrixRows = 3;
	uint8_t matrixCols = 3;
	Matrix m(matrixRows, matrixCols);
	m[0][0] = 2; m[0][1] = 2; m[0][2] = 3;
	m[1][0] = 4; m[1][1] = 5; m[1][2] = 6;
	m[2][0] = 7; m[2][1] = 8; m[2][2] = 9;
	m.Show();
	m.inverse();
	m.Show();

}
