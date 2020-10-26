/**
 * @file railwayPlanner.c
 * @author  maayan sharon
 * @version 1.0
 * @date 8 Aug 2020
 *
 * @brief System to find min railway path
 *
 * @section LICENSE
 * This program is part of school ex
 *
 * @section DESCRIPTION
 * System to find min railway path
 * Input  : files with relevent data (plugs, rail parts, length)
 * Process: read the file, calculates the min rail and writes to output file.
 *          deals with problems with file or data
 * Output : output file with relevant msg
 */

// ------------------------------ includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <limits.h>

// -------------------------- const definitions -------------------------
/**
 * @def DELIMITERS ","
 * @brief the signal which split the line
 */
#define DELIMITERS ","
/**
 * @def END_OF_STRING '\0'
 * @brief end of string char
 */
#define END_OF_STRING '\0'
/**
 * @def END_OF_LINE '\n'
 * @brief end of line char
 */
#define END_OF_LINE '\n'
/**
 * @def END_CHAR '9'
 * @brief asccii number of the last number
 */
#define END_CHAR '9'
/**
 * @def START_CHAR '9'
 * @brief asccii number of the first number
 */
#define START_CHAR '0'
/**
 * @def INVALID_NUM_ARGS "Usage: RailwayPlanner <InputFilePath>"
 * @brief msg to file
 */
#define INVALID_NUM_ARGS "Usage: RailwayPlanner <InputFilePath>"
/**
 * @def FILE_DOES_NOT_EXIST "File does not exist. "
 * @brief msg to file
 */
#define FILE_DOES_NOT_EXIST "File does not exist. "
/**
 * @def FILE_EMPTY "File is empty."
 * @brief msg to file
 */
#define FILE_EMPTY "File is empty."
/**
 * @def FILE_ERROR_IN_LINE "Invalid input in line: %d."
 * @brief msg to file
 */
#define FILE_ERROR_IN_LINE "Invalid input in line: %d."
/**
 * @def FILE_MIN_VAL_MSG "The minimal price is: %d."
 * @brief msg to file
 */
#define FILE_MIN_VAL_MSG "The minimal price is: %d."
/**
 * @def FALSE 0
 * @brief false
 */
#define FALSE 0
/**
 * @def TRUE 1
 * @brief true
 */
#define TRUE 1
/**
 * @def ONE 1
 * @brief 1
 */
#define ONE 1
/**
 * @def BAD_PLUG -1
 * @brief plag is not valis
 */
#define BAD_PLUG -1
/**
 * @def MAX_LINE_LENGTH 1025
 * @brief max line length 1024 + '\0' char
 */
#define MAX_LINE_LENGTH 1025
/**
 * @def  VAlID_NUM_OF_ARGS 2
 * @brief valid num of args
 */
#define VALID_NUM_OF_ARGS 2
/**
 * @def PLUG_SIZE 1
 * @brief valid plug size
 */
#define PLUG_SIZE 1
/**
 * @def PLUG_SIZE_RAW 2
 * @brief size of last plag in line
 */
#define PLUG_SIZE_RAW 2
/**
 * @def INITIAL_CAPACITY 5
 * @brief INITIAL_CAPACITY for memory allocation
 */
#define INITIAL_CAPACITY 5
/**
 * @def READ_MODE "r"
 * @brief read mode
 */
#define READ_MODE "r"
/**
 * @def BASE 10
 * @brief base to convert or update capacity
 */
#define BASE 10

// ------------------------------ functions -----------------------------

/* ------------------------ STRUCTS  --------------- */
/**
 * @struct railPart
 * @brief this struct represents a possible railpart of the railway
 * @par e - end plug (right)
 * @par s - start plug (start)
 * @par p - price
 * @par d - distance
 */
typedef struct railPart
{
    int e; //end plug (right)
    int s; //start plug (start)
    int p; //price
    int d; //distance

} railPart;

/* ------------------------ VALIDATION FUNCTION --------------- */


/**
 * @brief check if line is digit
 * @param lineInput
 * @return 1 if line is digit, 0 otherwise
 */
