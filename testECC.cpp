#include <testECC.hpp>


using namespace std;

ECC circuit;

void CalculatedExample()
{
	cout << "1. Ввод данных об электрической схеме в правильном формате:\n";
	circuit.input_data();

	cout << "\n2. Сортировка введенных данных:\n";

	cout << "\n3. Создание матрицы смежности:\n";
	circuit.create_adjacency_matrix();
	circuit.show_adjacency_matrix();

	cout << "\n4. Поиск остовного дерева графа:\n";
	circuit.find_spanning_tree();

	cout << "\n5. Составление уравнений напряжения ветвей дерева:\n";
	circuit.create_equations_voltage_of_branches();

	cout << "\n6. Составление уравнений токов хорд:\n";
	circuit.create_equations_current_of_chords();

	cout << "\n7. Создание ориентированного графа:\n";
	circuit.create_oriented_graph();
	circuit.show_structural_matrix();

	cout << "\n8. Устранение матричной зависимости:\n";
	circuit.elimination_of_matrix_dependency();
	circuit.show_structural_matrix();

	cout << "\n9. Преобразование матрицы в единичную:\n";
	circuit.conversion_to_the_identity_matrix();
	circuit.show_structural_matrix();

	cout << "\n10. Выделение фундаментальной матрицы:\n";
	circuit.allocate_fundamental_matrix();
	circuit.show_fundamental_matrix();
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

	multiply(left, right, result);

	// Output:
	showMatrix(left);
	showMatrix(right);
	showMatrix(result);
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
	showMatrix(primaryMatrix);


	// Tranponse Matrix:
	uint8_t newMatrixRows = matrixCols;
	uint8_t newMatrixCols = matrixRows;
	vector < vector <double> > totalMatrix( newMatrixRows, vector <double> (newMatrixCols) );
	transpose(primaryMatrix, totalMatrix);
	showMatrix(totalMatrix);
}


template <typename T>
void showMatrix(T matrix)
{
	for(uint8_t row = 0; row < matrix.size(); row++)
	{
		for(uint8_t col = 0; col < matrix[0].size(); col++)
			cout << matrix[row][col] << " ";
		cout << endl;
	}
	cout << endl;
}
