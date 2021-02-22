#include <iostream>
#include <fstream> //need to both read and write from/to files.
#include <string>//need to use string to get file names.
#include <stdlib.h> //need to use abs() when calculating condition number and ordering pivots.
#include <vector> 
#include <cmath>
using namespace std;

vector<double> matrix_multipler(vector< vector<double> > A, vector<double> x){// This function multiplies a matrix and a vector. Ax=b, returns b vector.
	
	vector<double> b;//Solution matrix is created
	for(int i=0; i<A.size(); i++)
		b.push_back(0);
	
	for(int i=0; i<A.size(); i++){//Matrix multiplication is done here
		for(int j=0; j<A.size(); j++){
			b[i] += A[i][j]*x[j];
		}
	}
	return b;}

double getNorm( vector<double> x ){//This function gets infinity norm of a vector.
	
	double max=abs(x[0]);
	for(int i=0; i<x.size(); i++){
		if(max<abs(x[i]))
			max = abs(x[i]);}
	
	return max;}

vector<double> normalize(vector<double> x, double norm){//This function just normalize vector by dividing it by the norm of the vector
	for(int i=0; i<x.size(); i++)
		x[i] /= norm;
	return x;}

double signArrange(vector< vector<double> >A,vector<double> eigenvector, double eigenvalue){//This function determines the sign of the eigenvalue

	//The method in this function is: if Av=lv (v is eigenvector l is eigenvalue), Av-lv=0
	//First eigenvalue is assumed to be positive and Av-lv is done
	//Then eigenvalue is assumed to be negative and Av-lv is done
	//Av-lv must be 0 for true eigenvalue, but due to machine presicion the value may not be exactly 0
	//Therefore both vectors are compared by their infinity norms
	//Vector which is closest to zero should be eigenvector. We can decide the eigenvalue's sign by this method.	
	vector<double> positive;
	vector<double> negative;
	positive = matrix_multipler(A,eigenvector);//In this loop, eigenvalue is assumed to be positive
	for(int i=0; i<eigenvector.size(); i++)
		positive[i] -= eigenvalue*eigenvector[i];//and Av-lv is done
	
	negative = matrix_multipler(A, eigenvector);//In this loop, eigenvalue is assumed to be negative
	for(int i=0; i<eigenvector.size(); i++)
		negative[i] += eigenvalue*eigenvector[i];//and Av-lv is done
	
	
	if(getNorm(negative) < getNorm(positive))// vectors are compared by their infinity norms
		eigenvalue = -eigenvalue;//Sign is decided
	
	return eigenvalue;//eigenvalue with the correct sign is returned	
}
vector<double> findEigen(vector< vector<double> > A, double tolerance, double *eigenvalue){//This function returns eigenvector while seting the eigenvalue

	vector<double> X;//This is the random vector which is assumed close to eigenvector
	X.push_back(1.00);
	for(int i=1; i<A.size(); i++)
	X.push_back(0);
	  
	double diff = tolerance + 1; // to initiate next loop, diff will be 0 just after while loop is initiated.
	*eigenvalue = 0;

	while(tolerance < abs(diff)){//This loop sets eigenvector and eigenvalue
		diff = - *eigenvalue;//this holds the old value of eigen value which will be added to new eigenvalue.(For the first loop, diff is set to 0)
		X = matrix_multipler(A,X);// This function multiplies a matrix and a vector. Ax=b, returns b vector.
		*eigenvalue = getNorm(X);//This function gets infinity norm of a vector.
		diff += *eigenvalue; //NewEiegenValue - OldEigenvalue. This provides subtraction of new value of eigenvalue1 and old value of eigenvalue1
		X = normalize(X,*eigenvalue);}//This function just normalize vector by dividing it by the norm of the vector
		
	*eigenvalue = signArrange(A,X,*eigenvalue);//This function determines the sign of the eigenvalue
 	return X;
}

vector< vector<double> > deflation(vector< vector<double> >A, vector<double> x){//This function deflates A matrix to B matrix by deflating first eigenvalue and eigenvector
	int p=0; // refers position
	while(x[p]==0)//Chooses an eigenvalue element which is not equals 0
		p++;
		
	vector<double> row_of_A;//Gets the related row of A
	for(int k=0; k<x.size(); k++)
		row_of_A.push_back(A[p][k]);

	for(int i=0; i<A.size(); i++)//Creates B matrix by substracting the related matrix
		for(int j=0; j<A.size(); j++)
			A[i][j] -= row_of_A[j]*x[i]/x[p];
			
	return A;
}
int main(int argc, char *argv[]){
			
	vector<double> X;		// This vector will be used to get A matrix as a vector
			
	ifstream AFile( argv[1], ios::in );
	
		if(AFile.is_open())// need to check file is open or not.
		{	
			while(AFile.good()) 
			{
  			double temp;
 	 		AFile >> temp;
 			X.push_back(temp);
			}
		AFile.close();}	
		
    	else{
    	cout<<"A file can not open, please try again"<<endl;
		exit(0);}
		
    int n = sqrt(X.size());	
	if(n*n != X.size()){
		cout<<"Matrix is not a square matrix!";
		exit(0);}
		
	vector< vector<double> > A( n , vector<double> (n)); // nxn A matrix is declared
	
	for(int i=0;i<n;i++)// This loop sets nxn 2D A matrix from 1D vector.
		for(int j=0; j<n; j++)
			A[i][j]=X[i*n + j];
		
	X.clear();//Now X vector will be a vector to store eigenvalue 
	


	string s = argv[2];
	double tolerance = atof(s.c_str());//tolerance is changed to double from string
	double eigenvalue1;
	double eigenvalue2;

	X = findEigen(A,tolerance,&eigenvalue1);//This function returns eigenvector while seting eigenvalue.
	A = deflation(A,X); // A become the deflated B matrix 
	findEigen(A,tolerance,&eigenvalue2);// Although this function returns eigen vector, we do not care aboout eigen vector. We use only this function to set eigenvalue#2.


	ofstream solutionFile;//This part creates output file with the name argv[3]
	solutionFile.open(argv[3]);
	solutionFile<<"Eigenvalue#1: "<<eigenvalue1<<"\r\n";
    for (int i=0; i<n; i++)
	     solutionFile<< X[i] <<"\r\n";
	solutionFile<<"Eigenvalue#2: "<<eigenvalue2<<"\r\n";  
	solutionFile.close();
	

return 0;}
