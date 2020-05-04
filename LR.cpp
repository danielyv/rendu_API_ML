//
// Created by Yves DANIEL on 28/04/2020.
//


#include <iostream>
#include <cstring>
#include "LR.h"

void LR::train(){
	Matrix x=LR::adaptX(this->X);
	for(int i=0;i<iteration;i++){
		
		//E=X*Coeff-Y
		Matrix err=x*this->coef-this->Y;
		
		//D=(T(E)*X)*(1/m)
		Matrix derivative=((++err)*x*(1.0/x.getH()));
		
		//Coeff=Coeff-alpha*T(D)
		this->coef=this->coef-(++derivative*this->learningRate);
	}
}

Matrix LR::predict(Matrix parameters){
	if(this->standard){
		parameters.standardize(this->meanX,this->stdX);
		Matrix p=adaptX(parameters);
		Matrix result=p*this->coef;
		result.unstandardize(meanY,stdY);
		return result;
	}else{
		return adaptX(parameters)*this->coef;
	}
	
}

Matrix LR::adaptX(Matrix x){
	
	Matrix tmp=x.expend(x.getH(),x.getW()+1);
	
	for(int i=0;i<tmp.getH();i++){
		tmp.set(i,x.getW(),1);
	}
	
	return tmp;
}

LR::LR(Matrix x,Matrix y,double LearningRate, int nbIteration):Model(x,y),coef(Matrix(x.getW()+1,1)),learningRate(LearningRate),
                                              meanX(Matrix(1,x.getW())),stdX(Matrix(1,x.getW())),
                                              meanY(Matrix(1,Y.getW())),stdY(Matrix(1,Y.getW())),iteration(nbIteration){}

void LR::standardize(){
	
	this->standard=1;
	this->meanX=this->X.mean();
	
	this->meanY=this->Y.mean();
	
	this->stdX=this->X.std(this->meanX);
	
	this->stdY=this->Y.std(this->meanY);
	
	X.standardize(meanX,stdX);
	
	Y.standardize(meanY,stdY);
	
	
}

double LR::rsquare(){
	long double std=this->stdY.get(0,0);
	long double ssTot=std*std*this->Y.getH();
	Matrix mSsRes=this->Y-(adaptX(this->X)*this->coef);
	mSsRes.squareItUp();
	long double ssRes=mSsRes.sum().get(0,0);
	return 1-ssRes/ssTot;
}

void LR::save(char*name){
	
	this->coef.save(buildPath(name,this->extension[0]));
	this->meanX.save(buildPath(name,this->extension[1]));
	this->meanY.save(buildPath(name,this->extension[2]));
	this->stdX.save(buildPath(name,this->extension[3]));
	this->stdY.save(buildPath(name,this->extension[4]));
	
}

LR::LR(char*coef,char*meanX,char*meanY,char*stdX,char*stdY):Model(Matrix(0,0),Matrix(0,0)),coef(Matrix(coef)),meanX(Matrix(meanX)),
                                                            meanY(Matrix(meanY)),stdX(Matrix(stdX)),stdY(Matrix(stdY)),
                                                            standard(1){
	
}

char*LR::buildPath(char* modelName,char* extension){
	char* path=(char*)malloc(sizeof(char)*(strlen(modelName)+strlen(extension)));
	strcpy(path,modelName);
	strcat(path,extension);
	return path;
}
LR::LR(char* modelName):Model(Matrix(0,0),Matrix(0,0)){
	char *paths[5];
	
	for(int i=0;i<5;i++){
		paths[i]=buildPath(modelName,this->extension[i]);
	}
	
	new (this)LR(paths[0],paths[1],paths[2],paths[3],paths[4]);
	
	for(int i=0;i<5;i++){
		free(paths[i]);
	}
	
	
}

LR::LR():Model(Matrix(),Matrix()){

}
