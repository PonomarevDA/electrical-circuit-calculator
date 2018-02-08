#include <iostream>
#include <func.hpp>
#include <cstring>

using namespace std;

// 1. Ввод данных
void ECC::data_input()
{
	cout << "It does not work yet :(\n";
	//char arr[9][4];
	//__uint8_t length = 0;
}

// 2. Создание структурной матрицы по ЗТК из массива входных данных
void ECC::create_structural_matrix_with_use_current()
{
	numberOfElements = NUMBER_OF_ELEMENTS;
	numberOfNotes = NUMBER_OF_NOTES;

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


// 3. Исключение базисного узла
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


// 4. Преобразование матирцы в единичную
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
