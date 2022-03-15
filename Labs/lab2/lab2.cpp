/**************************************************/
/*                                                */
/*  Lab 2 Starter file                            */
/*                                                */
/*  Name: Junhee Kim                              */
/*  Student number: 159777184                     */
/*                                                */
/**************************************************/

/* remember to code these in an iterative only manner*/

unsigned int factorial (unsigned int n){
    int output = 1;						
    if (n > 1){					 
	output = n * factorial(n - 1);
    }
    return output;
}

double power (double base, unsigned int n){
    int output = 1;
    if (n > 0){
        output = base * power(base, n - 1);
    }
    return output;
}

unsigned int fibonacci (unsigned int n){
    int output;
    if ( n <= 1 ) { output = n; }
    else{
	output = fibonacci(n - 1) + fibonacci(n - 2);
    }
    return output;	
}
