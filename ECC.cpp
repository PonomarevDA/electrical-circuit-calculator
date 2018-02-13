#include <iostream>
#include <deque>
#include <set>
#include <ECC.hpp>
#include <types.hpp>
#include <cstring>

using namespace std;

// 1. Ввод данных
void ECC::input_data()
{
	// Кусок кода ниже заменить на ввод данных из файла со временем:
	numberOfElements = 5;
	dataArr = new dataStr[numberOfElements];
	dataArr[0].init(1, 1, 4, 'u', 5);
	dataArr[1].init(2, 1, 2, 'r', 1000);
	dataArr[2].init(3, 3, 4, 'r', 1000);
	dataArr[3].init(4, 2, 3, 'r', 1000);
	dataArr[4].init(5, 4, 2, 'i', 0.005);
	// Кусок кода выше заменить на ввод данных из файла со временем:

	// Подсчет количества узлов:
	numberOfNodes = 0;
	for(__int8_t count = 0; count < numberOfElements; count++)
		numberOfNodes = (numberOfNodes > dataArr[count].nodeFirst) ? numberOfNodes : dataArr[count].nodeFirst;
}

// 2. Предварительная сортировка входных данных
void ECC::sort_data()
{
	// Сначала ИН, затем R, в конце ИТ
}

// 3. Создание матрицы смежности
void ECC::create_adjacency_matrix()
{
	// Выделение памяти для матрицы смежности (считаем граф неориентированным):
	adjacencyMatrix = new bool* [numberOfNodes];
	for (__uint8_t count = 0; count < numberOfNodes; count++)
		adjacencyMatrix[count] = new bool [NUMBER_OF_ELEMENTS];

	// Заполняем матрицу смежности: 1 - есть ребро, 0 - нет ребра
	for(__int8_t count = 0; count < numberOfElements; count++)
	{
		adjacencyMatrix[ (dataArr[count].nodeFirst) - 1][ (dataArr[count].nodeLast) - 1] = 1;
		adjacencyMatrix[ (dataArr[count].nodeLast) - 1][ (dataArr[count].nodeFirst) - 1] = 1;
	}

	// Вывод матрицы смежности на экран:
	for(__uint8_t i = 0; i < numberOfNodes; i++)
	{
		for(__uint8_t j = 0; j < numberOfNodes; j++)
		{
			cout << adjacencyMatrix[i][j] + 0 << " ";
		}
		cout << endl;
	}
}

// 4. Поиск остовного дерева
void ECC::find_spanning_tree()
{
	// Init
	deque<uint8_t> nodesFuture;
	deque<uint8_t> nodesPresent;
	deque<uint8_t> nodesPast;	// обязательность сомнительна
	set<uint8_t> chords;		// обязательность сомнительна
	set<uint8_t> branches;

	// Заполняем контейнеры nodesFuture и chords:
	for(uint8_t node = 1; node <= numberOfNodes; node++)
	{
		nodesFuture.push_back(node);
		chords.insert(node);
	}
	for(uint8_t element = numberOfNodes + 1; element <= numberOfElements; element++)
		chords.insert(element);

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
		nodesPast.push_back(nodesPresent.at(0));
		nodesPresent.erase(nodesPresent.begin());
	}

	// Вывод на экран содержимого списка
	cout << "nodesFuture: ";
	for(uint8_t node = 0; node < nodesFuture.size(); node++)
		cout << nodesFuture.at(node) + 0 << " ";

	cout << "\nnodesPresent: ";
	for(uint8_t node = 0; node < nodesPresent.size(); node++)
		cout << nodesPresent.at(node) + 0 << " ";

	cout << "\nnodesPast: ";
	for(uint8_t node = 0; node < nodesPast.size(); node++)
		cout << nodesPast.at(node) + 0 << " ";

	cout << "\nchords: ";
	for(uint8_t element = 0; element < chords.size(); element++)
		cout << *chords.begin()+element + 0 << " ";

	cout << "\nbranches: ";
	for(uint8_t element = 0; element < branches.size(); element++)
		cout << *branches.begin()+element + 0 << " ";
	cout << endl;

}

// 5. Создание структурной матрицы по ЗТК из массива входных данных
void ECC::create_oriented_graph()
{
	// Выделение памяти для структурной матрицы
	structuralMatrix = new __int8_t* [NUMBER_OF_NODES];
	for (__uint8_t count = 0; count < NUMBER_OF_NODES; count++)
			structuralMatrix[count] = new __int8_t [NUMBER_OF_ELEMENTS];

	__int8_t buffer[NUMBER_OF_NODES][NUMBER_OF_ELEMENTS] = {
		{1,  1,  0,  0,  0},
		{0, -1,  0,  1, -1},
		{0,  0,  1, -1,  0},
		{-1, 0, -1,  0,  1}};

	for(__int8_t count = 0; count < NUMBER_OF_NODES; count++)
		memcpy(structuralMatrix[count], buffer[count], NUMBER_OF_ELEMENTS);
}


// 6. Исключение базисного узла
void ECC::elimination_of_matrix_dependency()
{
	bool isMatrixDependent = 0;
	for (__int8_t countOfNode = 0; countOfNode < NUMBER_OF_NODES; countOfNode++)
	{
		__int8_t sum_of_row = 0;
		for (__int8_t countOfElement = 0; countOfElement < NUMBER_OF_ELEMENTS; countOfElement++)
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


// 7. Преобразование матирцы в единичную
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

// 8. Выделение фундаментальной матрицы
void ECC::allocate_fundamental_matrix()
{
	fundamentalMatrix = new __int8_t* [numberOfNodes];
	for (__uint8_t row = 0; row < numberOfNodes; row++)
	{
		fundamentalMatrix[row] = new __int8_t [numberOfElements - numberOfNodes];
		memcpy(fundamentalMatrix[row], structuralMatrix[row] + numberOfNodes, numberOfElements - numberOfNodes);
	}

}

// Последовательное выполнение всех методов
void ECC::start()
{
}


// Форматированный вывод структурной матрицы
void ECC::structural_matrix_output()
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
void ECC::fundamental_matrix_output()
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
