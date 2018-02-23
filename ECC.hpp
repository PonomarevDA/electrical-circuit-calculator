#ifndef ECC_HPP
#define ECC_HPP

#include <types.hpp>
#include <matrix.hpp>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>

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

	dataStr* dataArr;			// входные данные
	uint8_t numberOfNodes;		// входные данные
	uint8_t numberOfElements;	// входные данные

	vector<uint8_t> branches;	// вектор элементов - ветвей дерева
	vector<uint8_t> chords;		// вектор элементов - хорд
	matrix adjacencyMatrix;		// матрица смежности
	matrix structuralMatrix;	// структурная матрица

	matrix Rtree;				// диагональная матрица сопротивлений (Ом)
	matrix Gchord;				// диагональная матрица проводимостей (См)
	matrix U0;					// вектор напряжений (В)
	matrix I0;					// вектор токов (А)
	matrix F;					// фундаментальная матрица (F)
	matrix Itree;				// Вектор токов дерева (Iд)
	matrix Uchord;				// Вектор напряжений хорд (Uх)

public:
	//ECC();
	void input_data();			//
	void sort_data();

	void create_adjacency_matrix();
	void find_spanning_tree();
	void create_equations_voltage_of_branches();
	void create_equations_current_of_chords();

	void create_oriented_graph();
	void elimination_of_matrix_dependency();
	void allocate_fundamental_matrix();

	void calculate();

	void show_adjacency_matrix();
	void show_structural_matrix();
	void show_fundamental_matrix();




};


#endif // FUNC

