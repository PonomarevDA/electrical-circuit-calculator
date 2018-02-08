#ifndef FUNC_HPP
#define FUNC_HPP

// electrical circuit calculator
class ECC
{
public:
	enum{ NUMBER_OF_NOTES = 4, NUMBER_OF_ELEMENTS = 5};
	void data_input();
	void create_structural_matrix_with_use_current();
//	void create_structural_matrix_with_use_voltage();
	void elimination_of_matrix_dependency();
	void conversion_to_the_identity_matrix();
	void allocate_fundamental_matrix();

//	void calculate();


	void structural_matrix_output();

private:
	__int8_t** inputData;
	long double** RMatrix;
	long double** UMatrix;
	long double** IMatrix;

	__uint8_t numberOfNotes;
	__uint8_t numberOfElements;
	__int8_t** structuralMatrix;
	__int8_t** fundamentalMatrix;

};


#endif // FUNC

