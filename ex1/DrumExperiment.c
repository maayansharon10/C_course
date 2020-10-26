#include <stdio.h>
#include <stdlib.h> // Imported for "EXIT_FAILURE" and "EXIT_SUCCESS"
#include <string.h> // Imported for "strtok"
#include <math.h>
//todo deal with empty input!!!!
// todo check numbers which start with 0
/**
 * @brief Defines the maximum line length.
 */
#define LINE_MAX_SIZE 1024

/**
 * @brief The delimiters used to separate the input
 */
#define INPUT_DELIMS " \t\r\n"

/**
 * @brief Defines the maximum array length of gArray and hArray
 */
#define ARRAY_MAX_SIZE 100

/**
 * @brief Defines false return value for functions
 */
#define FALSE 0

/**
 * @brief Defines true return value for functions
 */
#define TRUE 1

/**
 * @brief Defines epsilon
 */
#define EPSILON 0.00000001

/**
 * @brief Defines invalid int
 */
#define INVALID_INT -1

/**
 * @brief Defines a erroe msg to err
 */
#define ERROR_MSG  "ERROR\n"

/**
 * @brief Defines how to print a star
 */
#define STAR_TO_PRINT  '*'

/**
 * @brief Defines how to print a star
 */
#define STARS_NUM  20


/**
 * @brief Defines how to print a star
 */
#define ROUND_NUMBER 1000

/**
 * if char is a number from 0-9 return 1, if not, return 0
 */
int isChrDigit(char chr)
{
    int number = chr - '0';
    /* if it is valid - continue loop, and if not, return failure */
    if (number < 0 || 9 < number)
    {
        return FALSE; //not a digit
    } // chr is a digit -
    return TRUE;
}

/**
 * assuming chr is a vaild digit, return the digit(int) val for chr according to ascii table.
 * @param chr 
 * @return the digit(int) val for chr according to ascii table.
 */
int chrToDigit(char chr)
{
    return (chr - '0');
}


/**
 * check if a number (chr array) is valid double
 * @param value
 * @param length
 * @return 1 if number is valid, 0 otherwise
 */
int isAValidDouble(char * value, size_t length)
{
    /* check value is not longer than the double def according to ex */
    if (length > 9)
    {
        return FALSE;
    }

    int pointCounter = 0;
    for (size_t i = 0; i < length; i++)
    {
        /* if there's more than one point in the value - not valid */
        if (pointCounter > 1)
        {
            return FALSE;
        }
        /* if it is a point - add to counter, otherwise, check if chr is a digit*/
        int isPoint = value[i] - '.';
        if (isPoint == 0) // if it is a point -
        {
            // if it is the last chr in value - cannot be '.'
            if (i == length - 1)
            {
                return FALSE;
            } // update point counter
            pointCounter += 1;
        }
        else //if not a point
        {
            // if chr is not a digit - return false
            if (!isChrDigit(value[i]))
            {
                return FALSE;
            }
            /* if it is valid - continue loop, and if not, return failure */
        }
    }
    /* if got to end of loop then value is a valid number*/
    return TRUE;

}

/**
 * convert char array to int
 * @param value
 * @param length
 * @return char array as int
 */
int chrArrToInt(char * value, int length)
{
    int newint = 0;

    for (int i = 0; i < length; i++)
    {
        int num = chrToDigit(value[i]);
        newint = newint * 10;
        newint = newint + num;
    }
    return newint;
}

/**
 * finds index of point if exists in number.
 * if exist return the index of the point in number, if doesn't exist - return -1
 * @param value
 * @param4 length
 * @return if exist return the index of the point in number, if doesn't exist - return -1
 */
int findPointInNum(const char * value, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (value[i] == '.')
        {
            return i;
        }
    }
    return INVALID_INT;
}

/**
 * convert char aray into a double
 * @param value pointer
 * @param length
 * @return a double number
 */
