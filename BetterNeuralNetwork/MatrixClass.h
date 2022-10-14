#pragma once

#include <iostream>
#include "Randomer.h"
#include <Windows.h>
#include <vector>

namespace mxLib
{
	template<typename MatrixType>
	class Matrix
	{
	private:
		int cols = 0, rows = 0;
		std::vector<MatrixType> arr;
		MatrixType sum = 0;

		int* storageRows = 0, * storageCols = 0;
		MatrixType** MatrixArray2D;
		bool alreadyWas = false;



	public:


		template<class T>   // prototyp sablony
		Matrix(T rows1, T cols1)
		{
			rows = rows1;
			cols = cols1;
			MatrixArray2D = new MatrixType* [rows];

			// for loop nacte hodnoty na zacatku na hodnotu 0 
			for (auto i = 0; i < rows; ++i) {
				MatrixArray2D[i] = new MatrixType[cols];
			}

			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					MatrixArray2D[i][j] = 0;
				}
			}
			alreadyWas = true;

		}

		Matrix()
		{

		}

		~Matrix()
		{
			for (auto i = 0; i < rows; ++i)
				delete[] MatrixArray2D[i];
			delete[] MatrixArray2D;
		}

		Matrix& operator=(Matrix<MatrixType> matrix)
		{
			if (this == &matrix)
				return *this;

			rows = matrix.rows;
			cols = matrix.cols;

			MatrixArray2D = new MatrixType * [rows];

			// for loop nacte hodnoty na zacatku na hodnotu 0 
			for (auto i = 0; i < rows; ++i) {
				MatrixArray2D[i] = new MatrixType[cols];
			}

			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					MatrixArray2D[i][j] = 0;
				}
			}
			alreadyWas = true;

			for (int i = 0; i < matrix.rows; ++i)
				for (int j = 0; j < matrix.cols; ++j)
					MatrixArray2D[i][j] = matrix.MatrixArray2D[i][j];

			sum = matrix.sum;
			arr = matrix.arr;			//alreadyWas = matrix.alreadyWas;
			storageCols = matrix.storageCols;
			storageRows = matrix.storageRows;

			return *this;

		}

		void setRandomValue(MatrixType min, MatrixType max)
		{
			// funkce ktera po zavolani nastavi konkretni matici na nahodnou hodnotu
			if (alreadyWas == true)
			{
				Randomer<MatrixType> random{ min, max }; // random funkce ktera podle zadanych definic vybira nahodne hodnoty

				for (auto i = 0; i < rows; ++i) {
					for (auto j = 0; j < cols; ++j) {
						MatrixArray2D[i][j] = random();

					}
				}

			}

			else {
				std::cout << "nemuzete nahodne definovat neexistujici matici!!" << std::endl;
			}
		}

		template<class T>
		void addWithSingleNumber(T allPlusNumber)
		{
			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					MatrixArray2D[i][j] += allPlusNumber;

				}
			} // tato funkce secte kazdy prvek v matici jednim stejnym cislem
		}

		template<class T>
		void addTwoMatrix(T matrix1)
		{
			// tato funkce secte cele dve matice pomoci jednoho for loopu
			if (rows == matrix1.rows && cols == matrix1.cols) {

				// podminky ktere rozhoduji jestli jsou radky stejne
				// protoze pri scitani dvou matic musi byt obe stejne velke

				for (auto i = 0; i < matrix1.rows; ++i) {
					for (auto j = 0; j < matrix1.cols; ++j) {
						MatrixArray2D[i][j] = matrix1.MatrixArray2D[i][j] + MatrixArray2D[i][j];
					}
				}

			}  
			else {
				std::cout << "Neni mozno scitat matici o ruznych rozmerech!! " << std::endl;
			}
		}

		template<class T>
		void multiplyWithSingleNumber(T allPlusNumber)
		{
			// tato funkce vynasobi kazde prvek v matici celim cislem
			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					MatrixArray2D[i][j] *= allPlusNumber;

				}
			}

		}

		template<class T>
		void multiplyTwoMatrix(T matrix1, T matrix2)
		{

			if (alreadyWas != true)
			{
				rows = matrix1.rows;
				cols = matrix2.cols;

				MatrixArray2D = new MatrixType* [rows];

				// for loop nacte hodnoty na zacatku na hodnotu 0 
				for (auto i = 0; i < rows; ++i) {
					MatrixArray2D[i] = new MatrixType[cols];
				}

				for (auto i = 0; i < rows; ++i) {
					for (auto j = 0; j < cols; ++j) {

						MatrixArray2D[i][j] = 0;

						if (i == rows - 1 && j == cols - 1) {
							alreadyWas = true;
						}
					}
				}

			}



			if (matrix1.cols == matrix2.rows) {

				for (auto i = 0; i < matrix1.rows; ++i) {
					for (auto j = 0; j < matrix2.cols; ++j) {
						MatrixType sum = 0;
						for (auto k = 0; k < matrix1.cols; ++k)
						{
							sum += matrix1.MatrixArray2D[i][k] * matrix2.MatrixArray2D[k][j];
						}
						MatrixArray2D[i][j] = sum;
					}
				}

			}
			else {
				std::cout << "pri nasobeni matice matici se musi pocet radku prvni matice rovnat poctu sloupcu druhe matice!! " << std::endl;
			}

		}

		template<class T>
		void multiplyTwoMatrixWithOneOther(T matrix)
		{
			if (rows == matrix.rows && cols == matrix.cols) {
				for (auto i = 0; i < rows; ++i) {
					for (auto j = 0; j < cols; ++j) {
						MatrixArray2D[i][j] *= matrix.MatrixArray2D[i][j];
					}
				}

			}
		}
		void showValues()
		{
				std::cout << "Hodnoty matice: " << cols << " " << rows << std::endl;
			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
							std::cout << MatrixArray2D[i][j] << "   ";
				}
					std::cout << std::endl;
			}
		}

		template<class T>
		void transpose(T matrixForTranspose)
		{

			cols = matrixForTranspose.rows;
			rows = matrixForTranspose.cols;   // 2x1


			MatrixArray2D = new MatrixType* [rows];

			for (auto i = 0; i < rows; ++i) {
				MatrixArray2D[i] = new MatrixType[cols];
			}

			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					MatrixArray2D[i][j] = 0;
				}
			}
			
			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					MatrixArray2D[i][j] = matrixForTranspose.MatrixArray2D[j][i];
				}
			}

		}

		template<class T>
		void fromArray(std::vector<T> array1, T inSize)
		{
			if (alreadyWas != true)
			{
				rows = inSize;
				cols = 1;
				MatrixArray2D = new MatrixType* [rows];

				// for loop nacte hodnoty na zacatku na hodnotu 0 
				for (auto i = 0; i < rows; ++i) {
					MatrixArray2D[i] = new MatrixType[cols];
				}

				for (auto i = 0; i < rows; ++i) {
					for (auto j = 0; j < cols; ++j) {

						MatrixArray2D[i][j] = 0;

						if (i == rows - 1 && j == cols - 1)
							alreadyWas = true;

					}
				}

			}

			for (auto i = 0; i < inSize; ++i)
			{
				MatrixArray2D[i][0] = array1[i];
			}
		
		}


		std::vector<MatrixType> toArray()
		{

			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					arr.push_back(MatrixArray2D[i][j]);
				}
			}
			return arr;

		}


		template<class T >
		void map(T activationFunction)
		{

			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					MatrixType val = MatrixArray2D[i][j];
					MatrixArray2D[i][j] = activationFunction(val);
				}
			}
		}

		template<class T>
		void map(Matrix mat1, T activationFunction)
		{
			if (alreadyWas != true)
			{
				rows = mat1.rows;
				cols = mat1.cols;

				MatrixArray2D = new MatrixType* [rows];

				// for loop nacte hodnoty na zacatku na hodnotu 0 
				for (auto i = 0; i < rows; ++i) {
					MatrixArray2D[i] = new MatrixType[cols];
				}

				for (auto i = 0; i < rows; ++i) {
					for (auto j = 0; j < cols; ++j) {

						if (i == rows - 1 && j == cols - 1) {
							alreadyWas = true;
						}
					}
				}

			}

			for (auto i = 0; i < rows; ++i) {
				for (auto j = 0; j < cols; ++j) {
					MatrixType val = mat1.MatrixArray2D[i][j];
					MatrixArray2D[i][j] = activationFunction(val);
				}
			}
		
		}


		template<class T>
		void subtract(T firstMatrix, T secondMatrix)
		{
			if (firstMatrix.rows == secondMatrix.rows && firstMatrix.cols == secondMatrix.cols) {
				rows = firstMatrix.rows;
				cols = firstMatrix.cols;

				if (alreadyWas != true)
				{



					MatrixArray2D = new MatrixType* [rows];

					// for loop nacte hodnoty na zacatku na hodnotu 0 
					for (auto i = 0; i < rows; ++i) {
						MatrixArray2D[i] = new MatrixType[cols];
					}

					for (auto i = 0; i < rows; ++i) {
						for (auto j = 0; j < cols; ++j) {

							MatrixArray2D[i][j] = 0;

							if (i == rows - 1 && j == cols - 1)
								alreadyWas = true;
						}
					}

				}
				// tato funkce odecte cele dve matice pomoci jednoho for loopu

					// podminky ktere rozhoduji jestli jsou radky stejne
					// protoze pri scitani dvou matic musi byt obe stejne velke
				for (auto i = 0; i < firstMatrix.rows; ++i) {
					for (auto j = 0; j < secondMatrix.cols; ++j) {
						MatrixArray2D[i][j] = firstMatrix.MatrixArray2D[i][j] - secondMatrix.MatrixArray2D[i][j];
					}
				}
			}

			else {
				std::cout << "neni stejny pocet radku a sloupcu obu matic!!!" << std::endl;
			}

		}
	};
}