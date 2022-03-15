/**************************************************/
/*                                                */
/*  Lab 1 Starter file                            */
/*                                                */
/*  Name: Junhee Kim                              */
/*  Student number: 159777184                     */
/*                                                */
/**************************************************/

/* remember to code these in an iterative only manner*/


unsigned int factorial (unsigned int n){
    unsigned int output = 1;         
    for (int i = 2; i <= n; i++){    
	output = output*i;	     
    }
    return output;		     
}

double power (double base, unsigned int n){
    unsigned int output = 1;	     	
    for (int i = 1; i <= n; i++){    		
	output = output * base;	     	
    }
    return output;		     	
}

unsigned int fibonacci (unsigned int n){
    unsigned int fb_p = 0;	     
    unsigned int fb_n = 1;	     
    unsigned int fb = 0;	     
    for (int i = 0; i < n; i++){     
	fb = fb_p + fb_n;	     
	fb_p = fb_n;		     
	fb_n = fb;		     
    }
    return fb_p;		     
}