double chrArrToDouble(char * value, int length)
{
    // return the index of the point in value
    int point = findPointInNum(value, length);
    double newDouble = 0;
    // if doesn't have a point -
    if (point == INVALID_INT)
    {
        newDouble = (double) chrArrToInt(value, length);
    }
    else
    {
        int beforePoint = chrArrToInt(value, point);
        int afterLength = length - point - 1;
        int afterPoint = chrArrToInt(value + point + 1, afterLength);
        newDouble = beforePoint + (1 / pow(10, afterLength)) * afterPoint;
        //newDouble = beforePoint + (1 / (10^afterLength) * afterPoint;

    }
    return newDouble;
}

/**
 *  convert a line, char array, to array of doubles, if possible
 * assuming counter would always be sent as 0 - later would represent the size of array, if valid
 * @param line
 * @param array
 * @param counter
 * @return  0 if not successful, 1 if successful
 */
int chrArrayToDoubleArray(char * line, double * array, int * size)
{

    int counter = 0;

    char * value = NULL;
    /* Now "line" contains the contents of the entire line, including the suffix "\n",
     and "strlen(line)" contains the line length.
     Lets read the first value. If there's no input, we'll get "NULL" */

    /* 1. get first value from string */
    value = strtok(line, INPUT_DELIMS);

    /* 2. Handle every value and read the next value */

    while (value != NULL)
    {

        // todo - in class they said i have to send the array size as a param, but here i don't
        //  have to since i alreay know it's size cannot be over ARRAY_MAX_SIZE or if val is
        //  null, is there something i am missing?

        /* A) if there's too many values in line - line is not valid - return*/
        if (counter > ARRAY_MAX_SIZE)
        {
            return FALSE;
        }
        /* B) get value length */
        size_t length = strlen(value);

        /* C) check number is valid, if not - return */
        if (!isAValidDouble(value, length))
        {
            return FALSE;
        }
        /* D) value is valid - convert to double and put in array*/
        double newValDouble = chrArrToDouble(value, (int) length);
        array[counter] = newValDouble;

        /* E) Read the next value and update counter*/
        value = strtok(NULL, INPUT_DELIMS);
        counter++;
    }
    /* if there were not values added - meaning empty*/
    if (counter == 0)
    {
        return FALSE;
    }
    /* 3. if got here - array is valid, update array size return 1     */

    * size = counter;
    return TRUE;
}

/**
 * create a vaild double array from user input, if possible
 * @param line
 * @param array
 * @param size
 * @return 0 if failed, 1 otherwise
 */
int createValidDoubleArray(double * array, int * size)
{
    /* Reads the gline from the stdin */
    char bufferLine[LINE_MAX_SIZE];
    if (fgets(bufferLine, LINE_MAX_SIZE, stdin) == NULL)
    {
        return FALSE;
    }
    /* try create gArray - if failed creating the array - invalid input - return */
    if (!chrArrayToDoubleArray(bufferLine, array, size))
    {
        return FALSE;
    }
    return TRUE;
}

/**
 * asks user for input, if possible creates a non negaive int
 * @param n
 * @return 1 if successful,  0 otherwise.
 */
int createPositiveInt(int * n)
{

    /* Reads the line from the stdin */
    char line[LINE_MAX_SIZE];
    if (fgets(line, LINE_MAX_SIZE, stdin) == NULL)
    {
        fprintf(stderr, ERROR_MSG);
        return FALSE;
    }

    char * value = NULL;
    /* Now "line" contains the contents of the entire line, including the suffix "\n",
     and "strlen(line)" contains the line length.*/

    /* get first value from string */
    value = strtok(line, "\n");
    size_t length = strlen(value);

    /* check input is a valid number, if not - return */
    for (size_t i = 0; i < length; i++)
    {
        if (!isChrDigit(value[i]))
        {
            return FALSE;
        }
    }

    /*  create new int   */
    int newInt = chrArrToInt(value, (int) length);

    /* check there's only one input*/
    value = strtok(NULL, INPUT_DELIMS);
    if (value != NULL)
    {
        return FALSE;
    }
    * n = newInt;
    return TRUE;


}

/**
 * centralize given array 
 * @param array 
 * @param sizeOrgArray 
 * @param numOfElements 
 */
