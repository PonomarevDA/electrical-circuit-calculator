#include <ECC.hpp>

using namespace std;


// 1. Ввод данных
void ECC::input_data()
{
	// Кусок кода ниже заменить на ввод данных из файла:
	numberOfElements = 5;
	dataArr = new dataStr[numberOfElements];
	dataArr[0].init(1, 1, 4, 'u', 5);
	dataArr[1].init(2, 1, 2, 'r', 1000);
	dataArr[2].init(3, 3, 4, 'r', 1000);
	dataArr[3].init(4, 2, 3, 'r', 1000);
	dataArr[4].init(5, 4, 2, 'i', 0.005);
	// Кусок кода выше заменить на ввод данных из файла.

	// Подсчет количества узлов:
	numberOfNodes = 0;
	for(__int8_t count = 0; count < numberOfElements; count++)
		numberOfNodes = (numberOfNodes > dataArr[count].nodeFirst) ? numberOfNodes : dataArr[count].nodeFirst;
}

// 2. Сортировка входных данных
void ECC::sort_data()
{
	// Сначала ИН, затем R, в конце ИТ
	cout << "Don\'t work yet :(" << endl;
}

// 3. Создание матрицы смежности
void ECC::create_adjacency_matrix()
{
	// Выделение памяти для матрицы смежности (считаем граф неориентированным):
	adjacencyMatrix = new bool* [numberOfNodes];
	for (__uint8_t count = 0; count < numberOfNodes; count++)
		adjacencyMatrix[count] = new bool [numberOfElements];

	// Заполняем матрицу смежности: 1 - есть ребро, 0 - нет ребра
	for(__int8_t count = 0; count < numberOfElements; count++)
	{
		adjacencyMatrix[ (dataArr[count].nodeFirst) - 1][ (dataArr[count].nodeLast) - 1] = 1;
		adjacencyMatrix[ (dataArr[count].nodeLast) - 1][ (dataArr[count].nodeFirst) - 1] = 1;
	}
}

// 4. Поиск остовного дерева
void ECC::find_spanning_tree()
{
	// Init
	deque<uint8_t> nodesFuture;
	deque<uint8_t> nodesPresent;
	set<uint8_t> branches;
	set<uint8_t> chords;

	// Заполняем контейнеры nodesFuture и chords:
	for(uint8_t node = 1; node <= numberOfNodes; node++)
	{
		chords.insert(chords.end(), node);
		nodesFuture.push_back(node);
	}
	for(uint8_t element = numberOfNodes + 1; element <= numberOfElements; element++)
		chords.insert(chords.end(), element);

	// Пусть первый элемен (№0) будет первой ветвью дерева
	nodesFuture.erase(nodesFuture.begin() + dataArr[0].nodeFirst - 1);
	nodesFuture.erase(nodesFuture.begin() + dataArr[0].nodeLast - 1);
	nodesPresent.push_back(dataArr[0].nodeFirst);
	nodesPresent.push_back(dataArr[0].nodeLast);
	chords.erase(chords.begin());
	branches.insert(1);

	// Основной цикл поиска - по элементам списка nodesPresent
	while(!nodesPresent.empty())
	{
		// Цикл по узлам из списка nodesFuture:
		for(uint8_t node = 0; node < nodesFuture.size(); node++)
		{
			// Первый узел списка nodesPresent образует ветвь дерева с узлом индекса node?
			if( adjacencyMatrix[nodesFuture.at(node)][nodesPresent.at(0)] )
			{
				// Определение номера элемента
				uint8_t number;
				for(number = 0; number < numberOfElements; number++)
				{
					if( ( (dataArr[number].nodeFirst == nodesFuture.at(node)) &&
						  (dataArr[number].nodeLast == nodesPresent.at(0))) ||
						( (dataArr[number].nodeLast == nodesFuture.at(node)) &&
						  (dataArr[number].nodeFirst == nodesPresent.at(0))))
					{
						break;
					}
				}
				number++;
				// Проверка, что данный элемнент уже не является ветвью дерева
				if(branches.count(number))
					break;
				// Проверка, что данный элемент не является ИТ
				if(dataArr[number - 1].type == 'i')
					break;

				// В таком случае, рассматриваемый элемент - ветвь дерева
				chords.erase(number);
				branches.insert(branches.end(), number);
				nodesPresent.push_back(nodesFuture.at(node));
				nodesFuture.erase(nodesFuture.begin() + node);
				node--; // !!!размер nodesFuture уменьшился на 1
				break;
			}
		}
		// Если с узлом nodesPresent.at(0) уже нельзя образовать ветви
		nodesPresent.erase(nodesPresent.begin());
	}

	// Вывод на экран содержимого списков и множеств
	cout << "nodesFuture: ";
	for(uint8_t node = 0; node < nodesFuture.size(); node++)
		cout << nodesFuture.at(node) + 0 << " ";

	cout << "\nnodesPresent: ";
	for(uint8_t node = 0; node < nodesPresent.size(); node++)
		cout << nodesPresent.at(node) + 0 << " ";

	cout << "\nchords: ";
	for(uint8_t element = 0; element < chords.size(); element++)
		cout << *chords.begin()+element + 0 << " ";

	cout << "\nbranches: ";
	for(uint8_t element = 0; element < branches.size(); element++)
		cout << *branches.begin()+element + 0 << " ";
	cout << endl;


	tree = new uint8_t [branches.size() + 1];
	tree[0] = branches.size();
	for (uint8_t element = 1; element <= branches.size(); element++)
		tree[element] = *branches.begin() + element - 1;
}

