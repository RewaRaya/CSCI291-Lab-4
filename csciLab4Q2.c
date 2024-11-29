
Terms · Privacy · Program Policies
Last account activity: 22 minutes ago
Details
#include <stdio.h>
#include <stdlib.h>

/*Define constants*/
#define WIDTH 512
#define HEIGHT 512

//Function Prototypes:
int readPGMText(const char *filename, unsigned char *pixels, int width, int height);
int writePGMText(const char *filename, unsigned char *pixels, int width, int height);
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height);
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height);
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height);
//unsigned because data type differs 


//MAIN Function
int main() {
    // string literals (pointers) for image names  
    const char *cover_image = "baboon.pgm"; 
    const char *secret_image = "farm.pgm";
    const char *stego_image = "stego_image_bin.pgm";
    const char *extracted_secret = "extracted_secret.pgm";

    //pointers declarations:
    unsigned char *coverPixels, *secretPixels, *outputPixels;
    
    //declare variables
    int coverWidth = WIDTH, coverHeight = HEIGHT;
    int secretWidth = WIDTH, secretHeight = HEIGHT;

    // Dynamically allocate memory for cover image
    coverPixels = (unsigned char *)malloc(coverWidth*coverHeight*sizeof(unsigned char));
    /*allocating memory for coverPixels with size of width*height 
    with each pixel being size of 1 byte (unsighned char = 1 byte)*/
    
    //If unsuccessful to allocate memory:
    if (coverPixels == NULL) {
        printf("Error: Memory allocation failed for coverPixels\n");
        return -1; //exit with -1
    }

    // Read the cover image with function call
    if (readPGMText(cover_image, coverPixels, coverWidth, coverHeight) != 0) {
        //if reading fails:
        free(coverPixels); //free allocated memory before exiting program
        return -1;
    }

    // Dynamically allocate memory for secret image
    secretPixels = (unsigned char *)malloc(secretWidth*secretHeight*sizeof(unsigned char));
    //secretPixels will hold the secret image's pixel data
    
    //Case if allocation fails:
    if (secretPixels == NULL) {
        printf("Error: Memory allocation failed for secretPixels\n");
        free(coverPixels);
        return -1;
    }

    // Read the secret image with function call
    if (readPGMText(secret_image, secretPixels, secretWidth, secretHeight) != 0) {
        //stores secret image's pixel data in secretPixels.
        //Case if unsuccessful:
        free(coverPixels);
        free(secretPixels);
        return -1;
    }

    // Check if dimensions match
    if (coverWidth != secretWidth || coverHeight != secretHeight) {
        //when cover image and secret image dont match in dimensions:
        printf("Error: Image dimensions do not match\n");
        free(coverPixels);
        free(secretPixels);
        return -1;
    }

    // Embed the secret image into the cover image with function call
    embedLSB(coverPixels, secretPixels, coverWidth, coverHeight);
    /*embeds the secret image inside the cover image using the least significant 
    bit of each pixel in the cover image.*/

    // Save the stego image in binary format by writing it with function call
    /*writes modified image that contains secret image in a new binary file*/
    if (writePGMBinary(stego_image, coverPixels, coverWidth, coverHeight) != 0) {
        //case if unsuccessful:
        free(coverPixels);
        free(secretPixels);
        return -1;
    }

    // Dynamically allocate memory for the extracted secret image with function call
    outputPixels = (unsigned char *)malloc(coverWidth*coverHeight*sizeof(unsigned char));
    //case if unsuccessful:
    if (outputPixels == NULL) {
        printf("Error: Memory allocation failed for outputPixels\n");
        free(coverPixels);
        free(secretPixels);
        return -1;
    }

    // Extract the secret image from the stego image with function call
    extractLSB(coverPixels, outputPixels, coverWidth, coverHeight);
    /*Extracts the secret image from the stego image by 
    using the least significant bits of each pixel in the coverPixels array.*/

    // Save the extracted secret image in text format using function call
    if (writePGMText(extracted_secret, outputPixels, coverWidth, coverHeight) != 0) {
        //if case unsuccessful:
        free(coverPixels);
        free(secretPixels);
        free(outputPixels);
        return -1;
    }

    // Clean up before exiting program:
    /*Frees the dynamically allocated memory for the cover image, secret image, 
    and the extracted secret image to prevent memory leaks.*/
    free(coverPixels);
    free(secretPixels);
    free(outputPixels);

    //Print message to user for successful operation:
    printf("Steganography completed successfully.\n");

    return 0;
}

