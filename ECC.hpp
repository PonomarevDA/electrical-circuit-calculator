#ifndef ECC_HPP
#define ECC_HPP

#include <types.hpp>
#include <matrix.hpp>
#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>

using namespace std;

// electrical circuit calculator
class ECC
{
private:
	struct dataStr						// строка входных данных
	{
		void init(uint8_t i, uint8_t f, uint8_t l, int8_t t, double v)
		{ index = i; nodeFirst = f; nodeLast = l; type = t; value = v;}
		uint8_t index;
		uint8_t nodeFirst;
		uint8_t nodeLast;
		int8_t type;
		double value;
	};

	dataStr* dataArr;			// массив данных о системе
	uint8_t numberOfNodes;		// данные о системе
	uint8_t numberOfElements;	// данные о системе

	vector<uint8_t> branches;	// вектор элементов - ветвей дерева
	vector<uint8_t> chords;		// вектор элементов - хорд
	Matrix adjacencyMatrix;		// матрица смежности
	Matrix structuralMatrix;	// структурная матрица

	Matrix Rtree;				// диагональная матрица сопротивлений ветвей (Ом)
	Matrix Gchord;				// диагональная матрица проводимостей хорд (См)
	Matrix U0;					// вектор напряжений (В)
	Matrix I0;					// вектор токов (А)
	Matrix F;					// фундаментальная матрица (F)
	Matrix Itree;				// Вектор токов дерева (Iд)
	Matrix Uchord;				// Вектор напряжений хорд (Uх)

	void output_data(dataStr* dataArr, uint8_t lenght);
	error_t make_nodes_a_branch_if_it_possible(uint8_t, uint8_t, task_t);
	uint8_t find_number_of_element(const uint8_t node1, const uint8_t node2);

public:
	//ECC();
	void input_data();							// 1. Ввод данных о схеме
	void sort_data();							// 2. Сортировка данных

	void create_adjacency_matrix();				// 3. Создание матрицы смежности
	void find_spanning_tree();					// 4. Поиск ветвей остовного дерева
	void create_equations_voltage_of_branches();// 5. Создание уравнений напряжения ветвей
	void create_equations_current_of_chords();	// 6. Создание уравнений токов хорд

	void create_oriented_graph();				// 7. Создание ориентированного графа
	void elimination_of_matrix_dependency();	// 8. Удаление базисного узла
	void allocate_fundamental_matrix();			// 9. Выделение фундаментальной матрицы

	void calculate();							// 10. Расчет токов/напряжений
};


#endif // FUNC

