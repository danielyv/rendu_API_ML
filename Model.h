//
// Created by Yves DANIEL on 28/04/2020.
//

#ifndef APIMLPROJECT_MODEL_H
#define APIMLPROJECT_MODEL_H

#include "Matrix.h"

class Model{
protected:
	Matrix X=Matrix();
	Matrix Y=Matrix();

public:
	virtual void train() = 0;
	
	virtual Matrix predict(Matrix parameters) = 0;
	
	Model(Matrix x,Matrix y):X(x),Y(y){}
};


#endif //APIMLPROJECT_MODEL_H