// 5. Составление уравнений напряжения ветвей дерева:
void ECC::create_equations_voltage_of_branches()
{
	// Выделение памяти для матрицы, инициализация и вывод на экран:
	Rtree = new long double* [tree[0]];
	for(uint8_t element = 0; element < tree[0]; element++)
	{
		Rtree[element] = new long double[tree[0]];
		for(uint8_t col = 0; col < tree[0]; col++)
			Rtree[element][col] = 0;
		if(dataArr[ tree[element + 1] - 1].type == 'r')
			Rtree[element][element] = dataArr[ tree[element + 1] - 1].value;
		else
			Rtree[element][element] = 0;
	}

	for(uint8_t row = 0; row < tree[0]; row++)
	{
		for(uint8_t col = 0; col < tree[0]; col++)
		{
			cout << Rtree[row][col] + 0 << " ";
		}
		cout << endl;
	}
	cout << endl;

	// Выделение памяти для вектора, инициализация и вывод на экран:
	I0 = new long double[tree[0]];
	for(uint8_t element = 0; element < tree[0]; element++)
	{
		if(dataArr[ tree[element + 1] - 1 ].type == 'u')
			I0[element] = dataArr[ tree[element + 1]  - 1].value;
		else
			I0[element] = 0;
	}

	for(uint8_t row = 0; row < tree[0]; row++)
	{
		cout << I0[row] + 0 << " ";
	}
	cout << endl;

	// Вывод на экран номера ветвей дерева
	for(uint8_t element = 1; element <= tree[0]; element++)
		cout << tree[element] + 0 << " ";
	cout << endl;

}

// 6. Составление уравнений токов хорд:


// 7. Создание структурной матрицы по ЗТК из массива входных данных
void ECC::create_oriented_graph()
{
	// Выделение памяти для структурной матрицы
	structuralMatrix = new __int8_t* [numberOfNodes];
	for (__uint8_t row = 0; row < numberOfNodes; row++)
	{
		structuralMatrix[row] = new __int8_t [numberOfElements];
		for(__uint8_t col = 0; col < numberOfElements; col++)
			structuralMatrix[row][col] = 0;
	}

	for(__int8_t elem = 0; elem < numberOfElements; elem++)
	{
		structuralMatrix[dataArr[elem].nodeFirst - 1][elem] = 1;
		structuralMatrix[dataArr[elem].nodeLast - 1][elem] = -1;
	}
}


// 8. Исключение базисного узла
void ECC::elimination_of_matrix_dependency()
{
	bool isMatrixDependent = 0;
	for (__int8_t countOfNode = 0; countOfNode < numberOfNodes; countOfNode++)
	{
		__int8_t sum_of_row = 0;
		for (__int8_t countOfElement = 0; countOfElement < numberOfElements; countOfElement++)
		{
			sum_of_row=+structuralMatrix[countOfNode][countOfElement];
			if (isMatrixDependent = sum_of_row != 0)
				break;
		}
		if (isMatrixDependent)
			break;
	}

	if (isMatrixDependent)
	{
		cout << "Матрица линейно зависима, сделаем ее независимой\n";
		numberOfNodes--;
	}
}


