//
// Created by Yves DANIEL on 28/04/2020.
//

#include <assert.h>
#include <sstream>
#include <fcntl.h>
#include <zconf.h>
#include "Matrix.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <cstring>


Matrix::Matrix(int h,int w,long double**matrix){
	this->h=h;
	this->w=w;
	this->matrix=(long double**)malloc(sizeof(long double*)*h);
	for(int i=0;i<h;i++){
		this->matrix[i]=(long double*)malloc(sizeof(long double)*w);
		for(int e=0;e<w;e++){
			this->matrix[i][e]=matrix[i][e];
		}
	}
}

Matrix::Matrix(int h,int w,int type){
	this->h=h;
	this->w=w;
	this->matrix=(long double**)malloc(sizeof(long double*)*h);
	for(int i=0;i<h;i++){
		this->matrix[i]=(long double*)malloc(sizeof(long double)*w);
		for(int e(0);e<w;e++){
			this->matrix[i][e]=0.0;
		}
	}
	switch(type){
		case MATRIXTYPE::NEUTRAL :
			assert(w==h);
			for(int i(0);i<this->h;i++){
				this->matrix[i][i]=1;
			}
			break;
		default:
			assert(0);
			break;
	}
}


Matrix::Matrix(int h,int w){
	this->h=h;
	this->w=w;
	this->matrix=(long double**)malloc(sizeof(long double*)*h);
	for(int i(0);i<h;i++){
		this->matrix[i]=(long double*)malloc(sizeof(long double)*w);
		for(int e(0);e<w;e++){
			
			this->matrix[i][e]=0.0;
		}
	}
}

Matrix::Matrix(Matrix const&m){
	this->h=m.h;
	this->w=m.w;
	this->matrix=(long double**)malloc(sizeof(long double*)*this->h);
	for(int i=0;i<this->h;i++){
		this->matrix[i]=(long double*)malloc(sizeof(long double)*this->w);
		for(int e=0;e<this->w;e++){
			this->matrix[i][e]=m.matrix[i][e];
		}
	}
}


int Matrix::getW(){
	return this->w;
}

int Matrix::getH(){
	return this->h;
}

Matrix Matrix::operator+(Matrix const&a){
	assert(a.w==this->w&&a.h==this->h);
	Matrix matrix(this->h,this->w,this->matrix);
	for(int i(0);i<this->h;i++){
		for(int e(0);e<this->w;e++){
			matrix.matrix[i][e]+=this->matrix[i][e];
		}
	}
	
	
	return matrix;
}

Matrix Matrix::operator-(){
	Matrix matrix(this->h,this->w,this->matrix);
	for(int i(0);i<this->h;i++){
		for(int e(0);e<this->w;e++){
			matrix.matrix[i][e]=-matrix.matrix[i][e];
		}
	}
	
	return matrix;
}

Matrix Matrix::operator*(long double q){
	
	Matrix matrix(this->h,this->w,this->matrix);
	for(int i=0;i<matrix.getH();i++){
		for(int j=0;j<matrix.getW();j++){
			matrix.matrix[i][j]=matrix.matrix[i][j]*q;
		}
	}
	
	return matrix;
}

Matrix Matrix::operator-(Matrix const&a){
	
	assert(a.w==this->w&&a.h==this->h);
	Matrix matrix(this->h,this->w,this->matrix);
	for(int i(0);i<this->h;i++){
		for(int e(0);e<this->w;e++){
			matrix.matrix[i][e]-=a.matrix[i][e];
		}
	}
	
	return matrix;
}

Matrix Matrix::operator*(Matrix const&a){
	assert(a.h==this->w);
	
	Matrix matrix(this->h,a.w);
	
	for(int i(0);i<this->h;i++){
		for(int e(0);e<a.w;e++){
			for(int k(0);k<this->w;k++){
				matrix.matrix[i][e]+=this->matrix[i][k]*a.matrix[k][e];
				
			}
		}
	}
	
	return matrix;
}

//Vérification d'égalités
bool Matrix::operator==(Matrix const&a){
	bool equals(a.w==this->w&&a.h==this->h);
	for(int i(0);i<this->h;i++){
		for(int e(0);e<this->w;e++){
			equals&=abs(a.matrix[i][e]-this->matrix[i][e])<EPSILON;
		}
	}
	return equals;
}

