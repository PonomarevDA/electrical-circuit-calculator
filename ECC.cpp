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
	vector<uint8_t> nodesFuture;
	vector<uint8_t> nodesPresent;

	// Заполняем контейнеры nodesFuture и chords:
	for(uint8_t node = 1; node <= numberOfNodes; node++)
	{
		chords.push_back(node);
		nodesFuture.push_back(node);
	}
	for(uint8_t element = numberOfNodes + 1; element <= numberOfElements; element++)
		chords.push_back(element);

	// Пусть первый элемен (№0) будет первой ветвью дерева
	chords.erase(chords.begin());
	branches.push_back(1);
	for(uint8_t index = 0; index < nodesFuture.size(); index++)
	{
		if(dataArr[0].nodeFirst == nodesFuture[index] || dataArr[0].nodeLast == nodesFuture[index])
		{
			nodesFuture.erase(nodesFuture.begin() + index);
			index--;
		}
	}
	nodesPresent.push_back(dataArr[0].nodeFirst);
	nodesPresent.push_back(dataArr[0].nodeLast);


	// Основной цикл поиска - по элементам множества nodesPresent
	while(!nodesPresent.empty())
	{
		// Цикл по узлам из множества nodesFuture:
		for(uint8_t index = 0; index < nodesFuture.size(); index++)
		{
			// Образует ли первый узел nodesPresent с ветвю дерева с узлом индекса node?
			if( adjacencyMatrix[nodesFuture.at(index)][nodesPresent.at(0)] )
			{
				// Определение номера элемента
				uint8_t number;
				for(number = 0; number < numberOfElements; number++)
				{
					if( ( (dataArr[number].nodeFirst == nodesFuture.at(index)) &&
						  (dataArr[number].nodeLast == nodesPresent.at(0))) ||
						( (dataArr[number].nodeLast == nodesFuture.at(index)) &&
						  (dataArr[number].nodeFirst == nodesPresent.at(0))))
					{
						break;
					}
				}
				number++;
				// Проверка, что данный элемнент уже не является ветвью дерева
				bool result = 0;
				for(uint8_t count = 0; count < branches.size(); count++)
				{
					if(branches[count] == number)
						result = 1;
				}
				// Проверка, что данный элемент не является ИТ
				if (result == 0)
				{
					if(dataArr[number - 1].type == 'i')
						break;
				}


				// Если все ок, то рассматриваемый элемент - ветвь дерева
				branches.insert(branches.end(), number);
				for(uint8_t count = 0; count < chords.size(); count++)
				{
					if(chords[count] == number)
						chords.erase(chords.begin() + count);
				}
				nodesPresent.push_back(nodesFuture.at(index));
				nodesFuture.erase(nodesFuture.begin() + index);
				index--; // !!!размер nodesFuture уменьшился на 1
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
}

// 5. Составление уравнений напряжения ветвей дерева:
void ECC::create_equations_voltage_of_branches()
{
	// Матрица сопротивлений ветвей дерева:
	Rtree = new long double* [branches.size()];
	for(uint8_t element = 0; element < branches.size(); element++)
	{
		Rtree[element] = new long double[branches.size()];
		for(uint8_t col = 0; col < branches.size(); col++)
			Rtree[element][col] = 0;
		if(dataArr[ *(branches.begin() + element) -1 ].type == 'r')
			Rtree[element][element] = dataArr[ *(branches.begin() + element) -1 ].value;
	}

	for(uint8_t row = 0; row < branches.size(); row++)
	{
		for(uint8_t col = 0; col < branches.size(); col++)
		{
			cout << Rtree[row][col] + 0 << " ";
		}
		cout << endl;
	}
	cout << endl;

	// Вектор напряжений ветвей дерева:
	U0 = new long double[branches.size()];
	for(uint8_t element = 0; element < branches.size(); element++)
	{
		if(dataArr[ *(branches.begin() + element) -1 ].type == 'u')
			U0[element] = dataArr[ *(branches.begin() + element) -1 ].value;
		else
			U0[element] = 0;
	}

	for(uint8_t row = 0; row < branches.size(); row++)
	{
		cout << U0[row] + 0 << " ";
	}
	cout << endl;

	// Вывод на экран номера ветвей дерева
	for(uint8_t element = 0; element < branches.size(); element++)
		cout << branches[element] + 0 << " ";
	cout << endl;

}

// 6. Составление уравнений токов хорд:
void ECC::create_equations_current_of_chords()
{
	// Матрица проводимости хорд:
	Gchord = new long double* [chords.size()];
	for(uint8_t element = 0; element < chords.size(); element++)
	{
		Gchord[element] = new long double[chords.size()];
		for(uint8_t col = 0; col < chords.size(); col++)
			Gchord[element][col] = 0;
		if(dataArr[ *(chords.begin() + element) -1 ].type == 'r')
			Gchord[element][element] = 1 / dataArr[ *(chords.begin() + element) -1 ].value;
	}

	for(uint8_t row = 0; row < chords.size(); row++)
	{
		for(uint8_t col = 0; col < chords.size(); col++)
		{
			cout << Gchord[row][col] + 0 << " ";
		}
		cout << endl;
	}
	cout << endl;

	// Вектор токов хорд:
	I0 = new long double[chords.size()];
	for(uint8_t element = 0; element < chords.size(); element++)
	{
		if(dataArr[ *(chords.begin() + element) -1 ].type == 'i')
			I0[element] = dataArr[ *(chords.begin() + element) -1 ].value;
		else
			I0[element] = 0;
	}

	for(uint8_t row = 0; row < chords.size(); row++)
	{
		cout << I0[row] + 0 << " ";
	}
	cout << endl;

	// Вывод на экран номера хорд
	for(uint8_t element = 0; element < chords.size(); element++)
		cout << chords[element] + 0 << " ";
	cout << endl;
}

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


// 9. Выделение фундаментальной матрицы
void ECC::allocate_fundamental_matrix()
{
	identity(structuralMatrix, numberOfNodes, numberOfElements);
	fundamentalMatrix = new __int8_t* [numberOfNodes];
	for (__uint8_t row = 0; row < numberOfNodes; row++)
	{
		fundamentalMatrix[row] = new __int8_t [numberOfElements - numberOfNodes];
		memcpy(fundamentalMatrix[row], structuralMatrix[row] + numberOfNodes, numberOfElements - numberOfNodes);
	}

}

// 11. Алгоритм расчета цепи
void ECC::calculate()
{

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
