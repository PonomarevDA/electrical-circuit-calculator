#include <ECC.hpp>

using namespace std;


// 1. Ввод данных
void ECC::input_data()
{
	// Кусок кода ниже заменить на ввод данных из файла:
	#define var2;
	#ifdef var1
	numberOfElements = 5;
	dataArr = new dataStr[numberOfElements];
	dataArr[0].init(1, 1, 4, 'u', 5);
	dataArr[1].init(2, 1, 2, 'r', 1000);
	dataArr[2].init(3, 3, 4, 'r', 1000);
	dataArr[3].init(4, 2, 3, 'r', 1000);
	dataArr[4].init(5, 4, 2, 'i', 0.01);
	#endif
	#ifdef var2
	numberOfElements = 4;
	dataArr = new dataStr[numberOfElements];
	dataArr[0].init(1, 1, 3, 'u', 1);
	dataArr[1].init(2, 3, 2, 'u', 5);
	dataArr[2].init(3, 1, 2, 'r', 2);
	dataArr[3].init(4, 3, 1, 'i', 1);
	#endif

	// Кусок кода выше заменить на ввод данных из файла.

	// Вывод данных в терминал:
	output_data(dataArr, numberOfElements);

	// Подсчет количества узлов:
	numberOfNodes = 0;
	for(__int8_t count = 0; count < numberOfElements; count++)
		numberOfNodes = (numberOfNodes > dataArr[count].nodeFirst) ? numberOfNodes : dataArr[count].nodeFirst;
}

// 2. Сортировка входных данных
void ECC::sort_data()
{
	// Сначала ИН, затем R, в конце ИТ
	cout << "Предполагается, что массив данных изначально отсортирован..." << endl;
}

// 3. Создание матрицы смежности
void ECC::create_adjacency_matrix()
{
	// Выделение памяти для матрицы смежности (считаем граф неориентированным):
	adjacencyMatrix.Init(numberOfNodes, numberOfNodes);

	// Заполняем матрицу смежности: 1 - есть ребро, 0 - нет ребра
	for(uint8_t count = 0; count < numberOfElements; count++)
	{
		adjacencyMatrix[ (dataArr[count].nodeFirst) - 1][ (dataArr[count].nodeLast) - 1] = 1;
		adjacencyMatrix[ (dataArr[count].nodeLast) - 1][ (dataArr[count].nodeFirst) - 1] = 1;
	}

	// Вывод матрицы смоежности в терминал
	adjacencyMatrix.Show();
}

// 4. Поиск остовного дерева
// Остовное дерево - минимальное подмножество ребер графа, таких, что из
// любой вершины графа можно попасть в любую вершину, двигаясь по этим ребрам.
// Остовное дерево должно включать все ИН!
// Остовное дерево не должно содержать ИТ!
void ECC::find_spanning_tree()
{
	vector<uint8_t> outsideNodes;		// узлы, еще не принадлежащие остовному дереву
	vector<uint8_t> edgeNodes;				// крайние узлы ветвей дерева

	// Узлы источников напряжения
	vector<uint8_t> nodesOfVoltageSource;
	uint8_t countOfVoltageSource = 0;
	while (dataArr[countOfVoltageSource].type == 'u')
	{
		nodesOfVoltageSource.push_back( dataArr[countOfVoltageSource].nodeFirst );
		nodesOfVoltageSource.push_back( dataArr[countOfVoltageSource].nodeLast );
		countOfVoltageSource++;
	}
	for( uint8_t count = 0; count < nodesOfVoltageSource.size(); count++)
		cout << nodesOfVoltageSource[count] + 0 << " ";
	cout << endl;

	// Заполняем контейнеры nodesFuture и chords:
	for(uint8_t node = 1; node <= numberOfNodes; node++)
		outsideNodes.push_back(node);
	for(uint8_t element = 1; element <= numberOfElements; element++)
		chords.push_back(element);

	// Пусть первый элемен массива dataArr (№0) будет первой ветвью дерева
	chords.erase(chords.begin());
	branches.push_back(1);
	edgeNodes.push_back(dataArr[0].nodeFirst);
	edgeNodes.push_back(dataArr[0].nodeLast);
	for(uint8_t index = 0; index < outsideNodes.size(); index++)
	{
		if(dataArr[0].nodeFirst == outsideNodes[index] || dataArr[0].nodeLast == outsideNodes[index])
		{
			outsideNodes.erase(outsideNodes.begin() + index);
			index--;
		}
	}


	// Пока существуют внешние узлы (не принадлежащие остовному дереву)
	task_t task = FIND_VOLTAGE_SOURCE;
	while(!outsideNodes.empty())
	{
		for(uint8_t index1 = 0; index1 < edgeNodes.size(); index1++)
		{
			error_t searchResult = CANT_FIND;
			for(uint8_t index2 = 0; index2 < outsideNodes.size(); index2++)
			{
				searchResult = make_nodes_a_branch_if_it_possible(outsideNodes[index2], edgeNodes[index1], task);
				if (searchResult == OK)
				{
					edgeNodes.push_back(outsideNodes[index2]);
					outsideNodes.erase(outsideNodes.begin() + index2);
					break;
				}
			}
			if (searchResult == OK)
			{
				if (task == FIND_OTHER_ELEMENT)
				{
					task = FIND_VOLTAGE_SOURCE;
					break;
				}
			}
			else if (searchResult == CANT_FIND)
			{
				if (task == FIND_OTHER_ELEMENT)
				{
					edgeNodes.erase(edgeNodes.begin() + index1);
					index1--;
				}
			}

		}
		if(task == FIND_VOLTAGE_SOURCE)
		{
			task = FIND_OTHER_ELEMENT;
		}

	}

	// Вывод на экран содержимого векторов
	cout << "chords: ";
	for(uint8_t element = 0; element < chords.size(); element++)
		cout << chords[element] + 0 << " ";

	cout << "\nbranches: ";
	for(uint8_t element = 0; element < branches.size(); element++)
		cout << branches[element] + 0 << " ";
	cout << endl;
}