//Inverser matrice #Pivot2GAUSSEUH
Matrix Matrix::operator!(){
	assert(abs(this->determinant())-0.0>=this->EPSILON);
	Matrix matrix=Matrix(this->h,this->w,this->matrix);
	Matrix net=Matrix(this->h,this->w,MATRIXTYPE::NEUTRAL);
	
	for(int j=0,r=-1;j<matrix.w;++j){
		
		int k=-1;
		long double val=0.0;
		for(int i(r+1);i<matrix.h;++i){
			if(k==-1||abs(matrix.matrix[i][j])>abs(val)){
				k=i;
				val=matrix.matrix[i][j];
			}
		}
		if(val!=0.0){
			r++;
			
			for(int e(0);e<matrix.w;e++){
				matrix.matrix[k][e]=matrix.matrix[k][e]/val;
				net.matrix[k][e]=net.matrix[k][e]/val;
			}
			
			matrix.swapLines(k,r);
			net.swapLines(k,r);
			
			for(int i(0);i<matrix.h;++i){
				long double coeff=matrix.matrix[i][j];
				
				if(i!=r){
					for(int e(0);e<this->w;e++){
						matrix.matrix[i][e]-=matrix.matrix[r][e]*coeff;
						net.matrix[i][e]-=net.matrix[r][e]*coeff;
					}
				}
			}
			
			
		}
		
	}
	
	return net;
}

//TOSTRING.
std::string Matrix::toString(){
	std::stringstream str;
	for(int i(0);i<this->h;i++){
		for(int e(0);e<this->w;e++){
			str<<this->matrix[i][e]<<";";
		}
		str.seekp(-1, std::ios_base::end);
		str<<"\n";
	}
	str.seekp(-1, std::ios_base::end);
	std::string string=str.str();
	if (!string.empty()) {
		string.pop_back();
	}	return string;
}

int Matrix::occ(char c,char*str){
	int count=0;
	for(int i=0;i<strlen(str);i++){
		count+=str[i]==c;
	}
	return count;
}

char**Matrix::split(char*str,int nbWord,char c){
	char**split=(char**)malloc(sizeof(char*)*nbWord);
	char toAdd=1;
	for(int i=0,count=0;str[i]!='\0';i++){
		if(toAdd){
			toAdd=0;
			split[count++]=str+i;
		}
		if(str[i]==c){
			str[i]='\0';
			toAdd=1;
		}
	}
	return split;
}
char *readcontent(const char *filename)
{
	char *fcontent = NULL;
	int fsize = 0;
	FILE *fp;
	
	fp = fopen(filename, "r");
	if(fp) {
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);
		fsize--;
		rewind(fp);
		fcontent = (char*) malloc(sizeof(char) * fsize);
		fread(fcontent, 1, fsize, fp);
		
		fclose(fp);
	}
	return fcontent;
}
//TODO Terminer corriger la taille
Matrix::Matrix(const char*path){
	//Lecture du fichier
	char*file=readcontent(path);
	//Nombre de lignes
	this->h=occ('\n',file)+1;
	
	//Split des lignes
	char**spLine=split(file,this->h,'\n');
	
	//Allocation mémoire de la matrice height
	this->matrix=(long double**)malloc(sizeof(long double*)*this->h);
	
	for(int i=0;i<this->h;i++){
		//Set le width de la matrice
		if(this->w==0){
			this->w=occ(';',spLine[i])+1;
		}
		//Split de la ligne en élément
		char**spEl=split(spLine[i],this->w,';');
		//Allocation mémoire de la matrice width
		this->matrix[i]=(long double*)malloc(sizeof(long double)*this->w);
		
		//Conversion de char* en long double
		for(int j=0;j<this->w;j++){
			this->matrix[i][j]=strtod(spEl[j],NULL);
		}
	}
	free(file);
}

//DESTRUCTEUR DE MATRICE !PIOU PIOU PIOU
int Matrix::destroy(){
	for(int i(0);i<this->h;i++){
		free(matrix[i]);
	}
	free(matrix);
	return 1;
}

//Matrice transposée, "You spin me right now"~~
Matrix Matrix::operator++(){
	Matrix m = Matrix(this->w, this->h);
	for (int i(0); i < this->h; i++) {
		for (int e(0); e < this->w; e++) {
			m.matrix[e][i] = this->matrix[i][e];
		}
	}
	m.correct();
	return m;
}


//Pseudoinverse, "Coucou je m'appelle Inverse"
Matrix Matrix::operator--(){
	Matrix matrix=Matrix(this->h,this->w,this->matrix);
	Matrix tr=++matrix;
	Matrix trix=tr*matrix;
	Matrix part1=!(trix*trix);
	Matrix part2=trix*tr;
	Matrix result=part1*part2;
	return result;
}

//Valeur absolue, sert pour le pivot de gausse, pff, un truc de gausse.
long double Matrix::abs(long double val){
	if(val<0){
		return -val;
	}
	return val;
}


//Echanges 2 lignes, sert pour le pivot de gausse qui va pivoter tes morts
void Matrix::swapLines(int l,int L){
	long double val;
	for(int i=0;i<this->w;i++){
		val=this->matrix[l][i];
		matrix[l][i]=matrix[L][i];
		matrix[L][i]=val;
	}
}