// 9. Преобразование матрицы в единичную
void ECC::conversion_to_the_identity_matrix()
{
	enum {ERROR, OK};
	bool status = OK;
	__uint8_t* bufferRow;
	bufferRow = new __uint8_t[numberOfElements];

	// Прямой ход
	for (__uint8_t countConv = 0; countConv < numberOfNodes; countConv++)
	{
		// Перестановка строк матрицы, если необходимо:
		if(structuralMatrix[countConv][countConv] == 0)
		{
			memcpy(bufferRow, (*structuralMatrix + countConv*numberOfElements), numberOfElements - countConv);
			for(__uint8_t countSort = countConv + 1; countSort < numberOfNodes; countSort++)
			{
				if(structuralMatrix[countSort][countConv] != 0)
				{
					memcpy((*structuralMatrix + countConv*numberOfElements), (*structuralMatrix + countSort*numberOfElements + countConv), numberOfElements - countConv);
					memcpy((*structuralMatrix + countSort*numberOfElements + countConv), bufferRow, numberOfElements - countConv);
					continue;
				}
				if(countSort == numberOfNodes - 1)
				{
					status = ERROR;
				}
			}
		}
		// Инверсия строки, если необходимо:
		if(status == OK)
		{
			if(structuralMatrix[countConv][countConv] == -1)
			{
				for(__uint8_t countInverse = countConv; countInverse < numberOfElements; countInverse++)
				{
					structuralMatrix[countConv][countInverse] *= -1;
				}
			}
		}

		// Обнуляем элменты под номерами countConv для нижних строк:
		if(status == OK)
		{
			for(__uint8_t rowZero = countConv + 1; rowZero < numberOfNodes; rowZero++)
			{
				if(structuralMatrix[rowZero][countConv] == 1)
				{
					for(__uint8_t colZero = countConv; colZero < numberOfElements; colZero++)
					{
						structuralMatrix[rowZero][colZero] -= structuralMatrix[countConv][colZero];
					}
				}
				else if(structuralMatrix[rowZero][countConv] == -1)
				{
					for(__uint8_t colZero = countConv; colZero < numberOfElements; colZero++)
					{
						structuralMatrix[rowZero][colZero] += structuralMatrix[countConv][colZero];
					}
				}
			}
		}
	}
	// Обратный ход
	if(status == OK)
	{
		for (__int8_t rowZero = numberOfNodes - 2; rowZero >= 0; rowZero--)
		{
			for (__uint8_t colZero = rowZero + 1; colZero < numberOfNodes; colZero++)
			{
				if (structuralMatrix[rowZero][colZero] == 1)
				{
					for (__uint8_t colCount = colZero; colCount < numberOfElements; colCount++)
					{
						structuralMatrix[rowZero][colCount] -= structuralMatrix[rowZero + (colZero - rowZero)][colCount];
					}
				}
				else if(structuralMatrix[rowZero][colZero] == -1)
				{
					for (__uint8_t colCount = colZero; colCount < numberOfElements; colCount++)
					{
						structuralMatrix[rowZero][colCount] += structuralMatrix[rowZero + (colZero - rowZero)][colCount];
					}
				}
			}
		}
	}

	delete[] bufferRow;
}

// 10. Выделение фундаментальной матрицы
void ECC::allocate_fundamental_matrix()
{
	fundamentalMatrix = new __int8_t* [numberOfNodes];
	for (__uint8_t row = 0; row < numberOfNodes; row++)
	{
		fundamentalMatrix[row] = new __int8_t [numberOfElements - numberOfNodes];
		memcpy(fundamentalMatrix[row], structuralMatrix[row] + numberOfNodes, numberOfElements - numberOfNodes);
	}

}

// Вывод матрицы смежности на экран:
void ECC::show_adjacency_matrix()
{
	for(__uint8_t i = 0; i < numberOfNodes; i++)
	{
		for(__uint8_t j = 0; j < numberOfNodes; j++)
			cout << adjacencyMatrix[i][j] + 0 << " ";
		cout << endl;
	}
}


// Форматированный вывод структурной матрицы
void ECC::show_structural_matrix()
{
	for(__uint8_t i = 0; i < numberOfNodes; i++)
	{
		for(__uint8_t j = 0; j < numberOfElements; j++)
		{
			if (!((structuralMatrix[i][j] >> 7) & 1))
				cout << " ";
			cout << structuralMatrix[i][j] + 0 << " ";

		}
		cout << endl;
	}
}

// Форматированный вывод фундаментальной матрицы
void ECC::show_fundamental_matrix()
{
	for(__uint8_t i = 0; i < numberOfNodes; i++)
	{
		for(__uint8_t j = 0; j < numberOfElements - numberOfNodes; j++)
		{
			if (!((fundamentalMatrix[i][j] >> 7) & 1))
				cout << " ";
			cout << fundamentalMatrix[i][j] + 0 << " ";

		}
		cout << endl;
	}
}
