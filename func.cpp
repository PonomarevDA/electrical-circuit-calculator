#include <iostream>
#include <func.hpp>
#include <cstring>

using namespace std;

// 1. Ввод данных
void ECC::data_input()
{
	cout << "Write the number of elements: ";
	//cin >> numberOfElements;
	numberOfElements = 5; cout << "5\n";

	inputData = new dataStr[numberOfElements];

	cout << "Write the data in format \"Index noteFirst noteLast type value\".\n"
		 << "For example: \"1 1 4 u 5\" (voltage source 5V)\n"
		 << "Your input:\n";
	inputData[0].init(1, 1, 4, 'u', 5);
	inputData[1].init(2, 1, 2, 'r', 1000);
	inputData[2].init(3, 3, 4, 'r', 1000);
	inputData[3].init(4, 2, 3, 'r', 1000);
	inputData[4].init(5, 4, 2, 'i', 0.005);

	numberOfNotes = 0;
	for(__int8_t count = 0; count < numberOfElements; count++)
		numberOfNotes = (numberOfNotes > inputData[count].noteFist) ? numberOfNotes : inputData[count].noteFist ;
}

// 2. Сортировка входных данных

// 3. Создание матрицы смежности
void ECC::create_adjacency_matrix()
{
	// Выделение памяти для матрицы смежности (считаем граф неориентированным):
	adjacencyMatrix = new bool* [numberOfNotes];
	for (__uint8_t count = 0; count < numberOfNotes; count++)
		adjacencyMatrix[count] = new bool [NUMBER_OF_ELEMENTS];

	// Заполняем матрицу смежности: 1 - есть ребро, 0 - нет ребра
	for(__int8_t count = 0; count < numberOfElements; count++)
	{
		adjacencyMatrix[ (inputData[count].noteFist) - 1][ (inputData[count].noteLast) - 1] = 1;
		adjacencyMatrix[ (inputData[count].noteLast) - 1][ (inputData[count].noteFist) - 1] = 1;
	}

	// Вывод матрицы смежности на экран:
	for(__uint8_t i = 0; i < numberOfNotes; i++)
	{
		for(__uint8_t j = 0; j < numberOfNotes; j++)
		{
			cout << adjacencyMatrix[i][j] + 0 << " ";
		}
		cout << endl;
	}
}

// 4. Поиск остовного дерева

// 5. Создание структурной матрицы по ЗТК из массива входных данных
void ECC::create_oriented_graph()
{
	// Выделение памяти для структурной матрицы
	structuralMatrix = new __int8_t* [NUMBER_OF_NOTES];
	for (__uint8_t count = 0; count < NUMBER_OF_NOTES; count++)
			structuralMatrix[count] = new __int8_t [NUMBER_OF_ELEMENTS];

	__int8_t buffer[NUMBER_OF_NOTES][NUMBER_OF_ELEMENTS] = {
		{1,  1,  0,  0,  0},
		{0, -1,  0,  1, -1},
		{0,  0,  1, -1,  0},
		{-1, 0, -1,  0,  1}};

	for(__int8_t count = 0; count < NUMBER_OF_NOTES; count++)
		memcpy(structuralMatrix[count], buffer[count], NUMBER_OF_ELEMENTS);
}


// 6. Исключение базисного узла
void ECC::elimination_of_matrix_dependency()
{
	bool isMatrixDependent = 0;
	for (__int8_t countOfNote = 0; countOfNote < NUMBER_OF_NOTES; countOfNote++)
	{
		__int8_t sum_of_row = 0;
		for (__int8_t countOfElement = 0; countOfElement < NUMBER_OF_ELEMENTS; countOfElement++)
		{
			sum_of_row=+structuralMatrix[countOfNote][countOfElement];
			if (isMatrixDependent = sum_of_row != 0)
				break;
		}
		if (isMatrixDependent)
			break;
	}

	if (isMatrixDependent)
	{
		cout << "Матрица линейно зависима, сделаем ее независимой\n";
		numberOfNotes--;
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
	for (__uint8_t countConv = 0; countConv < numberOfNotes; countConv++)
	{
		// Перестановка строк матрицы, если необходимо:
		if(structuralMatrix[countConv][countConv] == 0)
		{
			memcpy(bufferRow, (*structuralMatrix + countConv*numberOfElements), numberOfElements - countConv);
			for(__uint8_t countSort = countConv + 1; countSort < numberOfNotes; countSort++)
			{
				if(structuralMatrix[countSort][countConv] != 0)
				{
					memcpy((*structuralMatrix + countConv*numberOfElements), (*structuralMatrix + countSort*numberOfElements + countConv), numberOfElements - countConv);
					memcpy((*structuralMatrix + countSort*numberOfElements + countConv), bufferRow, numberOfElements - countConv);
					continue;
				}
				if(countSort == numberOfNotes - 1)
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
			for(__uint8_t rowZero = countConv + 1; rowZero < numberOfNotes; rowZero++)
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
		for (__int8_t rowZero = numberOfNotes - 2; rowZero >= 0; rowZero--)
		{
			for (__uint8_t colZero = rowZero + 1; colZero < numberOfNotes; colZero++)
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
	fundamentalMatrix = new __int8_t* [numberOfNotes];
	for (__uint8_t row = 0; row < numberOfNotes; row++)
	{
		fundamentalMatrix[row] = new __int8_t [numberOfElements - numberOfNotes];
		memcpy(fundamentalMatrix[row], structuralMatrix[row] + numberOfNotes, numberOfElements - numberOfNotes);
	}

}


// Форматированный вывод структурной матрицы
void ECC::structural_matrix_output()
{
	for(__uint8_t i = 0; i < numberOfNotes; i++)
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
	for(__uint8_t i = 0; i < numberOfNotes; i++)
	{
		for(__uint8_t j = 0; j < numberOfElements - numberOfNotes; j++)
		{
			if (!((fundamentalMatrix[i][j] >> 7) & 1))
				cout << " ";
			cout << fundamentalMatrix[i][j] + 0 << " ";

		}
		cout << endl;
	}
}