// 5. Составление уравнений напряжения ветвей дерева:
void ECC::create_equations_voltage_of_branches()
{
	// Матрица сопротивлений ветвей дерева:
	Rtree.Init(branches.size(), branches.size());
	for(uint8_t element = 0; element < branches.size(); element++)
	{
		for(uint8_t col = 0; col < branches.size(); col++)
			Rtree[element][col] = 0;
		if(dataArr[ *(branches.begin() + element) -1 ].type == 'r')
			Rtree[element][element] = dataArr[ *(branches.begin() + element) -1 ].value;
	}
	cout << "Rtree:" << endl;
	Rtree.Show();
	cout << endl;

	// Вектор напряжений ветвей дерева:
	U0.Init(branches.size(), 1);
	for(uint8_t element = 0; element < branches.size(); element++)
	{
		if(dataArr[ *(branches.begin() + element) -1 ].type == 'u')
			U0[element][0] = dataArr[ *(branches.begin() + element) -1 ].value;
		else
			U0[element][0] = 0;
	}
	cout << "U0:" << endl;
	U0.Show();
	cout << endl;

	// Вывод на экран номера ветвей дерева
	cout << "Sequence of branches:" << endl;
	for(uint8_t element = 0; element < branches.size(); element++)
		cout << branches[element] + 0 << " ";
	cout << endl;
}

// 6. Составление уравнений токов хорд:
void ECC::create_equations_current_of_chords()
{
	// Матрица проводимости хорд:
	Gchord.Init( chords.size(), chords.size() );
	for(uint8_t element = 0; element < chords.size(); element++)
	{
		for(uint8_t col = 0; col < chords.size(); col++)
			Gchord[element][col] = 0;
		if(dataArr[ *(chords.begin() + element) -1 ].type == 'r')
			Gchord[element][element] = 1 / dataArr[ *(chords.begin() + element) -1 ].value;
	}
	cout << "Gchords:" << endl;
	Gchord.Show();
	cout << endl;

	// Вектор токов хорд:
	I0.Init(chords.size(), 1);
	for(uint8_t element = 0; element < chords.size(); element++)
	{
		if(dataArr[ *(chords.begin() + element) -1 ].type == 'i')
			I0[element][0] = dataArr[ *(chords.begin() + element) -1 ].value;
		else
			I0[element][0] = 0;
	}
	cout << "I0:" << endl;
	I0.Show();
	cout << endl;

	// Вывод на экран номера хорд
	cout << "Sequence of chords:" << endl;
	for(uint8_t element = 0; element < chords.size(); element++)
		cout << chords[element] + 0 << " ";
	cout << endl;
}

// 7. Создание структурной матрицы по ЗТК из массива входных данных
void ECC::create_oriented_graph()
{
	// Выделение памяти для структурной матрицы и инициализация
	structuralMatrix.Init(numberOfNodes, numberOfElements);
	for (__uint8_t row = 0; row < numberOfNodes; row++)
	{
		for(__uint8_t col = 0; col < numberOfElements; col++)
			structuralMatrix[row][col] = 0;
	}

	for(__int8_t elem = 0; elem < numberOfElements; elem++)
	{
		structuralMatrix[dataArr[elem].nodeFirst - 1][elem] = 1;
		structuralMatrix[dataArr[elem].nodeLast - 1][elem] = -1;
	}

	// Вывод матрицы в терминал:
	structuralMatrix.Show();
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
			if (isMatrixDependent = (sum_of_row != 0) )
				break;
		}
		if (isMatrixDependent)
			break;
	}

	if (isMatrixDependent)
	{
		cout << "Матрица линейно зависима, сделаем ее независимой\n";
		structuralMatrix.deleteLastRow();
		numberOfNodes--;
	}

	// Вывод матрицы в терминал:
	structuralMatrix.Show();
}


