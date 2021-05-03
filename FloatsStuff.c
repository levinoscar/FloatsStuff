#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>

#define NORM 0
#define DNORM 1
#define SPEC 2
#define BIAS 127


/*
    Declare a "typedef struct {} flt;" to contain data for a float
    The data should include:
        An integer for the sign
            (1 for positive, -1 for negative)
        An integer for the exponent value
            (should be bit value to integer minus BIAS or -126 for denormalized)
        A float to contain the value of the mantissa
            (Actual float value extracted from the binary value)
        An integer to contain the mode using the defines above
            (NORM, DNORM and SPEC)
    Example after processing: -15.375 = 1 10000010 11101100000000000000000
        sign = -1
        exp = 3
        man = 0.9218750000
        mode = NORM
*/

struct flt {

   int sign;
   int exponent;
   float mantissa;
   int mode;

};


/*
    Write a function get_flt_bits_int to return an integer with the
    bits copied from a float.
    Example:
        for f = -15.375,
        the bits of int n = 11000001011101100000000000000000
    Look at the slides and code from the float lectures and use the
    indirection trick.  This can easily be done in one line of code.
    The function should accept a float and return an int.
*/

int get_flt_bits_int(float f) {

    return *((int*) &f);

}


/*
    Write a function that returns the sign of a float as a char.
    You should call get_flt_bits_int to get the bits in an int
    and return '1' if the sign is negative else return '0'.  The
    function should accept a float and return a string.
*/

char* get_flt_sign_char(float f) {

   int n = get_flt_bits_int(f);

   if (((n >> 31) & 1) == 1) {

       return "1";

   } else {

       return "0";

   }

}


/*
    Write a function that returns the sign of a float as an integer.
    You should call get_flt_bits_int to get the bits in an int
    and return -1 if the sign is negative else return 1.  The function
    should accept a float and return an int.
*/

int get_flt_sign_val(float f) {

   int n = get_flt_bits_int(f);

   if (((n >> 31) & 1) == 1) {

       return -1;

   } else {

       return 1;

   }

}


/*
    Write a function to return a string containing the
    actual binary value of the exponent of a float in a
    char array.  You should call get_flt_bits_int to get
    the bits in an int and return the string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are "10000010"
    The function should accept a float and return a string.
*/

char* get_flt_exp_str(float f) {

   int n = get_flt_bits_int(f);

   int exp = (n >> 23) & 0xFF;

   char* expstr = (char*)malloc(sizeof(char) * 9);

   expstr[8] = '\0';

   for (int j = 7; j >= 0; j--) {

       if ((exp & 1) == 1) {

           expstr[j] = '1';

       } else {

           expstr[j] = '0';

       }

       exp >>= 1;

   }

   return expstr;

}


/*
    Write a function to return an integer containing the
    actual integer value of the exponent of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the exponent value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the actual value of the exponent is 3
    The function should accept a float and return an int.
*/

int get_flt_exp_val(float f) {

   int n = get_flt_bits_int(f);

   int exp = (n >> 23) & 0xFF;

   return exp - BIAS;

}




/*
    Write a function to return an integer containing the
    mode of the exponent of a float.  You should call
    get_flt_exp_val to get the bits in an int and return
    the int with the mode value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the mode is NORM
    The function should accept a float and return an int.
*/

int get_flt_exp_mode(float f) {

   int exp = get_flt_exp_val(f);

   if (exp == 0) {

       return DNORM;

   } else if (exp == 255 - BIAS) {

       return SPEC;

   } else {

       return NORM;

   }

}


/*
    Write a function to return a string containing the
    actual binary value of the mantissa of a float in a
    char array.  You should call get_flt_bits_int to get
    the bits in an int and return the string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are "11101100000000000000000"
    The function should accept a float and return a string.
*/

char* get_flt_man_str(float f) {

   int n = get_flt_bits_int(f);

   int manti = n & 0x07FFFFFF;

   char* mantistr = (char*)malloc(sizeof(char) * 24);

   mantistr[23] = '\0';

   for (int j = 22; j >= 0; j--) {

       if ((manti & 1) == 1) {

           mantistr[j] = '1';

       } else {

           mantistr[j] = '0';

       }

       manti >>= 1;

   }

   return mantistr;

}


/*
    Write a function to return a float containing the
    actual float value of the mantissa of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the mantissa value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are 11101100000000000000000
            the actual value of the mantissa is 0.9218750000
    The function should accept a float and return an int.
*/

