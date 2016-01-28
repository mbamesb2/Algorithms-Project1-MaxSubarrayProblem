/* CS325 Project 1 */
/* CS_325_400_W2016 */
/* Project Group 15:Michael Bamesberger, Allan Chan, Diana Baelly */
/*Calculates a maximum subarray sum using variety of techniques to 
/*demonstrate big-O runtime differences. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

void Alg1_Enumeration(int intArray[], int n);
void Alg2_BetterEnumeration(int intArray[], int n);
void fillArray(char* inputFile, int line, int);
int numLinesInFile(char* inputFile);
void getLineInFile(char* inputFile, int target_line, char* string);
int numElementsInLine (char* inputFile, int line );
void nullTermLine (char* string);
int elementsInString(char* string);
void fillNewArray(char* inputFile, int line, int* array, int numElements);
void alg3_DivideAndConquer(int* array, int numElements);
void alg3_maxSubArray(int *input, int low_idx, int high_idx, int* current_low_idx, int* current_high_idx, int* sum);
void alg3_maxLoopSubArray(int* input, int low_idx, int mid, int high_idx, int* loop_low, int* loop_high, int* loop_sum);
void alg4_dynMaxSubArray(int* array, int numElements);

#define MAX_SIZE 1000000

void Alg1_test_Enumeration(int intArray[], int n){
    int sum = 0, i, j, k, start, end, startPos = 0, endPos = 0;
    int max_sum = INT_MIN;
    if(n == 1){ //Only one value in array
        max_sum = intArray[0];
    }else{
        for(j = 0; j < n; j++){  //Tracks size of subarray
            for(i = 0; i < n; i++){ //Tracks start/end position of subarray
                start = i;
                end = i + j;
    
                if(end < n){    //Doesn't exceed end of array)
                    for(k = start; k <= end; k++){ //Sums individual subarray values
                            sum += intArray[k];
                        }
    
                    if(sum > max_sum){  //Check this subarray against max
                        max_sum = sum;
                        startPos = start;
                        endPos = end;
                    }
    
                    sum = 0;    //Reset sum for next subarray
                }
            }
        }
    }
}

void Alg2_test_BetterEnumeration(int intArray[], int n){
	int i, j, sum, max_sum = INT_MIN, startPos = 0, endPos = n - 1;
    for(i = 0; i <= n - 1; i++){ 
        sum = 0;
		for(j = i; j <= n-1; j++){
			sum += intArray[j];

			if(sum > max_sum){
				max_sum = sum;
				startPos = i;
				endPos = j;
			}
		}
    }
}


void alg3_test_maxLoopSubArray(int* input, int low_idx, int mid, int high_idx, int* loop_low, int* loop_high, int* loop_sum){

        int i = 0;
        int left_sum = INT_MIN;
        int right_sum = INT_MIN;
        int current_sum = 0;

        //mid to left of array
        for(i = mid; i >= 0; i--){
            current_sum = current_sum + input[i];
            if(current_sum > left_sum){
                left_sum = current_sum;
                *loop_low = i;
            }
        }

        //mid to right of array
        current_sum = 0;
        for(i = (mid + 1); i <= high_idx; i++){
            current_sum = current_sum + input[i];
            if(current_sum > right_sum){
                right_sum = current_sum;
                *loop_high = i;
            }
        }

        *loop_sum = (left_sum + right_sum); //max sum found
}



void alg3_test_maxSubArray(int *input, int low_idx, int high_idx, int* current_low_idx, int* current_high_idx, int* current_sum){

    //If only one element in array
    if(low_idx == high_idx){
        *current_low_idx = low_idx;
        *current_high_idx = high_idx;
        *current_sum = input[high_idx];
    } else {
        int mid = (low_idx + high_idx)/2;

        //Find max subarray in left half of array
        int left_low = 0;
        int left_high = 0;
        int left_sum = 0;

        alg3_test_maxSubArray(input, low_idx, mid, &left_low, &left_high, &left_sum);

        //Find max subarray for right half of array
        int right_low = 0;
        int right_high = 0;
        int right_sum = 0;

        alg3_test_maxSubArray(input, mid + 1, high_idx, &right_low, &right_high, &right_sum);

        //Find max subarray for looping around array
        int loop_low = 0;
        int loop_high = 0;
        int loop_sum = 0;

        alg3_test_maxLoopSubArray(input, low_idx, mid, high_idx, &loop_low, &loop_high, &loop_sum);
        //

        //If left half of array is bigger
        if((left_sum >= right_sum) && (left_sum >= loop_sum)){
            *current_low_idx = left_low;
            *current_high_idx = left_high;
            *current_sum = left_sum;
        } else if ((right_sum >= left_sum) && (right_sum >= loop_sum)){
            *current_low_idx = right_low;
            *current_high_idx = right_high;
            *current_sum = right_sum;
        } else {
            *current_low_idx = loop_low;
            *current_high_idx = loop_high;
            *current_sum = loop_sum;
        }
    }
}


void alg3_test_DivideAndConquer(int* input, int numElements){
    int i = 0;

    int low_idx = 0;
    int high_idx = numElements - 1;
    int final_sum = 0;

    int final_low = 0;
    int final_high = 0;

    alg3_test_maxSubArray(input, low_idx, high_idx, &final_low, &final_high, &final_sum);
}

void alg4_test_dynMaxSubArray(int* array, int numElements){
    int i, j, startPos = 0, endPos = 0;
    int maxSum = 0;
    int currentMax = 0;
    
    
    if (numElements == 0)   // If there are no elements in the line array, then return
    {
        return;
    }
    
    for (j = 0; j < numElements; j++)    //Iterate through the line array
    {
        currentMax = currentMax + array[j];
        if (currentMax > maxSum){
            maxSum = currentMax;
			endPos = j;

        }
        else if (currentMax < 0){       // if the currentMax is negative, reset the number
            currentMax = 0;
			startPos = j + 1;
        }
        
    }
}

int main(){
    int line;
    int numElements;
    int inputNum, algNum, i, j;
    char* inputFile = "MSS_Problems.txt";
    int numLines = numLinesInFile(inputFile);
    clock_t timer;
    srand (time(NULL));

    printf("\nProject 1: Evaluation of Max-Sum-Subarray Algorithm Runtimes\n\n");

    for(line = 0; line < numLines; line++){
        numElements = numElementsInLine(inputFile, line);  
        int* array = (int*)malloc(numElements * (sizeof(int)));
        fillNewArray(inputFile, line, array, numElements);

        printf("Running Algorithm 1: Enumeration");       
        Alg1_Enumeration(array, numElements);
        printf("\nAlgorithm 1: Results written to file\n\n");       

        printf("Running Algorithm 2: Better Enumeration");       
        Alg2_BetterEnumeration(array, numElements);
        printf("\nAlgorithm 2: Results written to file\n\n");       

        printf("Running Algorithm 3: Divide-And-Conquer");       
        alg3_DivideAndConquer(array, numElements);
        printf("\nAlgorithm 3: Results written to file\n\n");       

        printf("Running Algorithm 4: Linear-Time");       
        alg4_dynMaxSubArray(array, numElements);
        printf("\nAlgorithm 4: Results written to file\n\n");       

    }

    ///////////////////
    //
    //
    ///////////////////
    int k;
    int rand_array_vals[MAX_SIZE];
    int rand_array_size_dnc[] = {1000, 2000, 4000, 6000, 8000, 10000, 16000, 20000, 32000, 64000};
    int rand_array_size_enum[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int rand_array_size_better_enum[] = {1000, 2000, 4000, 6000, 8000, 10000, 16000, 20000, 32000, 64000};
    int rand_array_size_linear[] = {100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000};
/*    for(i = 0; i < sizeof(rand_array_size_vals); i++){
        rand_array_vals[i] = rand()%200 +(-100);
    }
*/

        //Random number generation and array test for Algorithm
 //       printf("\nState the size of the array to test:");
 //       scanf("%d", &inputNum);


    printf("Press any key to run Algorithms for Experimental Analysis:");
    scanf("%d", &algNum);

    int *randArray = (int*)malloc(MAX_SIZE*sizeof(int));
