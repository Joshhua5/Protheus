#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Pro; 
using namespace Math;

int main() {
	// Basic Matrix test  
	char exit_status = 0;

	float mat1_dat[4][4] = {
		{ 2, 1, 5, 2 },
		{ 5, 4, 3, 4 },
		{ 1, 2, 4, 2 },
		{ 5, 2, 1, 5 },
	};

	float mat2_dat[4][4] = {
		{ 3, 2, 7, 2 },
		{ 4, 1, 5, 2 },
		{ 1, 2, 4, 5 },
		{ 1, 5, 2, 3 },
	};

	float expected_mul_answer[4][4] = {
		{ 17, 25, 43, 37 },
		{ 38, 40, 75, 45 },
		{ 17, 22, 37, 32 },
		{ 29, 39, 59, 34 },
	};
	float expected_add_answer[4][4] = {
		{ 5, 3, 12, 4 },
		{ 9, 5, 8, 6 },
		{ 2, 4, 8, 7 },
		{ 6, 7, 3, 8 },
	};
	float expected_sub_answer[4][4] = {
		{ -1, -1, -2,  0 },
		{ 1,  3, -2,  2 },
		{ 0,  0,  0, -3 },
		{ 4, -3, -1,  2 },
	};
	Matrix44 ex_mul_ans(expected_mul_answer);
	Matrix44 ex_add_ans(expected_add_answer);
	Matrix44 ex_sub_ans(expected_sub_answer);
	int expected_mat1_determinate = 77;
	int expected_mat2_determinate = -215;
	Matrix44 Mat1(mat1_dat);
	Matrix44 Mat2(mat2_dat);

	auto mul_ans = Mat1 * Mat2;
	auto add_ans = Mat1 + Mat2;
	auto sub_ans = Mat1 - Mat2;

	// Compare results

	if (mul_ans == ex_mul_ans)
		std::cout << "Multiplication of Matrix44 was successful" << std::endl;
	else {
		std::cout << "Multiplication of Matrix44 has failed" << std::endl;
		exit_status |= 1;
	}

	if (add_ans == ex_add_ans)
		std::cout << "Addition of Matrix44 was successful" << std::endl;
	else {
		std::cout << "Addition of Matrix44 has failed" << std::endl;
		exit_status |= 2;
	}

	if (sub_ans == ex_sub_ans)
		std::cout << "Subtraction of Matrix44 was successful" << std::endl;
	else {
		std::cout << "Subtraction of Matrix44 has failed" << std::endl;
		exit_status |= 4;
	}

	if (Mat1.Determinate() == expected_mat1_determinate && Mat2.Determinate() == expected_mat2_determinate)
		std::cout << "Determinate of Matrix44 was successful" << std::endl;
	else {
		std::cout << "Determinate of Matrix44 has failed" << std::endl;
		exit_status |= 8;
	}

	return exit_status;
}
