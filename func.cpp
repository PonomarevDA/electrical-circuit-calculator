#include <iostream>
#include <func.hpp>
#include <cstring>

using namespace std;

void ECC::data_input()
{
	cout << "It does not work yet :(\n";
}

//
void ECC::create_structural_matrix_with_use_current()
{
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

// Форматированный вывод структурной матрицы
void ECC::structural_matrix_output()
{
	for(__uint8_t i = 0; i < 4; i++)
	{
		for(__uint8_t j = 0; j < 5; j++)
		{
			if (!((structuralMatrix[i][j] >> 7) & 1))
				cout << " ";
			cout << structuralMatrix[i][j] + 0 << " ";

		}
		cout << endl;
	}
}

//
void ECC::elimination_of_matrix_dependency()
{
	bool isMatrixDependent = 0;
	// Проверка на линейную зависимость матрицы
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
	}
}


//
void ECC::conversion_to_the_identity_matrix()
{
	cout << "It does not work yet :(\n";
}