/*        for (i = 0; i < inputNum; i++){
            randArray[i] = rand()%200 + -100;
        }
*/
        /*Print the contents of the array.*/

    printf("Running Algorithm 1\n");
    for(i = 0; i < 10; i++){
        for(k = 0; k < 10; k++){ 
            for (j = 0; j < rand_array_size_enum[i]; j++){
                rand_array_vals[j] = rand()%200 + -100;
            }

            timer = clock();
            Alg1_test_Enumeration(rand_array_vals, rand_array_size_enum[i]);
            timer = clock() - timer;

            printf("Size n=%d\n ", rand_array_size_enum[i]);
            printf("\nEnum Test %d took %f seconds\n", i, (float)timer / (float)CLOCKS_PER_SEC);
        }
    }
    printf("Running Algorithm 2\n");
    for(i = 0; i < 10; i++){
        for(k = 0; k < 10; k++){
            for (j = 0; j < rand_array_size_better_enum[i]; j++){
                rand_array_vals[j] = rand()%200 + -100;
            }

            timer = clock();
            Alg2_test_BetterEnumeration(rand_array_vals, rand_array_size_better_enum[i]);
            timer = clock() - timer;
                        
            printf("Size n=%d\n", rand_array_size_better_enum[i]);
            printf("\nBetter Enum Test %d took %f seconds\n", i, (float)timer / (float)CLOCKS_PER_SEC);
        }
    }

    for(i = 0; i < 10; i++){
        for(k = 0; k < 10; k++){  
            for (j = 0; j < rand_array_size_dnc[i]; j++){
                    rand_array_vals[j] = rand()%200 + -100;
            }
                    
            timer = clock();
            alg3_test_DivideAndConquer(rand_array_vals, rand_array_size_dnc[i]);
            timer = clock() - timer;
    
            printf("\nDnC Test %d took %f seconds\n", i, (float)timer / (float)CLOCKS_PER_SEC);
            printf("Size of n is: %d\n ", rand_array_size_dnc[i]);
        }
    }
    for(i = 0; i < 10; i++){
        for(k = 0; k < 10; k++){  
            for (j = 0; j < rand_array_size_dnc[i]; j++){
                rand_array_vals[j] = rand()%200 + -100;
            }
    
            timer = clock();
            alg4_test_dynMaxSubArray(rand_array_vals, rand_array_size_linear[i]);
            timer = clock() - timer;

            printf("\nLinear Test %d took %f seconds\n", i, (float)timer / (float)CLOCKS_PER_SEC);
            printf("Size of n is: %d\n ", rand_array_size_linear[i]);
        }
    }
    return 0;
}

