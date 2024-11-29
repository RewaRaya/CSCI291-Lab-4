#include <iostream>
#include <iomanip>
#include <cstdlib> 
using namespace std; //to avoid typing std:: everytime

//function prototype:
void print_real(float number, int fieldspace, int precision);
/*prints value of number with a specific width and precision*/
//precision = no. of decimal places
//filedspace = width of output

// Template function to update the scale:
template <typename T>
void update_scale(T &m1, T &m2, T m3=10) {
    /*m3 is a deafult C++ parameter*/

    //Declaring variables:
    T sum, diff;
    /*sum and diff used for the operations not to use the updated value of m1*/
    
    // Update m1: (original m1 + original m2)*m3
    sum = (m1 + m2)*m3;

    // Update m2: (original m1 - original m2)*m3
    diff = (m1 - m2)*m3;
    
    //updating m1 and 2 with their new values:
    m1 = sum;
    m2 = diff;
}

//MAIN FUNCTION:
int main(void) {
    //declaring variables:
    float a, b;
    
    cout<<"Please input two real numbers: "; //cout for printing output messages
    cin >>a>>b; //cin for scanning input data

    //prints a and b with the specified precision and fieldspace:
    print_real(a, 7, 3); 
    print_real(b, 7, 3);

    // Call the function update_scale with the original inputted parameters a and b
    update_scale(a, b);
    
    // print updated value of a and b:
    print_real(a, 7, 3);  
    print_real(b, 7, 3); 
}

/* FUNCTION DEFINITION */
void print_real(float number, int fieldspace, int precision) {
    cout<<fixed<<setprecision(precision)<<setw(fieldspace)<<number<<endl;
}