float get_flt_man_val(float f) {

   int n = get_flt_bits_int(f);

   int manti = n & 0x07FFFFFF;

   manti = manti | 0x3F800000;

   return *((float*) & manti);

}


/*
    Write a function to return a string containing the
    actual binary value of a float in a char array.  You
    should call get_flt_sign_char, get_flt_exp_str and
    get_flt_man_str to get the bits in an char and two
    strings and return the concatenated string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            The sign is '1'
            the exponent is "10000010"
            and the mantissa bits are "11101100000000000000000"
            The string should be formatted as:
                "1 10000010 11101100000000000000000" to clearly
                separate the 3 parts.
    The function should accept a float and return a string.
*/

char* get_flt_bits_str(float f) {

   char* bitstr = (char*)malloc(sizeof(char) * 35);

   char* sign = get_flt_sign_char(f);

   bitstr[0] = sign[0];

   bitstr[1] = ' ';

   char* exp = get_flt_exp_str(f);

   for (int i = 2; i < 10; i++) {

       bitstr[i] = exp[i - 2];

   }

   bitstr[10] = ' ';

   char* manti = get_flt_man_str(f);

   for (int i = 0; i < 23; i++) {

       bitstr[i + 11] = manti[i];

   }

   bitstr[34] = '\0';

   return bitstr;

}


/*
    Write a function to separate the parts of a float
    into a flt struct as described above.  You should
    call get_flt_sign_val, get_flt_exp_mode,
    get_flt_exp_val and get_flt_man_val.
    Hint:  make sure to set exponent to -126 for
    DNORM mode.
*/

struct flt get_flt_val_flt(float f) {

   struct flt myfloat;

   myfloat.sign = get_flt_sign_val(f);

   myfloat.exponent = get_flt_exp_val(f);

   myfloat.mode = get_flt_exp_mode(f);

   myfloat.mantissa = get_flt_man_val(f);

   return myfloat;

}


/*
    Write a function to print a flt struct to screen.
    It should accept a flt struct and return nothing.
    Hint: Use if statement to print mode.
*/

void print_flt(struct flt myFloat) {

   printf("Printing Float structure:\n");

   printf("Sign = %d\n", myFloat.sign);

   printf("Exponent = %d\n", myFloat.exponent);

   printf("Mantissa = %f\n", myFloat.mantissa);

   printf("Mode = ");

   if (myFloat.mode == NORM) {

       printf("NORM\n");

   } else if (myFloat.mode == DNORM) {

       printf("DNORM\n");

   } else {

       printf("SPEC\n");

   }

}


/*
    Write a function to get the actual float value back
    out of a flt struct.
    Hints:
        The float value produced will depend on the mode.
        To set a float to infinity use the math library constant INFINITY
        To set a float to not-a-number use the math library constant NAN
        Check the slides and text for conditions for NORN, DNORM and SPEC
        You need to return (sign) * M * 2^e
*/

float get_flt_bits_val(struct flt myFloat) {

   if (myFloat.exponent == 0) {

       if (myFloat.mantissa != 0) {

           return NAN;

       } else {

           return INFINITY;

       }

   }

   return (myFloat.sign * myFloat.mantissa * pow(2, myFloat.exponent));

}




/*
    Write a main function that calls an prints results for
    each function when completed.
    get_flt_sign_char
    get_flt_sign_val

    get_flt_exp_str
    get_flt_exp_val

    get_flt_man_str
    get_flt_man_val

    get_flt_bits_str

    get_flt_val_flt
    print_flt

    get_flt_bits_val
*/

int main() {

    // create a float

    float f = -15.375;


  
    // print every results from all functions

    printf("Integer value: %d\n", get_flt_bits_int(f));

    printf("Sign char: %s\n", get_flt_sign_char(f));

    printf("Sign val: %d\n", get_flt_sign_val(f));

    printf("Exponent string: %s\n", get_flt_exp_str(f));

    printf("Exponent val: %d\n", get_flt_exp_val(f));

    printf("Mantissa string: %s\n", get_flt_man_str(f));

    printf("Mantissa val: %f\n", get_flt_man_val(f));

    printf("Sign, Exponent and Mantissa: %s\n", get_flt_bits_str(f));

    struct flt myFloat = get_flt_val_flt(f);

    print_flt(myFloat);

    printf("Float val: %f\n", get_flt_bits_val(myFloat));


    return 0;

}
