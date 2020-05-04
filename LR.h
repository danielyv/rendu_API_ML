//
// Created by Yves DANIEL on 28/04/2020.
//

#ifndef APIMLPROJECT_LR_H
#define APIMLPROJECT_LR_H


#include "Model.h"

class LR:public Model{
private:
	enum { arrsize = 5 };
	
	char *extension[arrsize]={".coef",".meanX",".meanY",".stdX",".stdY"};
	Matrix coef=NULL;
	Matrix stdX=NULL;
	Matrix meanX=NULL;
	Matrix stdY=NULL;
	Matrix meanY=NULL;
	bool standard=0;
	double learningRate=1;
	int iteration;
public:
	static char* buildPath(char* modelName,char* extension);
	
	void train() override;
	static Matrix  adaptX(Matrix x);
	double rsquare();
	Matrix predict(Matrix parameters) override;
	void standardize();
	void save(char* name);
	LR(Matrix x,Matrix y,double LearningRate,int nbIteration);
	LR(char* coef,char* meanX,char* meanY,char* stdX,char* stdY);
	LR(char* modelName);
	LR();
};


#endif //APIMLPROJECT_LR_H