int checkLineIsDigit(const char *lineInput)
{
    for (int i = 0; i < MAX_LINE_LENGTH; i++)
    {
        if (lineInput[i] == END_OF_STRING || lineInput[i] == END_OF_LINE)
        {
            break;
        }
        if ((lineInput[i] > END_CHAR || lineInput[i] < START_CHAR))
        {

            return FALSE;
        }
    }
    return TRUE;
}

/**
 * @brief if argc is correct - return true. else - print error into file and close it.
 * @param argc
 * @param outPtr
 * @return
 */
int validateNumArgs(int argc, FILE *outPtr)
{
    if (argc != VALID_NUM_OF_ARGS)
    {
        fprintf(outPtr, INVALID_NUM_ARGS);
        fclose(outPtr);
        return FALSE;
    }
    return TRUE;
}

/**
 * @brief if rawPlug is valid - return it's place in pluglist, else return -1.
 * helper function to parseRailParts
 * @param plugRaw
 * @param plugList
 * @param plugListNum
 * @return rawPlug place in plugList upon success, -1 otherwise
 */
int validatePlug(char *plugRaw, const char *plugList, int plugListNum)
{
    if ((strlen(plugRaw) == PLUG_SIZE_RAW && plugRaw[TRUE] == END_OF_LINE) || strlen(plugRaw) == PLUG_SIZE)
    {
        for (int i = 0; i < plugListNum; i++)
        {
            if (plugRaw[0] == plugList[i])
            {
                return i;
            }
        }
    }
    return BAD_PLUG;
}


/* ------------------------ BUILD TABLE ------------------------ */
/**
 * @brief allocate memory for 2d table on heap. if falils frees all internal allocations
 * @param rows
 * @param cols
 * @return 1 upon success, 0 otherwise
 */
int **allocTable(int rows, int cols)
{
    int **table = (int **) calloc(rows, sizeof(int *));
    if (table == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < rows; i++)
    {
        table[i] = (int *) calloc(cols, sizeof(int));
        if (table[i] == NULL)
        {
            for (int k = 0; k < i; k++)
            {
                free(table[k]);
                table[k] = NULL;
            }
            free(table);
            table = NULL;
            return NULL;
        }
    }
    return table;
}


/* ------------------------ FREE FUNCTIONS --------------- */
/**
 * @brief prolblem with plugs was ditected. frees pluglist, print msg to outfile and closes files,
 * @param outPtr
 * @param inPtr
 * @param lineCount
 * @param plugList
 */
void freeProblemWithPlugs(FILE *outPtr, FILE *inPtr, const int *lineCount, char **plugList)
{
    free(*plugList);
    *plugList = NULL;
    fprintf(outPtr, FILE_ERROR_IN_LINE, *lineCount);
    fclose(outPtr);
    fclose(inPtr);
}

/**
 * @brief there was a problem with parsing one of the parts. free relevant fields and close files
 * @param outPtr
 * @param inPtr
 * @param partsList
 * @param lineCount
 */
void freeProblemWithPart(FILE *outPtr, FILE *inPtr, railPart **partsList, const int *lineCount)
{
    free(*partsList);
    *partsList = NULL;
    fprintf(outPtr, FILE_ERROR_IN_LINE, *lineCount);
    fclose(outPtr);
    fclose(inPtr);
}

/**
 * @brief program finished. free everything left, closes files
 * @param outPtr
 * @param inPtr
 * @param railLength
 * @param plugList
 * @param partsList
 * @param lineCount
 * @param table
 */
void freeAtTheEnd(FILE *outPtr, FILE *inPtr, int railLength, char *plugList, railPart *partsList, int **table)
{
    if (plugList != NULL)
    {
        free(plugList);
        plugList = NULL;
    }
    //free table
    for (int x = 0; x < railLength + 1; x++)
    {
        free(table[x]);
        table[x] = NULL;
    }
    free(table);
    table = NULL;
    //free everything - railPartList, plugList, table
    if (partsList != NULL)
    {
        free(partsList);
        partsList = NULL;
    }
    //close files
    fclose(outPtr);
    fclose(inPtr);
}

