#include <iostream>
#include <ECC.hpp>

using namespace std;

int main()
{
	ECC circuit;

	cout << "1. Ввод данных об электрической схеме в правильном формате:\n";
	circuit.input_data();

	cout << "\n2. Создание матрицы смежности:\n";
	circuit.create_adjacency_matrix();

	cout << "\n3. Создание ориентированного графа:\n";
	circuit.create_oriented_graph();
	circuit.structural_matrix_output();

	cout << "\n4. Поиск остовного дерева графа:\n";
	circuit.find_spanning_tree();

	cout << "\n5. Установка жесткой нумерации ветвей графа:\n";

	cout << "\n6. Устранение матричной зависимости:\n";
	circuit.elimination_of_matrix_dependency();
	circuit.structural_matrix_output();

	cout << "\n7. Преобразование матрицы в единичную:\n";
	circuit.conversion_to_the_identity_matrix();
	circuit.structural_matrix_output();

	cout << "\n8. Выделение фундаментальной матрицы:\n";
	circuit.allocate_fundamental_matrix();
	circuit.fundamental_matrix_output();


}