void centralizeArray(double * array, int sizeOrgArray, int numOfElements)
{


    /*num of zeros should be after (right to) the values*/

    int rightZeroCounter = 0;
    /* check if num is round or not*/

    rightZeroCounter = (int) ((double) (sizeOrgArray - numOfElements) / 2);
    /* check if num is round or not, if so - add one */
    double isRound = ((double) (sizeOrgArray - numOfElements) / 2) - rightZeroCounter;
    if (isRound > 0.0)
    {
        rightZeroCounter += 1;
    }

    /* the index we wish to copy */
    int fromIndex = numOfElements - 1;
    int toIndex = sizeOrgArray - rightZeroCounter - 1; //the first index from right we want to fill

    /*  go over array backwords and copy*/
    for (; toIndex >= 0; toIndex--)
    {

        * (array + toIndex) = array[fromIndex];
        /*  if from index is 0, meaning we finished copping, complete the rest with zeros */
        if (fromIndex == 0)
        { //
            break;
        }
            /* we still didn't finish coping*/
        else
        {
            fromIndex -= 1;

        }
    }
    toIndex -= 1;
    for (; toIndex >= 0; toIndex--)
    {
        array[toIndex] = FALSE;

    }
}

/**
 * normalize given array
 * @param array 
 * @param sizeOrgArray 
 */
void normalizeArray(double * array, int sizeOrgArray)
{

    double arraySum = 0;
    /* calc array sum*/
    for (int i = 0; i < sizeOrgArray; i++)
    {
        arraySum += array[i];
    }
    // we know all items in array are non negative, so the sam is non negative also

    if (arraySum == 0)
    {
        return;
    }

    /* divide each item in sum to normalize it*/
    for (int i = 0; i < sizeOrgArray; i++)
    {

        double newVal = array[i] / arraySum;
        * (array + i) = newVal;
    }

}

/**
 * assuming 2 arrays with same size; copy array1 to array2. now array2 is same as array1
 * @param array1
 * @param array2
 */
void copyArray1IntoArray2(const double * array1, double * array2, int size)
{
    for (int i = 0; i < size; i++)
    {
        * (array2 + i) = array1[i];
    }
}

/**
 * helper function for convolution, which does the actual convolution process
 * @param gArray 
 * @param hArray 
 * @param result 
 * @param size 
 */
void convolutionHelper(const double * gArray, const double * hArray, double * result, int size)
{
    /* initialized temp array*/
    double tempArray[ARRAY_MAX_SIZE] = {0};
    int half = (size / 2);
    // go over t
    for (int t = -half - 1; t < half - 1; t++)
    {


        double tempSum = 0;

        //go over m
        for (int m = -half; m <= half; m++)
        {

            //calc locations
            int fLoc = t - m + half;
            int hLoc = m + 50;

            // make sure locations are valid
            // if not valid - continue:
            if ((hLoc < 0 || size < hLoc) || (fLoc < 0 || size < fLoc))
            {
                continue;
            }
            // if valid - sum up
            tempSum += gArray[fLoc] * hArray[hLoc];
        }

        //place sum in result[t];
        * (tempArray + t + half + 1) = tempSum;
    }
    copyArray1IntoArray2(tempArray, result, size);

    //normalize
    normalizeArray(result, size);

}


/**
 * does the convolution n times on 2 arrays. put results in a result array
 * @param gArray
 * @param hArray
 * @param result
 * @param size
 * @param n num of times we want to run the convolution
 */
void convolution(const double * gArray, const double * hArray, double * result, int size, int n)
{


    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            convolutionHelper(gArray, hArray, result, size);
        }
        else
        {
            convolutionHelper(result, hArray, result, size);
        }
    }
}

/**
 * round the array values according to instructions
 * @param array 
 * @param arraySize 
 */
void roundArrayValues(double * array, int arraySize)
{

    for (int i = 0; i < arraySize; i++)
    {
        double newVal = round(ROUND_NUMBER * array[i]) / ROUND_NUMBER;
        array[i] = newVal;
    }

}

