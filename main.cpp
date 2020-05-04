#include <iostream>
#include <cstring>
#include "Matrix.h"
#include "LR.h"

int main (int argc,char *argv[]) {
	
	std::cout.unsetf ( std::ios::floatfield );
	std::cout.precision(20);
	if(argc>2){
		Matrix X=Matrix(0,0);
		
		Matrix Y=Matrix(0,0);
		
		LR model=LR(X,Y,1,1);
		char* test;
		switch(atoi(argv[1])){
			case 0:
				test=LR::buildPath(argv[3],".X");
				X = Matrix(test);
				Y = Matrix(LR::buildPath(argv[3],".Y"));
				model= LR(X,Y,0.001,atoi(argv[4]));
				model.standardize();
				model.train();
				model.save(argv[3]);
				break;
			
			case 1:
				model= LR(LR::buildPath(argv[2],".coef"),LR::buildPath(argv[2],".meanX"),LR::buildPath(argv[2],".meanY"),LR::buildPath(argv[2],".stdX"),LR::buildPath(argv[2],".stdY"));
				X = Matrix(LR::buildPath(argv[2],".query"));
				//TODO TROUVER LE CHANNEL DE COMMUNICATION RETOUR
				std::cout <<model.predict(X).toString();
				break;
			default:
				break;
		}
	}
	return 0;
}