void Alg1_Enumeration(int intArray[], int n){
    int sum = 0, i, j, k, start, end, startPos = 0, endPos = 0;
    int max_sum = INT_MIN;
    if(n == 1){ //Only one value in array
        max_sum = intArray[0];
    }else{
        for(j = 0; j < n; j++){  //Tracks size of subarray
            for(i = 0; i < n; i++){ //Tracks start/end position of subarray
                start = i;
                end = i + j;
    
                if(end < n){    //Doesn't exceed end of array)
                    for(k = start; k <= end; k++){ //Sums individual subarray values
                            sum += intArray[k];
                        }
    
                    if(sum > max_sum){  //Check this subarray against max
                        max_sum = sum;
                        startPos = start;
                        endPos = end;
                    }
    
                    sum = 0;    //Reset sum for next subarray
                }
            }
        }
    }
    
    FILE *outputResults = fopen("MSS_Results.txt", "a");

    fprintf(outputResults, "%s", "Enumeration\n");
    fprintf(outputResults, "%c", '[');

    for(i = 0; i < n; i++){
        if(i != 0){
            fprintf(outputResults, "%s", ", ");
        }

        fprintf(outputResults, "%d", intArray[i]);
    }

    fprintf(outputResults, "%c%c", ']' ,'\n');

    //Save max subarray
    fprintf(outputResults, "%c", '[');
    for(i = startPos; i <= endPos; i++){
        if(i != startPos){
            fprintf(outputResults, "%s", ", ");
        }

        fprintf(outputResults, "%d", intArray[i]);
    }

    fprintf(outputResults, "%c%c", ']' ,'\n');
    fprintf(outputResults, "%d\n\n", max_sum);

    fclose(outputResults);
}

void Alg2_BetterEnumeration(int intArray[], int n){
	int i, j, sum, max_sum = INT_MIN, startPos = 0, endPos = n - 1;
     for(i = 0; i <= n - 1; i++){ 
        sum = 0;
		for(j = i; j <= n-1; j++){
			sum += intArray[j];

			if(sum > max_sum){
				max_sum = sum;
				startPos = i;
				endPos = j;
			}
		}
    }

    FILE *outputResults = fopen("MSS_Results.txt", "a");

    fprintf(outputResults, "%s", "Better Enumeration\n");
    fprintf(outputResults, "%c", '[');

    for(i = 0; i < n; i++){
        if(i != 0){
            fprintf(outputResults, "%s", ", ");
        }

            fprintf(outputResults, "%d", intArray[i]);
    }

        fprintf(outputResults, "%c%c", ']' ,'\n');

    //Save max subarray
        fprintf(outputResults, "%c", '[');
        for(i = startPos; i <= endPos; i++){
            if(i != startPos){
                fprintf(outputResults, "%s", ", ");
            }

            fprintf(outputResults, "%d", intArray[i]);
        }

    fprintf(outputResults, "%c%c", ']' ,'\n');
    fprintf(outputResults, "%d\n\n", max_sum);

    fclose(outputResults);
}

