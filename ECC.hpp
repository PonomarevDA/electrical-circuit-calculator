#ifndef ECC_HPP
#define ECC_HPP

#include <types.hpp>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>

using namespace std;

// electrical circuit calculator
class ECC
{
public:
	//ECC();
	void input_data();
	void sort_data();

	void create_adjacency_matrix();
	void find_spanning_tree();
	void create_equations_voltage_of_branches();
	void create_equations_current_of_chords();

	void create_oriented_graph();
	void elimination_of_matrix_dependency();
	void conversion_to_the_identity_matrix();
	void allocate_fundamental_matrix();

	void calculate();

	void show_adjacency_matrix();
	void show_structural_matrix();
	void show_fundamental_matrix();



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

	dataStr* dataArr;
	__uint8_t numberOfNodes;
	__uint8_t numberOfElements;

	bool** adjacencyMatrix;				// матрица смежности

	long double** Rtree;				// диагональная матрица сопротивлений (Ом)
	long double** Gchord;				// диагональная матрица проводимостей (См)
	long double* U0;					// вектор напряжений (В)
	long double* I0;					// вектор токов (А)

	__int8_t** structuralMatrix;		// структурная матрица
	__int8_t** fundamentalMatrix;		// фундаментальная матрица (F)

	vector<uint8_t> branches;
	vector<uint8_t> chords;
};


#endif // FUNC