/**
 * the max item of a given array
 * @param array
 * @param arraySize
 * @return the max item of a given array
 */
double getMaxDOubleArray(const double * array, int arraySize)
{
    double max = 0;
    // go through array and find max
    for (int i = 0; i < arraySize; i++)
    {
        if (i == 0)
        {
            max = array[i];
        }
        else
        {
            if (max < array[i])
            { // if current val is bigger- update max
                max = array[i];
            } // if not bigger - continue to next val in array
        }
    }
    return max;
}

/**
 * calc Starts for print usage per one Item of array
 * @param value 
 * @param max 
 * @return 
 */
int calcStartsForItem(double value, double max)
{

    return (int) ((value / max) * STARS_NUM);


}

/**
 * function deals with printing results
 * @param array 
 * @param arraySize 
 * @param max 
 */
void printResults(double * array, int arraySize, double max)
{

    for (int i = 0; i < arraySize; i++)
    {
        // clac num of stars for item
        int numOfStars = calcStartsForItem(array[i], max);
        // print result for item
        printf("%0.3f: ", array[i]);
        for (int j = 0; j < numOfStars; j++)
        {
            printf("%c", STAR_TO_PRINT);
        }
        if ((i != (arraySize - 1)))
        {
            printf("\n");
        }
    }
}

/**
 * deals with output from end to end and prints output to screen
 * @param array
 * @param arraySize
 */
void outputHandler(double * array, int arraySize)
{
    // round 3 point in each elements
    roundArrayValues(array, arraySize);

    // calc max
    double maxVal = getMaxDOubleArray(array, arraySize);

    //check max>epsilon
    if (maxVal < EPSILON)
    {
        return;
    }

    // print in right format
    printResults(array, arraySize, maxVal);
}

/**
 * @brief The main function that runs the program.
 * @param argc Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated
 * multibyte strings that represent the arguments passed to the program from the execution
 * environment. The value of argv[argc] is guaranteed to be a null pointer.
 * @return 0, to tell the system the execution ended without errors, or 1, to
 * tell the system that the code has execution errors.
 */
int main(int argc, const char * argv[])
{
    double gArray[ARRAY_MAX_SIZE] = {0};
    double hArray[ARRAY_MAX_SIZE] = {0};
    double resultArray[ARRAY_MAX_SIZE] = {0};

    /* Initializing the variables that'll contain the lines information */
    int g = 0;
    int * gSize = & g;
    int h = 0;
    int * hSize = & h;
    int n = 0;
    int * pN = & n;
    /* try to create g array, if fails - exit */
    if (!createValidDoubleArray(gArray, gSize))
    {
        fprintf(stderr, ERROR_MSG);
        return EXIT_FAILURE;
    }


    /* try to create h array, if fails - exit */
    if (!createValidDoubleArray(hArray, hSize))
    {
        fprintf(stderr, ERROR_MSG);
        return EXIT_FAILURE;
    }

    // size of  h should be <= size of g
    if (* hSize > * gSize)
    {
        fprintf(stderr, ERROR_MSG);
        return EXIT_FAILURE;
    }
    //now we have 2 valid arrays.

    /* try to get n - non negative int */
    if (!createPositiveInt(pN))
    {
        fprintf(stderr, ERROR_MSG);
        return EXIT_FAILURE;
    }


    /*centralize arrays:*/
    centralizeArray(gArray, ARRAY_MAX_SIZE, g);
    centralizeArray(hArray, ARRAY_MAX_SIZE, h);

    /*normalize arrays:*/
    normalizeArray(gArray, ARRAY_MAX_SIZE);
    normalizeArray(hArray, ARRAY_MAX_SIZE);

    /*convolution*/
    convolution(gArray, hArray, resultArray, ARRAY_MAX_SIZE, n);

    /* out put*/
    /* if n=0 print gArray, otherwise - print resultArray*/
    if (n == 0)
    {
        outputHandler(gArray, ARRAY_MAX_SIZE);
    }
    else
    {
        outputHandler(resultArray, ARRAY_MAX_SIZE);
    }


    return EXIT_SUCCESS;


}