/**
 * @brief problem ditected while allocating memory to table. frees relevet fields and closes files
 * @param outPtr
 * @param inPtr
 * @param lineCount
 * @param plugList
 * @param partsList
 */
void freeProblemWithTableAlloc(FILE *outPtr, FILE *inPtr, int lineCount, char *plugList, railPart *partsList)
{
    free(plugList);
    plugList = NULL;
    free(partsList);
    partsList = NULL;
    fprintf(outPtr, FILE_ERROR_IN_LINE, lineCount);
    fclose(outPtr);
    fclose(inPtr);
}


/* ------------------------ PARSING FUNCTIONS --------------- */
/**
 * @brief parse the first line in 'inFile'. update linecount and railLength upon success. closes files upon otherrise
 * @param outPtr
 * @param inPtr
 * @param lineCount
 * @param buffer
 * @param railLength
 * @return 1 (true) upon sucess, 0 (false) otherwise.
 */
int parseFirstLine(FILE *outPtr, FILE *inPtr, int *lineCount, char *buffer, int *railLength)
{
    char *firstLine = fgets(buffer, MAX_LINE_LENGTH, inPtr);
    if (firstLine == NULL)
    {
        fprintf(outPtr, FILE_EMPTY);
        fclose(inPtr);
        fclose(outPtr);

        return FALSE;

    }
    if (!checkLineIsDigit(buffer))
    {
        fprintf(outPtr, FILE_ERROR_IN_LINE, *lineCount);
        fclose(outPtr);
        fclose(inPtr);
        return FALSE;
    }
    *railLength = (int) strtol(buffer, NULL, BASE);
    if (*railLength <= FALSE)
    {
        fprintf(outPtr, FILE_ERROR_IN_LINE, *lineCount);
        fclose(outPtr);
        fclose(inPtr);
        return FALSE;
    }
    (*lineCount)++;
    return TRUE;
}

/**
 * @brief parse second line of the input file.
 * update plug list (allocate memory) and numPlugs. update line counter
 * handles errors if needed
 * @param outPtr
 * @param inPtr
 * @param lineCount
 * @param buffer
 * @param numPlugs
 * @param plugList
 * @return 1 upon success, 0 otherwise
 */
int parseSecondLine(FILE *outPtr, FILE *inPtr, int *lineCount, char *buffer, int *numPlugs, char **plugList)
{
    int numPlugsCapacity = INITIAL_CAPACITY;
    int plugsCounter = 0;
    fgets(buffer, MAX_LINE_LENGTH, inPtr);
    *plugList = (char *) calloc(numPlugsCapacity, sizeof(char));
    if (*plugList == NULL)
    {
        fclose(outPtr);
        fclose(inPtr);
        return FALSE;
    }
    char *plug = strtok(buffer, DELIMITERS); // split by ','
    while (plug != NULL)
    {  // go over all possible plugs
        if (plugsCounter >= numPlugsCapacity)
        {
            numPlugsCapacity = numPlugsCapacity + BASE;
            char *valid = (char *) realloc(*plugList, numPlugsCapacity);
            if (valid == NULL)
            {
                freeProblemWithPlugs(outPtr, inPtr, lineCount, plugList);
                return FALSE;
            }
            *plugList = valid;
        }
        if ((strlen(plug) == PLUG_SIZE_RAW && plug[TRUE] == END_OF_LINE) || strlen(plug) == PLUG_SIZE)
        {
            (*plugList)[plugsCounter] = *plug;
            plug = strtok(NULL, DELIMITERS);
            plugsCounter++;
        }
        else
        {
            freeProblemWithPlugs(outPtr, inPtr, lineCount, plugList);
            return FALSE;
        }
    }
    if (plugsCounter < numPlugsCapacity)  //reduce size if needed at the end
    {
        numPlugsCapacity = plugsCounter;
        char *valid = (char *) realloc(*plugList, numPlugsCapacity);
        if (valid == NULL)
        {
            freeProblemWithPlugs(outPtr, inPtr, lineCount, plugList);
            return FALSE;
        } // realloc was successful
        *plugList = valid;
    }
    (*numPlugs) = plugsCounter;
    (*lineCount)++;
    return TRUE;
}

