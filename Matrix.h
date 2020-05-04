//
// Created by Yves DANIEL on 28/04/2020.
//

#ifndef APIMLPROJECT_MATRIX_H
#define APIMLPROJECT_MATRIX_H

#include <string>

enum MATRIXTYPE:int{
	NEUTRAL=0
};

class Matrix{
public:
	
	Matrix(int h,int w,long double**matrix);
	
	Matrix(const char*path);
	
	Matrix(int h,int w,int type);
	
	Matrix(int h,int w);
	
	Matrix(Matrix const&m);
	
	Matrix();
	Matrix operator+(Matrix const&a);
	
	Matrix operator-(Matrix const&a);
	
	Matrix operator-();
	
	Matrix operator*(Matrix const&a);
	
	Matrix operator%(Matrix const&a);
	
	Matrix operator%(long double q);
	
	
	Matrix operator*(long double q);
	
	bool operator==(Matrix const&a);
	
	Matrix operator!();
	
	Matrix operator--();
	
	Matrix operator++();
	Matrix mean();
	Matrix std(Matrix mean);
	Matrix sum();
	void standardize(Matrix mean, Matrix std);
	void unstandardize(Matrix mean,Matrix std);
	std::string toString();
	
	long double determinant();
	int destroy();
	
	Matrix applyFunc(long double (*f)(long double));
	
	int getW();
	
	int getH();

	Matrix expend(int h,int w);
	void set(int i,int j,long double value);
	long double get(int i,int j);
	Matrix getLine(int i);
	void squareItUp();
	void save(char* name);
	
private:
	long double**matrix;
	int h=0;
	int w=0;
	
	long double abs(long double val);
	
	void swapLines(int l,int L);
	
	void correct();
	static char**split(char*str,int nbWord,char c);
	static int occ(char c,char*str);
	long double EPSILON=0.00000000000001;
	
};

#endif //APIMLPROJECT_MATRIX_H