//get number of lines in file
int numLinesInFile(char *inputFile){
    int ch = 0;
    int numLines = 0, numChar = 0;

    FILE *fp;
    fp = fopen(inputFile, "r");
    if(fp == NULL){
        printf("The file %s could not be opened\n", inputFile);
    }
    while(!feof(fp)){
        ch = fgetc(fp);
         if((ch == '\n') && (numChar > 0)){
            numLines++;
            numChar = 0;    //Reset for next line
        }else if(ch == '\n'){
            numChar = 0; 
        }else{
           numChar++;   //Non return character
        }
    }
    fclose(fp);

    return numLines;
}

//get number of elements in line of file
int numElementsInLine(char* inputFile, int target_line){
    int line_num = 0;

    FILE *fp;
    fp = fopen(inputFile, "r");

    if(fp == NULL){
        printf("The file %s could not be opened\n", inputFile);
    }

    char line[MAX_SIZE]; //stores line read

    while(fgets(line, sizeof(line), fp) != NULL){
        if (line_num == target_line){
            break;
        }
        line_num++;
    }

    nullTermLine(line);

    int numElements = 0;
    numElements = elementsInString(line);

   return numElements;
}

//null terminate string
void nullTermLine(char* string){
    size_t length = strlen(string) - 1;
    if(string[length] == '\n'){
        string[length] = '\0';
    }
}

int elementsInString(char* string){
    int i = 0;
    int comma_count = 0;
    int length = strlen(string);

    for(i = 0; i < length; i++){
        if(string[i] == ','){
            comma_count++;
        }
    }

    if(comma_count == 0){
        return 1;   //number of elements is 1
    } else {
        return (comma_count + 1);
    }
}

void fillNewArray(char* inputFile, int line, int* array, int numElements){
    int i = 0;

    //initialize array
    for(i = 0; i < numElements; i++){
        array[i] = 0;
    }

    char string[MAX_SIZE];
    getLineInFile(inputFile, line, string);

    char* token;
    token = strtok(string, "[,]");
    int currentIdx = 0;
    int current;

    while(token != NULL){

        sscanf(token, "%d", &current);
        if(currentIdx >= numElements){
            break;
        }

        array[currentIdx++] = current;

        //Continue with next token
        token = strtok(NULL, "[,]");
    }
//
//    for(i=0; i < numElements; i++){
//        printf("contents in loop[%d]: %d\n ", i, array[i]);
//    }

}

void getLineInFile(char *input, int target_line, char* string){
    FILE *fp;
    fp = fopen(input, "r");

    if(fp == NULL){
        perror("Can't open file");
    }  else {
        int line_num = 0;
        char line[MAX_SIZE];

        while(fgets(line, (sizeof(line)), fp) != NULL){
            if(line_num == target_line){
                break;
            }
            line_num++;

        }
    }
}

void alg3_DivideAndConquer(int* input, int numElements){
    int i = 0;

    int low_idx = 0;
    int high_idx = numElements - 1;
    int final_sum = 0;

    int final_low = 0;
    int final_high = 0;

    alg3_maxSubArray(input, low_idx, high_idx, &final_low, &final_high, &final_sum);

    int current_idx;
    int num_elements_found = final_high - final_low + 1;
    if(num_elements_found <= 0){
        //printf("Invalid number of elements found\n");
        return;
    }

    int* result = (int*)malloc(num_elements_found * sizeof(int));
    for(i=0; i < num_elements_found; i++){
        current_idx = final_low + i;
        if((current_idx < 0) || (current_idx >= num_elements_found)){
           //printf("Invalid bounds for array\n");
        }
        result[i] = input[current_idx];
    }

    /*for(i=0; i < num_elements_found; i++){
        printf("Array[%d] contains: %d \n", i, result[i]);
    }
    */

    FILE *outputResults = fopen("MSS_Results.txt", "a");

    fprintf(outputResults, "%s", "Divide and Conquer\n");
    fprintf(outputResults, "%c", '[');

    i = 0;
    for(i = 0; i < numElements; i++){
        if(i != 0){
            fprintf(outputResults, "%s", ", ");
        }

        fprintf(outputResults, "%d", input[i]);
    }
    fprintf(outputResults, "%c%c", ']' ,'\n');

    //Save max subarray
    int result_sum = 0;
    fprintf(outputResults, "%c", '[');
    for(i = 0; i < num_elements_found; i++){
        if(i!=0){
            fprintf(outputResults, "%s", ", ");
        }
        fprintf(outputResults, "%d", result[i]);
        result_sum = result_sum + result[i];
    }
    fprintf(outputResults, "%c%c", ']' ,'\n');
    fprintf(outputResults, "%d\n\n", result_sum);

    fclose(outputResults);
}

