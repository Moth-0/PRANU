#include<iostream>

struct data { int a,b; double sum;};
void harm(data& arg){
	int a=arg.a,b=arg.b; // you better work
	double sum=0;        // with thread-local variables
	for(int i=a;i<b;i++)sum+=1.0/i;
	arg.sum=sum; // only write once into memory
	}

int main(int argc, char** argv) {
    
return 0;}