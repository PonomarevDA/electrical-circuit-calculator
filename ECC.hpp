#ifndef ECC_HPP
#define ECC_HPP

#include <types.hpp>

// electrical circuit calculator
class ECC
{
public:
	//ECC();
	void input_data();
	void sort_data();
	void create_adjacency_matrix();
	void find_spanning_tree();
	void create_oriented_graph();
//	void search_graph_tree();
//	void order_matrix();
//	void create_equations_voltage();	// для ветвей дерева
//	void create_equations_current();	// для хорд


	void elimination_of_matrix_dependency();
	void conversion_to_the_identity_matrix();
	void allocate_fundamental_matrix();

//	void calculate();

	void start();

	void structural_matrix_output();
	void fundamental_matrix_output();

private:
	enum{ NUMBER_OF_NODES = 4, NUMBER_OF_ELEMENTS = 5};
	struct dataStr						// строка входных данных
	{
		void init(uint8_t i, uint8_t f, uint8_t l, int8_t t, double v)
		{
			index = i;
			nodeFirst = f;
			nodeLast = l;
			type = t;
			value = v;
		}
		uint8_t index;
		uint8_t nodeFirst;
		uint8_t nodeLast;
		int8_t type;
		double value;
	};
	dataStr* dataArr;
	long double** RMatrix;				// диагональная матрица сопротивлений (Ом)
	long double** UMatrix;				// диагональная матрица напряжений (В)
	long double** IMatrix;				// диагональная матрица токов (А)

	__uint8_t numberOfNodes;
	__uint8_t numberOfElements;
	__int8_t** structuralMatrix;		// структурная матрица
	__int8_t** fundamentalMatrix;		// фундаментальная матрица (F)
	bool** adjacencyMatrix;				// матрицаа смежности

};


#endif // FUNC

