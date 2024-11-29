#include <stdio.h> 
#include <string.h> //string handling library   
#include <ctype.h> //character handling functions

/*Function Prototype*/
int is_valid_float(const char *str);
//This functon determines the floating point numbers in the file

int main() {
    FILE *in_file, *out_file; //Input and output file pointers 
    char store[100]; //array to store float data from file
    int invalid_count = 0; //counter for number of non-floats in file

    /*Opening input file*/
    in_file = fopen("data.txt", "r"); 
    //use "r" because it is txt and we want to read the file
    
    /*Error Opening Input file case:*/
    if (in_file == NULL) { 
        fprintf(stderr, "Error opening input file\n");
        //stderr to print error message
        return -1; //return -1 to program for indication of error
    }

    /*Open output file*/
    out_file = fopen("valid_data.txt", "w"); //creating output file
    //use 'w' to indicate writing
    
    /*Error Opening Output file case:*/
    if (out_file == NULL) {
        fprintf(stderr, "Error opening output file\n");
        fclose(in_file); //closing input file before exiting program
        return -1;
    }

    // Read each token from the input file
    while (fscanf(in_file, "%s", store) != EOF) {
        // Check if the token is a valid float
        if (is_valid_float(store)) {
            // If valid, write it to the output file
            fprintf(out_file, "%s ", store);
        } else {
            // If invalid, increment the invalid count
            invalid_count++;
        }
    }

    // Report the number of invalid values
    printf("Number of invalid float values: %d\n", invalid_count);

    // Close the files
    fclose(in_file);
    fclose(out_file);

    return 0;
}

/*Function Definition:*/
int is_valid_float(const char *str) {
    int i=0;
    int has_digit_before_dot = 0, has_digit_after_dot = 0;

    // Skip leading whitespace
    while (isspace(str[i])) 
        i++; //increment i and skip whitespace

    // Check optional sign
    if (str[i] == '+' || str[i] == '-') {
        i++; //increment i and skip sign convention
    }

    // Ensure there's at least one digit before the dot or after the first digit if no dot
    while (isdigit(str[i])) {
        has_digit_before_dot = 1;
        i++;
    }

    // If there is a decimal point, we must have digits after it
    if (str[i] == '.') {
        i++;
        while (isdigit(str[i])) {
            has_digit_after_dot = 1;
            i++;
        }
    }


    // After processing, there should be no other non-digit characters
    if (str[i] != '\0') {
        return 0; // Invalid, because there are trailing characters 
    }

    // Valid float if:
    /*There were digits before the decimal point or after the decimal point.*/
    return (has_digit_before_dot || has_digit_after_dot); 
    //returns 0 only when both are 0.
}