/**
 * @brief parsing rail parts lines from file. create and update rail part list. upon success return 1,
 * other wise handles errors and return 0
 * @param outPtr
 * @param inPtr
 * @param plugList
 * @param PartsList
 * @param numPlugs
 * @return upon success return 1, 0 other wise
 */
int parseRailParts(FILE *outPtr, FILE *inPtr, const char *plugList, int numPlugs, railPart **partsList,
                   int *partsCounterR, int *lineCount)
{
    int partsCounter = 0;
    int capacity = INITIAL_CAPACITY;
    char buffer[MAX_LINE_LENGTH];
    *partsList = (railPart *) calloc(INITIAL_CAPACITY, sizeof(railPart));
    while (fgets(buffer, MAX_LINE_LENGTH, inPtr))  // for each railPart:
    {
        if (partsCounter >= capacity)
        {
            capacity = capacity + BASE;
            railPart *valid = (railPart *) realloc(*partsList, sizeof(railPart) * capacity);
            if (valid == NULL)
            {
                freeProblemWithPart(outPtr, inPtr, partsList, lineCount);
                return FALSE;
            }
            *partsList = valid;
        }
        char *startPlugRaw = strtok(buffer, DELIMITERS); // 1st token split by ','
        char *endPlugRaw = strtok(NULL, DELIMITERS); // 2nd token split by ','
        char *lengthRaw = strtok(NULL, DELIMITERS); // 3rd token split by ','
        char *priceRaw = strtok(NULL, DELIMITERS); // 4th token split by ','
        int price = FALSE;
        int length = FALSE;
        // check each param is valid
        int startPlug = validatePlug(startPlugRaw, plugList, numPlugs);
        int endPlug = validatePlug(endPlugRaw, plugList, numPlugs);
        int isLenValid = checkLineIsDigit(lengthRaw);
        int isPriceValid = checkLineIsDigit(priceRaw);

        if (isLenValid && isPriceValid)
        {
            price = (int) strtol(priceRaw, NULL, BASE);
            length = (int) strtol(lengthRaw, NULL, BASE);
        }
        // if part is not valid - write to output free railPartsList and return false
        if (price <= FALSE || length <= FALSE || startPlug == BAD_PLUG || endPlug == BAD_PLUG)
        {
            freeProblemWithPart(outPtr, inPtr, partsList, lineCount);
            return FALSE;
        }

        (*partsList)[partsCounter].p = price;
        (*partsList)[partsCounter].d = length;
        (*partsList)[partsCounter].e = endPlug;
        (*partsList)[partsCounter].s = startPlug;

        partsCounter++;
        (*lineCount)++;
    }
    *partsCounterR = partsCounter;
    return TRUE;
}


/**
 * @brief claculare the minimum result of a given rail in table up until row l and col k
 * @param partsList
 * @param partsListLen
 * @param table
 * @param l
 * @param k
 * @param curMinRes
 * @return the min result
 */
int clacMinRes(const railPart *partsList, int partsListLen, int *const *table, int l, int k, int curMinRes)
{
    for (int i = 0; i < partsListLen; i++)
    {
        // if the distance left is not negative and it id the right end plug
        if ((l - partsList[i].d >= 0) && (partsList[i].e == k))
        {
            if (INT_MAX - partsList[i].p >= table[l - partsList[i].d][partsList[i].s])
            {
                int curPrice = partsList[i].p + table[l - partsList[i].d][partsList[i].s];
                if (curPrice < curMinRes)
                {
                    curMinRes = curPrice;
                }
            }
        }
    }
    return curMinRes;
}

/**
 * @brief fills in the given table with results according to given algorithm
 * @param railLength
 * @param numPlugs
 * @param partsList
 * @param partsListLen
 * @param table
 */