// 9. Выделение фундаментальной матрицы
void ECC::allocate_fundamental_matrix()
{
	structuralMatrix.identity();
	structuralMatrix.Show();
	F.Init(numberOfNodes, numberOfElements - numberOfNodes);
	for (uint8_t row = 0; row < numberOfNodes; row++)
		memcpy(F.Data[row], structuralMatrix.Data[row] + numberOfNodes, (numberOfElements - numberOfNodes)*sizeof(double));

	// Вывод матрицы в терминал:
	cout << "F:" << endl;
	F.Show();
}

// 10. Алгоритм расчета цепи
void ECC::calculate()
{
	Itree.Init(branches.size(), 0);
	{
		Matrix bufferLeft(branches.size(), branches.size());
		Matrix bufferRight(branches.size(), branches.size());
		Matrix buffer(branches.size(), branches.size());
		Matrix ones(branches.size(), branches.size());
		for(uint8_t row = 0; row < branches.size(); row++)
		{
			for(uint8_t col = 0; col < branches.size(); col++)
			{
				ones[row][col] = 0;
				if (row == col)
					ones[row][col] = 1;
			}
		}
		bufferLeft = F * Gchord * F.transpose() * Rtree + ones;
		bufferLeft.inverse();
		bufferRight = (F * Gchord * F.transpose() * U0);
		buffer = F * I0;
		bufferRight = bufferRight + buffer;
		Itree = bufferLeft * bufferRight * (-1);
		cout << endl;
		Itree.Show();
	}

	Uchord.Init(chords.size(), 0);
	{
		Matrix ones	(chords.size(), chords.size());
		Matrix buffer(chords.size(), chords.size());
		Matrix bufferRight(chords.size(), chords.size());
		Matrix bufferLeft(chords.size(), chords.size());
		for(uint8_t row = 0; row < chords.size(); row++)
		{
			for(uint8_t col = 0; col < chords.size(); col++)
			{
				ones[row][col] = 0;
				if (row == col)
					ones[row][col] = 1;
			}
		}
		bufferLeft = F.transpose() * Rtree * F * Gchord + ones;
		bufferLeft = bufferLeft.inverse();
		bufferRight = (F.transpose() * Rtree * F * I0);
		buffer = F.transpose() * U0 * (-1);
		bufferRight = bufferRight + buffer;
		Uchord = bufferLeft * bufferRight * (-1);
		cout << endl;
		Uchord.Show();
	}
}


/*Private area*/
void ECC::output_data(dataStr* dataArr, uint8_t lenght)
{
	for(uint8_t count = 0; count < lenght; count++)
	{
		cout << setw(2) << dataArr[count].index + 0;
		cout << setw(3) << dataArr[count].nodeFirst + 0;
		cout << setw(3) << dataArr[count].nodeLast + 0;
		cout << setw(3) << dataArr[count].type;
		cout << setw(6) << dataArr[count].value + 0;
		cout << endl;
	}
}


error_t ECC::make_nodes_a_branch_if_it_possible(uint8_t node1, uint8_t node2, task_t task)
{
	error_t error = CANT_FIND;
	// Образует ли первый узел nodesPresent с узлом индекса index ветвь дерева?
	if( adjacencyMatrix[node1][node2] )
	{
		uint8_t number = find_number_of_element(node1, node2);

		// Проверка, что данный элемнент уже не является ветвью дерева
		enum resultOfCheck_t{SATISFY, DOES_NOT_SATISFY};
		resultOfCheck_t resultOfCheck = SATISFY;
		for(uint8_t count = 0; count < branches.size(); count++)
		{
			if(branches[count] == number)
				resultOfCheck = DOES_NOT_SATISFY;
		}
		// Проверка, что данный элемент не является ИТ
		if (resultOfCheck == SATISFY)
		{
			if (task == FIND_VOLTAGE_SOURCE)
			{
				if(dataArr[number - 1].type != 'u')
					resultOfCheck = DOES_NOT_SATISFY;
			}
			else
			{
				if(dataArr[number - 1].type == 'i')
					resultOfCheck = DOES_NOT_SATISFY;
			}
		}

		// Если элемент подходит, то он - ветвь дерева
		if (resultOfCheck == SATISFY)
		{
			branches.insert(branches.end(), number);
			for(uint8_t count = 0; count < chords.size(); count++)
			{
				if(chords[count] == number)
				{
					chords.erase(chords.begin() + count);
					break;
				}
			}
			error = OK;
		}
	}
	return error;
}

uint8_t ECC::find_number_of_element(const uint8_t node1, const uint8_t node2)
{
	uint8_t number = 0;
	for(number = 0; number < numberOfElements; number++)
	{
		if( ( (dataArr[number].nodeFirst == node1 ) &&
			  (dataArr[number].nodeLast == node2) ) ||
			( (dataArr[number].nodeLast == node1 ) &&
			  (dataArr[number].nodeFirst == node2) ) )
		{
			number++;
			break;
		}
	}
	return number;
}