void alg3_maxSubArray(int *input, int low_idx, int high_idx, int* current_low_idx, int* current_high_idx, int* current_sum){

    //If only one element in array
    if(low_idx == high_idx){
        *current_low_idx = low_idx;
        *current_high_idx = high_idx;
        *current_sum = input[high_idx];
    } else {
        int mid = (low_idx + high_idx)/2;

        //Find max subarray in left half of array
        int left_low = 0;
        int left_high = 0;
        int left_sum = 0;

        alg3_maxSubArray(input, low_idx, mid, &left_low, &left_high, &left_sum);

        //Find max subarray for right half of array
        int right_low = 0;
        int right_high = 0;
        int right_sum = 0;

        alg3_maxSubArray(input, mid + 1, high_idx, &right_low, &right_high, &right_sum);

        //Find max subarray for looping around array
        int loop_low = 0;
        int loop_high = 0;
        int loop_sum = 0;

       alg3_maxLoopSubArray(input, low_idx, mid, high_idx, &loop_low, &loop_high, &loop_sum);
        //

        //If left half of array is bigger
        if((left_sum >= right_sum) && (left_sum >= loop_sum)){
            *current_low_idx = left_low;
            *current_high_idx = left_high;
            *current_sum = left_sum;
        } else if ((right_sum >= left_sum) && (right_sum >= loop_sum)){
            *current_low_idx = right_low;
            *current_high_idx = right_high;
            *current_sum = right_sum;
        } else {
            *current_low_idx = loop_low;
            *current_high_idx = loop_high;
            *current_sum = loop_sum;
        }
    }
}

void alg3_maxLoopSubArray(int* input, int low_idx, int mid, int high_idx, int* loop_low, int* loop_high, int* loop_sum){

        int i = 0;
        int left_sum = INT_MIN;
        int right_sum = INT_MIN;
        int current_sum = 0;

        //mid to left of array
        for(i = mid; i >= 0; i--){
            current_sum = current_sum + input[i];
            if(current_sum > left_sum){
                left_sum = current_sum;
                *loop_low = i;
            }
        }

        //mid to right of array
        current_sum = 0;
        for(i = (mid + 1); i <= high_idx; i++){
            current_sum = current_sum + input[i];
            if(current_sum > right_sum){
                right_sum = current_sum;
                *loop_high = i;
            }
        }

        *loop_sum = (left_sum + right_sum); //max sum found
}

void alg4_dynMaxSubArray(int* array, int numElements){
    
    int i, j, startPos = 0, endPos = 0;
    int maxSum = 0;
    int currentMax = 0;
    
    
    if (numElements == 0)   // If there are no elements in the line array, then return
    {
        return;
    }
    
    for (j = 0; j < numElements; j++)    //Iterate through the line array
    {
        currentMax = currentMax + array[j];
        if (currentMax > maxSum){
            maxSum = currentMax;
			endPos = j;

        }
        else if (currentMax < 0){       // if the currentMax is negative, reset the number
            currentMax = 0;
			startPos = j + 1;
        }
        
    }

   FILE *outputResults = fopen("MSS_Results.txt", "a");

    fprintf(outputResults, "%s", "Linear Time\n");
    fprintf(outputResults, "%c", '[');

    for(i = 0; i < numElements; i++){
        if(i != 0){
            fprintf(outputResults, "%s", ", ");
        }

        fprintf(outputResults, "%d", array[i]);
    }

    fprintf(outputResults, "%c%c", ']' ,'\n');

    //Save max subarray
    fprintf(outputResults, "%c", '[');
    for(i = startPos; i <= endPos; i++){
        if(i != startPos){
            fprintf(outputResults, "%s", ", ");
        }

        fprintf(outputResults, "%d", array[i]);
    }

    fprintf(outputResults, "%c%c", ']' ,'\n');
    fprintf(outputResults, "%d\n\n", maxSum);

    fclose(outputResults);
}
