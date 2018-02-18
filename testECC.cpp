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