void fillTableValues(int railLength, int numPlugs, const railPart *partsList, int partsListLen, int *const *table)
{

    for (int l = 1; l < railLength + ONE; l++)
    {
        for (int k = 0; k < numPlugs; k++)
        {
            int curMinRes = INT_MAX;
            //calcMinResForCell();
            table[l][k] = clacMinRes(partsList, partsListLen, table, l, k, curMinRes);
        }
    }
}


/**
 * @brief go over the last line in table and return the min value
 * @param railLength
 * @param numPlugs
 * @param table
 * @return min val
 */
int findMinVal(int railLength, int numPlugs, int *const *table)
{
    int minVal = INT_MAX;
    for (int i = 0; i < numPlugs; i++)
    {
        if (table[railLength][i] < minVal)
        {
            minVal = table[railLength][i];
        }
    }
    if (minVal == INT_MAX)
    {
        minVal = BAD_PLUG;
    }
    return minVal;
}

/* ------------------------ MAIN ------------------------ */
/**
 * @brief main function of railway planner program.
 * @param argc
 * @param argv
 * @return 0 upon success, 1 otherwise
 */
int main(int argc, char **argv)
{
    /* ------------------------ open input and output file if not valid exit --------------- */
    /* open in and out files, if there're errors, handle them. */
    FILE *outPtr = NULL;
    outPtr = fopen("rwp_output.out", "w+");

    /* fopen() return NULL if last operation was unsuccessful */
    if (outPtr == NULL)
    {
        /* File not created hence exit */
        return EXIT_FAILURE;
    }
    /* check num of args is correct - handle errors and close file */
    if(!validateNumArgs(argc, outPtr))
    {
        return EXIT_FAILURE;
    }

    FILE *inPtr = NULL;

    /* 1) open input file and handle errors if needed
     * 2) check that input file is not empty and handle errors if needed */

    inPtr = fopen(argv[TRUE], READ_MODE);
    // if input file doesnot exists
    if (inPtr == NULL)
    {
        fprintf(outPtr, FILE_DOES_NOT_EXIST);
        fclose(outPtr);
        return FALSE;
    }

    /* ------------------------ parse first line ------------------------ */
    int lineCount = 1;
    int railLength; // L in instructions
    char buffer[MAX_LINE_LENGTH] = {0};
    // parse first line
    if (!parseFirstLine(outPtr, inPtr, &lineCount, buffer, &railLength))
    {
        return EXIT_FAILURE;
    }
    /* ------------------------ parse second line ------------------------ */

    int numPlugs = 0; // k in instructions
    char *plugList = NULL;
    if (!parseSecondLine(outPtr, inPtr, &lineCount, buffer, &numPlugs, &plugList))
    {
        return EXIT_FAILURE;
    }

    /* ------------------------ parse third + lines ------------------------ */
    railPart *partsList = NULL;
    int partsListLen = 0;
    if (!parseRailParts(outPtr, inPtr, plugList, numPlugs, &partsList, &partsListLen, &lineCount))
    {
        free(plugList);
//        plugList = NULL;
        return EXIT_FAILURE;
    }
//    printRailPartsList(partsList,partsListLen);

    /* ------------------------ create table ------------------------ */
    //partsList is ready!! start build the table
    int **table = allocTable(railLength + 1, numPlugs);
    if (table == NULL)
    {
        freeProblemWithTableAlloc(outPtr, inPtr, lineCount, plugList, partsList);
        return EXIT_FAILURE;
    }
//    printTable(railLength,numPlugs,table);

    // fill in the table according to the given algo
    fillTableValues(railLength, numPlugs, partsList, partsListLen, table);
//    printTable(railLength,numPlugs,table);

    /* ------------------------ find min val, write to file and exit ------------------------ */
    // go over the last line and return the min value
    int minVal = findMinVal(railLength, numPlugs, table);
    // write the min value into the outfile.
    fprintf(outPtr, FILE_MIN_VAL_MSG, minVal);
    freeAtTheEnd(outPtr, inPtr, railLength, plugList, partsList, table);
    return EXIT_SUCCESS;
}

