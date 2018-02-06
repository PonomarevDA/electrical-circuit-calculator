#include <iostream>
#include <func.hpp>

using namespace std;

int main()
{
	ECC circuit;

	cout << "1. Ввод данных об электрической схеме в правильном формате:\n";
	circuit.data_input();

	cout << "\n2. Структурная матрица, составленная по ЗТК:\n";
	circuit.create_structural_matrix_with_use_current();
	circuit.structural_matrix_output();

	cout << "\n3. Устранение матричной зависимости:\n";
	circuit.elimination_of_matrix_dependency();

	cout << "\n4. Преобразование матрицы в единичную:\n";
	circuit.conversion_to_the_identity_matrix();

	//cout << "\n5. Преобразование матрицы в единичную:\n";
	//circuit.allocate_fundamental_matrix();

	//

}