long double Matrix::determinant(){
	assert(this->w==this->h);
	long double result=0.0;
	for(int i(0);i<this->w;i++){
		long double value=1;
		for(int e(0);e<this->h;e++){
			value*=this->matrix[e][(i+e)%this->w];
		}
		result+=value;
	}
	for(int i(0);i<this->w;i++){
		long double value=1;
		for(int e(0);e<this->h;e++){
			int ind=(i-e)%this->w;
			if(ind<0){
				ind=this->w-ind;
			}
			value*=this->matrix[e][ind];
		}
		result-=value;
	}
	return result;
}

void Matrix::correct(){
	for(int i(0);i<this->h;i++){
		for(int e(0);e<this->w;e++){
			
			if(abs(this->matrix[i][e])<this->EPSILON){
				this->matrix[i][e]=0.0;
			}
		}
	}
}

Matrix Matrix::applyFunc(long double (*f)(long double)){
	Matrix m=Matrix(this->h,this->w,this->matrix);
	for(int i(0);i<m.h;i++){
		for(int e(0);e<m.w;e++){
			m.matrix[i][e]=f(m.matrix[i][e]);
		}
	}
	return m;
}

Matrix Matrix::operator%(Matrix const&a){
	assert(a.h==this->h);
	Matrix mat=Matrix(a.h,a.w+this->w);
	for(int i(0);i<mat.h;i++){
		for(int e(0);e<mat.w;e++){
			if(e<this->w){
				mat.matrix[i][e]=this->matrix[i][e];
			}else{
				mat.matrix[i][e]=a.matrix[i][e];
			}
		}
	}
	return mat;
}

Matrix Matrix::operator%(long double q){
	Matrix mat=Matrix(this->h,1+this->w);
	for(int i(0);i<mat.h;i++){
		for(int e(0);e<mat.w;e++){
			if(e<this->w){
				mat.matrix[i][e]=this->matrix[i][e];
			}else{
				mat.matrix[i][e]=q;
			}
		}
	}
	return mat;
}

Matrix Matrix::expend(int h,int w){
	assert(h>=this->getH()&&w>=this->getW());
	Matrix mat=Matrix(h,w);
	
	for(int i=0;i<this->getH();i++){
		for(int j=0;j<this->getW();j++){
			mat.matrix[i][j]=this->matrix[i][j];
		}
	}
	return mat;
}

void Matrix::set(int i,int j,long double value){
	this->matrix[i][j]=value;
}

long double Matrix::get(int i,int j){
	return this->matrix[i][j];
}

Matrix Matrix::getLine(int i){
	
	return Matrix(1,this->getW(),&(this->matrix[i]));
}

void Matrix::squareItUp(){
	for(int i=0;i<this->getH();i++){
		for(int j=0;j<this->getW();j++){
			this->matrix[i][j]*=this->matrix[i][j];
		}
	}
}

void Matrix::save(char* name){
	std::ofstream outfile;
	outfile.open(name);
	outfile <<this->toString();
	outfile.close();
}

Matrix Matrix::mean(){
	Matrix mat=Matrix(1,w);
	for(int i=0;i<this->getW();i++){
		for(int j=0;j<this->getH();j++){
			mat.matrix[0][i]+=this->matrix[j][i];
		}
		mat.matrix[0][i]/=this->getH();
	}
	return mat;
}

Matrix Matrix::std(Matrix mean){
	Matrix mat=Matrix(1,w);
	for(int i=0;i<this->getW();i++){
		for(int j=0;j<this->getH();j++){
			long double x=this->matrix[j][i]-mean.matrix[0][i];
			mat.matrix[0][i]+=x*x;
		}
		mat.matrix[0][i]=sqrt(mat.matrix[0][i]/this->getH());
	}
	return mat;
}

void Matrix::standardize(Matrix mean,Matrix std){
	for(int i=0;i<this->getW();i++){
		for(int j=0;j<this->getH();j++){
			long double x=this->matrix[j][i]-mean.matrix[0][i];
			if(abs(std.matrix[0][i])>this->EPSILON){
				this->matrix[j][i]=x/std.matrix[0][i];
			}
			else{
				this->matrix[j][i]=x;
			}
		}
	}
}

void Matrix::unstandardize(Matrix mean,Matrix std){
	for(int i=0;i<this->getW();i++){
		for(int j=0;j<this->getH();j++){
			long double x=this->matrix[j][i]*std.matrix[0][i];
			this->matrix[j][i]=x+mean.matrix[0][i];
		}
	}
}

Matrix Matrix::sum(){
	Matrix mat=Matrix(1,w);
	for(int i=0;i<this->getW();i++){
		for(int j=0;j<this->getH();j++){
			mat.matrix[0][i]+=this->matrix[j][i];
		}
	}
	return mat;}

Matrix::Matrix(){

}