/* Function to read a PGM image in text format. */
int readPGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "r"); // Open file for reading
    //if opening file was unsuccessful:
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    // Dynamically allocate memory for temporary storage
    char *storage = (char *)malloc(128 * sizeof(char));
    //if memory allocation was unsuccessful:
    if (storage == NULL) {
        printf("Error: Memory allocation failed for storage\n");
        fclose(file);
        return -1;
    }

    fgets(storage, 128, file);  // Read the P2 header
    if (storage[0] != 'P' || storage[1] != '2') {
        //if file was not pgm format:
        printf("Error: Invalid PGM file format\n");
        free(storage);
        fclose(file);
        return -1;
    }

    // Skip comments by replacing them 
    while (fgets(storage, 128, file) && storage[0] == '#');

    // Read width, height, and max gray value
    int w, h, max_val;
    sscanf(storage, "%d %d", &w, &h);
    fscanf(file, "%d", &max_val);
    
    //if values are not what is expected:
    if (w != width || h != height || max_val != 255) {
        printf("Error: Image dimensions or max gray value do not match expected values.\n");
        free(storage);
        fclose(file);
        return -1;
    }

    // Read pixel values by iterating through the allocation that generates an array
    for (int i = 0; i < width * height; i++) {
        int pixel;
        fscanf(file, "%d", &pixel);
        *(pixels +i) = (unsigned char)pixel;
    }

    free(storage); // Free dynamically allocated storage
    fclose(file); //close file before exiting
    return 0;
}

// Function to write a PGM image in text format
int writePGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", filename);
        return -1;
    }

    // Write the PGM header
    fprintf(file, "P2\n");
    fprintf(file, "# Created by LSB Steganography Program\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "255\n");

    // Write the pixel values
    for (int i = 0; i < width * height; i++) {
        fprintf(file, "%d\n", *(pixels + i));
    }

    fclose(file);
    return 0;
}

// Function to write a PGM image in binary format
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "wb");  // Open file in binary write mode
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", filename);
        return -1;
    }

    // Write the PGM header
    fprintf(file, "P5\n");
    fprintf(file, "# Created by LSB Steganography Program\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "255\n");

    // Write the pixel values in binary format
    size_t written = fwrite(pixels, sizeof(unsigned char), width * height, file);
    if (written != width * height) {
        printf("Error: Failed to write all pixel data to %s\n", filename);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}

// Function to embed the secret image into the cover image
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        unsigned char coverPixel = *(coverPixels + i);
        unsigned char secretPixel = *(secretPixels + i);

        // Extract the upper 4 bits (MSBs) of the cover image pixel
        unsigned char coverUpper = coverPixel & 0xF0;

        // Extract the upper 4 bits (MSBs) of the secret image pixel and shift them to the lower 4 bits
        unsigned char secretUpper = (secretPixel >> 4) & 0x0F;

        // Combine the two to form the stego pixel
        unsigned char stegoPixel = coverUpper | secretUpper;

        // Store the stego pixel back in the coverPixels memory space
        *(coverPixels + i) = stegoPixel;
    }
}

// Function to extract the secret image from the stego image
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        unsigned char stegoPixel = *(coverPixels + i);

        // Extract the lower 4 bits (LSBs) and shift them to the upper 4 bits
        unsigned char secretUpper = (stegoPixel & 0x0F) << 4;

        // Set the lower 4 bits of the secret pixel to zero
        unsigned char secretPixel = secretUpper;

        // Store the extracted secret pixel in the outputPixels memory space
        *(outputPixels + i) = secretPixel;
    }
}