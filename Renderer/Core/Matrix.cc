using namespace std;
#include "Matrix.hh"
#include <iostream>

namespace Marcel{

	Matrix::Matrix()
	{
		isize = jsize = 0;
		table = NULL;
	}

	Matrix::Matrix(int i, int j)
	{
		isize = i;
		jsize = j;

		table = new double*[isize];
		for (int k = 0; k < isize; k++)
			table[k] = new double[jsize];

		for (int i = 0; i < isize; i++)
			for (int j = 0; j < jsize; j++)
				table[i][j] = 0;
	}

	Matrix::~Matrix()
	{
		for (int i = 0; i < isize; i++)
			if (table[i] != NULL)
				delete[] table[i];

		if (table != NULL)
			delete[] table;
		table = NULL;
	}

	Matrix::Matrix(const Matrix& m)
	{

		isize = m.isize;
		jsize = m.jsize;

		table = new double*[isize];
		for (int k = 0; k < isize; k++)
			table[k] = new double[jsize];

		for (int i = 0; i < isize; i++)
			for (int j = 0; j < jsize; j++)
				table[i][j] = m.table[i][j];
	}

	Matrix Matrix::operator+(Matrix m)
	{
		if (m.isize != isize && m.jsize != jsize)
			throw 0;

		Matrix r(isize, jsize);
		for (int i = 0; i < isize; i++)
			for (int j = 0; j < jsize; j++)
				r.table[i][j] = m.table[i][j] + table[i][j];

		return r;
	}

	Matrix Matrix::operator-(Matrix m)
	{
		if (m.isize != isize && m.jsize != jsize)
			throw 0;

		Matrix r(isize, jsize);
		for (int i = 0; i < isize; i++)
			for (int j = 0; j < jsize; j++)
				r.table[i][j] = table[i][j] - m.table[i][j];

		return r;
	}


	Matrix Matrix::operator*(Matrix m)
	{
		if (jsize != m.isize)
			throw 0;

		Matrix r(isize, m.jsize);
		for (int i = 0; i < isize; i++)
			for (int j = 0; j < jsize; j++)
				for (int k = 0; k < m.jsize; k++)
					r.table[i][k] += table[i][j] * m.table[j][k];
		return r;
	}

	Matrix Matrix::operator*(double d)
	{
		Matrix r(isize, jsize);
		for (int i = 0; i < isize; i++)
			for (int j = 0; j < jsize; j++)
				r.table[i][j] = table[i][j] * d;

		return r;
	}

	Vector Matrix::operator*(Vector V)
	{
		if (jsize != 4)
			throw 0;

		Matrix r(4, 1);
		r.setValue(0, 0, V.x);
		r.setValue(1, 0, V.y);
		r.setValue(2, 0, V.z);
		r.setValue(3, 0, 1);

		Matrix r2(4, 1);
		r2 = operator*(r);

		Vector R;
		R.x = r2.table[0][0];
		R.y = r2.table[1][0];
		R.z = r2.table[2][0];

		return R;
	}

	Point Matrix::operator*(Point P)
	{
		if (jsize != 4)
			throw 0;

		Matrix r(4, 1);
		r.setValue(0, 0, P.x);
		r.setValue(1, 0, P.y);
		r.setValue(2, 0, P.z);
		r.setValue(3, 0, 1);

		Matrix r2(4, 1);
		r2 = operator*(r);

		Point R;
		R.x = r2.table[0][0];
		R.y = r2.table[1][0];
		R.z = r2.table[2][0];

		return R;
	}

	int Matrix::operator=(Matrix m)
	{
		isize = m.isize;
		jsize = m.jsize;

		if (table != NULL)
		{
			for (int i = 0; i < isize; i++)
				delete[] table[i];
			delete[] table;
		}

		table = new double*[isize];
		for (int k = 0; k < isize; k++)
			table[k] = new double[jsize];

		for (int i = 0; i < isize; i++)
			for (int j = 0; j < jsize; j++)
				table[i][j] = m.table[i][j];

		return 0;
	}

	double Matrix::getValue(int i, int j)
	{
		if (i < 0 || i > isize || j < 0 || j > jsize)
			throw 0;

		return table[i][j];
	}

	int Matrix::getIsize(){
		return isize;
	}
	int Matrix::getJsize(){
		return jsize;
	}

	void Matrix::setValue(int i, int j, double v)
	{
		if (i < 0 || i > isize || j < 0 || j > jsize)
			throw 0;

		table[i][j] = v;
	}

	std::ostream& operator<<(std::ostream& o, Matrix m)
	{
		for (int i = 0; i < m.getIsize(); i++) {
			for (int j = 0; j < m.getJsize(); j++)
				o << m.getValue(i, j) << " ";
			o << endl;
		}

		return o;
	}

	void Matrix::Identity()
	{
		for (int i = 0; i < isize; i++)
			for (int j = 0; j < jsize; j++)
				table[i][j] = (i == j);
	}
}