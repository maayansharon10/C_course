#include <stdio.h>
#include <stdlib.h>
//#include <zconf.h>
#include <math.h>
#include <string.h>
#include "queue.h"

/**
 * @brief the valid number of arguments the program expects.
 */
#define VAID_NUM_OF_ARGS 4

/**
 * @brief If function failed
 */
#define FAIL 0

/**
 * @brief If function failed
 */
#define SUCCESS 1

/**
 * @brief Defines the maximum line length.
 */
#define LINE_MAX_SIZE 1024

/**
 * @brief The delimiters used to separate the input
 */
#define INPUT_DELIMS " \t\r\n"

/**
 * @brief error of type invalid input
 */
#define INVALID_INPUT_ERROR 2

/**
 * @brief value of error of type not a tree
 */
#define NOT_A_TREE_ERROR_VAL 1

/**
 * @brief error to print if graph is not a tree
 */
#define NOT_A_TREE_ERROR_PRINT "The given graph is not a tree\n"



/**
 * @brief error to print if usage is wrong (wrong args)
 */
#define UNVALID_ARGS_ERROR_PRINT "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n"


/**
 * @brief error of type wrong usage (wrong args)
 */
#define UNVALID_ARGS_ERROR_VAL 3

/**
 * @brief the gap between the counter to the node's value this line represents
 */
#define LINE_COUNTER_GAP 2



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ node related methods ~~~~~~~~~~~~~~~~~~~~~~~~~~
/**
 * struct of type node.
 */
typedef struct TreeNode
{
    int data;
    int numChildren;
    int * children;
    int father;
    int visited;
    int distance;
    int prev;

} Node;

/**
 * initialize all params of a single node to deafult values
 * @param curNode
 * @param nodeData
 */
void initializeNode(Node * curNode, int nodeData)
{
    //initialize node's elements
    curNode->data = nodeData;
    curNode->numChildren = 0;
    curNode->children = NULL;
    curNode->father = -1;
    curNode->visited = 0;
    curNode->distance = -1;
    curNode->prev = -1;

}

/**
 * func copying the children array to a node's children array
 * @param node
 * @param childrenArray
 * @param numChildren
 */
void copyChildArray(Node * node, const int * childrenArray, int numChildren)
{
    for (int i = 0; i < numChildren; i++)
    {
        node->children[i] = childrenArray[i];
    }
}

/**
 * crearing a new node, allocating memory but doesn't free!
 * @param nodeData
 * @param numChildren
 * @param childrenArray
 * @return
 */
Node * createNewNode(int nodeData, int numChildren, int * childrenArray)
{
    Node * newNode = (Node *) malloc(sizeof(Node));
    initializeNode(newNode, nodeData);
    if (numChildren > 0)
    {
        newNode->numChildren = numChildren;
        newNode->children = (int *) malloc(sizeof(int) * numChildren);
        copyChildArray(newNode, childrenArray, numChildren);
    }
    return newNode;

}

/**
 * free a single node's fields.
 * @param node
 */
void freeNode(Node * node)
{
    free(node->children);
    node->children = NULL;

}

/**
 * check if a single char is a digit
 * @param chr
 * @return return 1 in case it is a char, 0 if not a char
 */
int isChrDigit(char chr)
{
    int number = chr - '0';
/* if it is valid - continue loop, and if not, return failure */
    if (number < 0 || 9 < number)
    {
        return FAIL; //not a digit
    } // chr is a digit -
    return SUCCESS;
}

/**
 * check if args are valid, and if so, update node1Data and node2Data accordignly
 * @param argc
 * @param argv
 * @param node1Data
 * @param node2Data
 * @return 0 if fails, 1 if succeed.
 */
int validateArgs(int argc, char ** argv, int * node1Data, int * node2Data, int * errorArgs)
{
    if (argc != VAID_NUM_OF_ARGS)
    {
        * errorArgs = UNVALID_ARGS_ERROR_VAL; // invalid args
        return FAIL;
    }
    // check 3rd and 4th are actually ints, and convert to int
    // check node 1 which is arg[2] is in fact an number
    if (!isChrDigit(argv[2][0]) || !(isChrDigit(argv[3][0])))
    {
        * errorArgs = INVALID_INPUT_ERROR;
        return FAIL;
    }

    char * ptr1;
    char * ptr2;
    double node1Double = strtod(argv[2], & ptr1);
    double node2Double = strtod(argv[3], & ptr2);

    if ((strcmp(ptr1, "") != 0) || (strcmp(ptr2, "") != 0))
    {
        * errorArgs = INVALID_INPUT_ERROR;
        return FAIL;
    }

    // check double is an int, if is in - v
    if (((int)(node1Double) != node1Double) || ((int)(node2Double) != node2Double))
    {
        * errorArgs = INVALID_INPUT_ERROR; // invalid input
        return FAIL;
    }

    * node1Data = (int) (node1Double);
    * node2Data = (int) (node2Double);

    return SUCCESS;
}


/**
 * read and process first line, filter it and return the valid number in case of success, 0 if failed
 * @param filePointer
 * @param node1 given in command line
 * @param node2 given in command line
 * @return valid number is succeded, 0 if failed
 */
int readAndProcessFirstLine(FILE * filePointer, int node1, int node2)
{
    // if there's a problem - return 0, if succeded - return a valid positive int
    char line[LINE_MAX_SIZE];
    int nVal = 0;
    int * n = & nVal;
    char * nAsDouble;
    char * anotherAsDouble;
    // ~~~~~~~~~~~~~dealing with first line ~~~~~~~~~~~~~~~~~~~~~~~
    // if DOES NOT exists, should be a valid int nAsDouble.

    if (fgets(line, sizeof line, filePointer) == NULL)
    {
        return FAIL;
    }
    // first line does exists, make sure it's an int

    /* Now "line" contains the contents of the entire line, including the suffix "\n",
    Lets read the first nAsDouble. If there's no input, we'll get "NULL" */
    nAsDouble = strtok(line, INPUT_DELIMS);

    char * restOfVal;
    double valueAsDouble = strtod(nAsDouble, & restOfVal);
    //check there's only a number in input
    if (strcmp(restOfVal, "") != FAIL)
    {
        return FAIL;
    }
    // check given number is an int
    if ((int)(valueAsDouble) != valueAsDouble)
    {
        return FAIL;
    }
    // assign the given num
    * n = (int) (valueAsDouble);

    // check 2 given nodes val should be < = n-1
    if ((node1 >= * n) || (node2 >= * n))
    {
        return FAIL;
    }

    /* Read the next value in line, if there's one - input is invalid */
    anotherAsDouble = strtok(NULL, INPUT_DELIMS);
    if (anotherAsDouble != NULL)
    {
        return FAIL;
    }

    // line is valid! return it as int
    return * n;

}


/**
 * helper function, sits inside a for loop. check if value[i] is valid
 * @param i
 * @param valAsDouble
 * @param value
 * @param tempChildrenArray
 * @param valCounter
 * @param error
 * @return  2 if it's c '-' and loop should break, 0 if fails, 1 is success
 */
int isValueValid(int i, double valAsDouble, char *value, int *tempChildrenArray, int *valCounter, int *error)
{
    if (i == 0)
    {

        if ((valAsDouble == 0) && (* value == '-') && (strlen(value) == 1))
        {
            char * nextValue = strtok(NULL, INPUT_DELIMS);
            if (nextValue == NULL)
            {
                tempChildrenArray[*valCounter] = 0;
                *valCounter = -1;
                return 2; // do break, '-' case
            }
            else
            {
                * error = INVALID_INPUT_ERROR;
                return FAIL;
            }

        }

        // check value doesn't start with zero
        if ((*value != '-') && ( (!isChrDigit(value[i])) || ((value[i] == '0') && (strlen(value) != 1)) ) )
        {
            //input is not valid!
            * error = INVALID_INPUT_ERROR;
            return FAIL;
        }
        if( (* value == '-' ) && (strlen(value) > 1))
        {
            * error = INVALID_INPUT_ERROR;
            return FAIL;
        }
    }
    else
    {
        if (!isChrDigit(value[i]))
        {
            //input is not valid!
            * error = INVALID_INPUT_ERROR;
            return FAIL;
        }
    }
    return SUCCESS;
}

/**
 * process one line and in case valid, update the temp children array and num children accordingly.
 * update error in case needed.
 * @param line
 * @param tempChildrenArray
 * @param maxNumOfNodes
 * @param numChildren
 * @param numEdges
 * @param error
 * @return 1 is succeded, 0 if failed
 */
int processLine(char * line, int * tempChildrenArray, const int maxNumOfNodes,
                int * numChildren, int * numEdges, int * error)
{
    // for each value in line, check it is a valid int, if so - put in childrenArray, else -
    // return fail

    char * value = NULL;
    value = strtok(line, INPUT_DELIMS);

    if (value == NULL)
    {
        * error = INVALID_INPUT_ERROR; // in valid input if line is empty
        return FAIL;
    }

    int valCounter = 0;


    /* Handle every value and read the next value */
    while (value != NULL)
    {

        //if num of nodes in line exceeded the max num of nodes - return
        if (valCounter >= maxNumOfNodes)
        {
            * error = INVALID_INPUT_ERROR;
            return FAIL;
        }

        char * restOfVal;
        double valAsDouble = strtod(value, & restOfVal);

        size_t length = strlen(value);
        // check value is indeed a valid int
        for (int i = 0; i < (int) length; i++)
        {
            int valueValid = isValueValid(i, valAsDouble, value, tempChildrenArray, &valCounter, error);
            if(valueValid == FAIL)
            {
                return FAIL;
            }
            else if (valueValid == 2)
            {
                break;
            }
        } // for sure value is a valid int

        if (valAsDouble > maxNumOfNodes)
        {
            * error = NOT_A_TREE_ERROR_VAL;
            return FAIL;
        }
        //convert to int and put in children array
        if(valCounter<maxNumOfNodes)
        {
            tempChildrenArray[valCounter] = (int) (valAsDouble);

        }
        /* Read the next value */
        value = strtok(NULL, INPUT_DELIMS);
        valCounter++;
    }

    //update num Children
    * numChildren = valCounter;
    * numEdges = valCounter;

    return SUCCESS;
}

/**
 * update node's father
 * @param nodeArray
 * @param i
 * @param shouldExist
 */
void updateFather(Node **nodeArray, int i, int *shouldExist)
{
    if (nodeArray[i]->numChildren > 0)
    {
        // go over all of the node's children -
        for (int j = 0; j < nodeArray[i]->numChildren; j++)
        {
            // if child already has a father -
            if (nodeArray[nodeArray[i]->children[j]]->father >= 0)
            {
                // too many fathers , so not a tree
                *shouldExist = NOT_A_TREE_ERROR_VAL;
            }
            else
            {
                //doesn't have a father - update father
                nodeArray[nodeArray[i]->children[j]]->father = nodeArray[i]->data;
            }
        }
    }
}


/**
 * reading file from 2nd line and on. process, update error pointer if needed, nodeArray, actual num of nodes and num
 * edges. update each node's father if valid.
 * @param filePointer
 * @param nodeArray
 * @param maxNumNodes
 * @param actualNumOfNodes
 * @param numEdges
 * @param error
 * @return 1 if succed, 0 otherwise
 */
int readAndProcessFileToNodeArray(FILE * filePointer, Node ** nodeArray, int maxNumNodes, int * actualNumOfNodes,
                                  int * numEdges, int * error)
{
    // ~~~~~~~~~~~~~dealing with all other lines ~~~~~~~~~~~~~~~~~~~~~~~
    //goal of this function - read input and place it in node array. if input is wrong- return false
    char line[LINE_MAX_SIZE];
    //int lineCounterVal = 2;
    int lineCounter = LINE_COUNTER_GAP;
    int shouldExist = FAIL;

    while (fgets(line, LINE_MAX_SIZE, filePointer) != NULL)
    {
        int * tempChildrenArray;
        int numChildrenVal = 0;
        int * numChildren = & numChildrenVal;

        // if there are more nodes than max amount
        if (lineCounter - LINE_COUNTER_GAP > maxNumNodes)
        {
            // should exit - invalid input
            shouldExist = INVALID_INPUT_ERROR;
            break;
        }


        tempChildrenArray = (int *) malloc(sizeof(int) * maxNumNodes);
        //initiallize all children to be -1
        // process line - if fails - free and return
        if (!processLine(line, tempChildrenArray, maxNumNodes, numChildren, numEdges, & shouldExist))
        {
            free(tempChildrenArray);
            // shouldExist is updated through the method
            break;
        }
        // line processed correctly :
        int curNodeVal = lineCounter - LINE_COUNTER_GAP;
        //check the node is not it's own child
        for (int j = 0; j < * numChildren; j++)
        {
            // the node is also a child - of node points to itself
            if (tempChildrenArray[j] == curNodeVal)
            {
                shouldExist = NOT_A_TREE_ERROR_VAL;

            }
        }

        // create new node and place in node array
        nodeArray[curNodeVal] = createNewNode(curNodeVal, * numChildren, tempChildrenArray);
        free(tempChildrenArray);
        lineCounter++;
    }


    int numOfNodes = lineCounter - LINE_COUNTER_GAP;
    //if the num of nodes doesnot match the number of nodes declared in first line - invalid input!
    if (numOfNodes != maxNumNodes)
    {
        shouldExist = INVALID_INPUT_ERROR;
    }
    // check who are the fathers of these nodes
    if (!shouldExist)
    {
        for (int i = 0; i < (numOfNodes); i++)
        {
            updateFather(nodeArray, i, &shouldExist);
        }
    }
    if (shouldExist)
    {
        // there was a problem parsing the line
        for (int i = 0; i < (numOfNodes); i++)
        {
            freeNode(nodeArray[i]);

        }
        * error = shouldExist;
        * actualNumOfNodes = numOfNodes;
        return FAIL;
    }
    // else - all other was good
    * actualNumOfNodes = numOfNodes;
    return SUCCESS;

}

/**
 * check graph can be a tree according to number of roots.
 * return 0 if there are more then one root, and 1 if there is only one root.
 * @param nodeArray
 * @param arraySize
 * @return 0 if there are more then one root, and 1 if there is only one root.
 */
int rootsNumValidation(Node ** nodeArray, const int arraySize, int * rootNode)
{
    int oneFather = FAIL; //true
    int rootNodeTemp = -1;

    // check if there are more then one node with no father
    // in other words - check if there are more than one 0 in the father's array
    for (int i = 0; i < arraySize; i++)
    {
        if (nodeArray[i]->father == -1) //meaning there is no father
        {
            oneFather += SUCCESS;
            rootNodeTemp = i;
        }
    }
    if (oneFather != SUCCESS)
    {
        // there is more then one node with no father, or there's no node with no father
        // - graph is not a tree
        return FAIL;
    }
    * rootNode = rootNodeTemp;
    return SUCCESS;
}

/**
 * before running dfs this method reset all relevent fields.
 * @param nodeArray
 * @param nodeArraySize
 */
void resetParamsForDFS(Node *** nodeArray, int nodeArraySize)
{
    for (int i = 0; i < nodeArraySize; i++)
    {
        (* nodeArray)[i]->visited = FAIL;
    }
}

/**
 * check if the graph is can be tree. if graph has a circle, return 1, if doesn't have a circle -
 * return 0
 * @param nodeArray
 * @param root
 * @return if succedded in finding a circle, return 1, if didn't succeed (doesn't have a circle) -
 * return 0
 */
int exploreDFS(Node *** nodeArray, Node * root)
{
    int hasCircle = FAIL; // value is used in recursion
    // mark root as visited.
    root->visited = SUCCESS;
    // for each one of root's children -
    for (int i = 0; i < root->numChildren; i++)
    {
        Node * child = (* nodeArray)[root->children[i]];
        if (child->visited == FAIL) // if one of the root's children are not
            // visited
        {
            hasCircle = exploreDFS(nodeArray, child);
            if (hasCircle == SUCCESS)
            {
                return SUCCESS;
            }
        }
        else
        {
            return SUCCESS;
        }
    }
    return FAIL;

}

/**
 * before running bfs, this method reset all relevent feilds
 * @param nodeArray
 * @param nodeArraySize
 */
void resetParamForBFS(Node ** nodeArray, int nodeArraySize)
{

    // initialize params for bfs
    // distance array is initialize -1
    // prev array is initialize -1
    for (int i = 0; i < nodeArraySize; i++)
    {
        nodeArray[i]->distance = -1;
        nodeArray[i]->prev = -1;
    }
}

/**
 *  * return the last node from the queue. update distance and prev of each node in nodeArray
 * @param startNode
 * @param nodeArray
 * @param nodeArraySize
 * @return the last node from the queue
 */
int runBFS(int startNode, Node ** nodeArray, int nodeArraySize)
{
    // initialize params for bfs (prev, dis = -1)
    resetParamForBFS(nodeArray, nodeArraySize);
    int lastNode = 0;

    // for each node in distance array - mark node dist = -1
    // mark start node's dist as 0 and
    nodeArray[startNode]->distance = 0;
    nodeArray[startNode]->prev = -1;

    Queue * bfsQueue = allocQueue();
    enqueue(bfsQueue, (unsigned int) startNode);

    while (queueIsEmpty(bfsQueue) != SUCCESS) // while queue is not empty
    {
        lastNode = (int) peekQueue(bfsQueue);

        int currentNode = (int) dequeue(bfsQueue);
        for (int i = 0; i < nodeArray[currentNode]->numChildren; i++)
        {
            //check if child 's distance is -1. if so - enqueue
            if (nodeArray[nodeArray[currentNode]->children[i]]->distance == -1)
            {
                //enqueue child
                enqueue(bfsQueue, (unsigned int) nodeArray[currentNode]->children[i]);
                // update's child's prev
                nodeArray[nodeArray[currentNode]->children[i]]->prev = currentNode;
                // update child's distance to be prev dist +1
                nodeArray[nodeArray[currentNode]->children[i]]->distance = nodeArray[currentNode]
                                                                                   ->distance + 1;
            }
        }
        int father = nodeArray[currentNode]->father;
        if (father != -1)
        {
            //check if father's distance is -1. if so - enqueue
            if (nodeArray[father]->distance == -1)
            {
                //enqueue child
                enqueue(bfsQueue, (unsigned int) nodeArray[father]->data);
                // update's child's prev
                nodeArray[father]->prev = currentNode;
                // update child's distance to be prev dist +1
                nodeArray[father]->distance = nodeArray[currentNode]->distance + 1;
            }
        }
    }
    free(bfsQueue);
    return lastNode;
}

/**
 * return the minimal branch of the tree ( shortest distance between root and a leaf).
 * @param rootVal
 * @param nodeArray
 * @param nodeArraySize
 * @return return the minimal branch of the tree ( shortest distance between root and a leaf).
 */
int getLengthMininalBranch(int rootVal, Node ** nodeArray, int nodeArraySize)
{
    // length of minimal branch -
    // -> bfs with root..
    runBFS(rootVal, nodeArray, nodeArraySize);

    //going over all distances, searching for a minumum dis which is also a leaf
    int minNode = 0;
    // initial first minimal branch as one of the leafs
    for (int j = 0; j < nodeArraySize; j++)
    {
        if (nodeArray[j]->numChildren == FAIL)
        {
            minNode = j;
            break;
        }
    }

    // now search for a shorter minimal branch
    for (int i = 0; i < nodeArraySize; i++)
    {
        // if the distance of the current node is shorter - update minNode
        if ((nodeArray[i]->distance < nodeArray[minNode]->distance) && (nodeArray[i]->numChildren == FAIL))
        {
            minNode = i;
        }
    }// return the distance of the min node
    return nodeArray[minNode]->distance;
}

/**
 * return the maximal branch of the tree ( longest distance between root and a leaf).
 * @param rootVal
 * @param nodeArray
 * @param nodeArraySize
 * @return return the maximal branch of the tree ( longest distance between root and a leaf).
 */
int getLengthMaximalBranch(int rootVal, Node ** nodeArray, int nodeArraySize)
{
    // length of minimal branch -
    // -> bfs with root..
    runBFS(rootVal, nodeArray, nodeArraySize);
    //going over all distances, searching for a maximun dis which is also a leaf
    int maxNode = 0;
    // initial first maximal branch as one of the leafs
    for (int j = 0; j < nodeArraySize; j++)
    {
        if (nodeArray[j]->numChildren == FAIL)
        {
            maxNode = j;
            break;
        }
    }
    // now search for a longest branch
    for (int i = 0; i < nodeArraySize; i++)
    {
        // if the distance of the current node is longer - update maxNode
        if ((nodeArray[i]->distance > nodeArray[maxNode]->distance) && (nodeArray[i]->numChildren == FAIL))
        {
            maxNode = i;
        }
    }// return the distance of the min node
    return nodeArray[maxNode]->distance;
}

/**
 * return the diameter of a given tree graph
 * @param rootNode
 * @param nodeArray
 * @param nodeArraySize
 * @return return the diameter of a given tree graph
 */
int getDiameter(int rootNode, Node ** nodeArray, int nodeArraySize)
{
    // get other node
    int otherNode = runBFS(rootNode, nodeArray, nodeArraySize);
    // get last node
    int lastNode = runBFS(otherNode, nodeArray, nodeArraySize);
    // return last node.distance
    return nodeArray[lastNode]->distance;
}

/**
 * finding the shortest path is a given valid tree graph and update the given args accordingly.
 * @param nodeArray  the valid tree graph as a node array
 * @param nodeArraySize
 * @param node1Data the node from which the path starts
 * @param node2Data the node the path ends with
 * @param shortestPath array which holds the simple path (int's with node's data in a natural order)
 * @param shortestPathCounter size of path array
 */
void findShortestPath(Node ** nodeArray, const int nodeArraySize, int node1Data,
                      int node2Data, int * shortestPath, int * shortestPathCounter)
{
    // int bfs (vertex1)
    runBFS(node1Data, nodeArray, nodeArraySize);
    //create new array with max num of node
    int * tempShortestPath;
    tempShortestPath = (int *) (malloc(sizeof(int) * (nodeArraySize)));
    // taking the vertex2 and saving it's prev
    // saving it's pre, and pre and pre... up until i get to vertx 1.
    int counter = 0;
    int i = node2Data;
    while ((nodeArray[i]->prev != node1Data))
    {
        tempShortestPath[counter] = i;
        i = nodeArray[i]->prev;
        counter++;
    }
    // node array[i]. prev == node1Data.
    tempShortestPath[counter] = i;
    counter++;

    // add node1Data at the end of the array,

    tempShortestPath[counter] = node1Data;
    counter++;
    // organize in shortestPath in opposite direction:


    for (int j = 0; j < counter; j++)
    {
        shortestPath[j] = tempShortestPath[counter - j - 1];
    }// now shortest path is updated.

    free(tempShortestPath);
    * shortestPathCounter = counter;
}

/**
 * method free all nodes in node array which is given as a param
 * @param nodeArray
 * @param actualNumNodes
 * @return 1 if succeded, 0 other wise
 */
int freeAll(Node *** nodeArray, int actualNumNodes)
{
    // free first childrenArray, node, node array
    for (int i = 0; i < actualNumNodes; i++)
    {
        free((* nodeArray)[i]->children);
        (* nodeArray)[i]->children = NULL;
        free((* nodeArray)[i]);
        (* nodeArray)[i] = NULL;
    }
    return SUCCESS;
}

/**
 * main function, running the program
 * @param argc
 * @param argv
 * @return 1 if failed, 0 if succeded
 */
int main(int argc, char ** argv)
{

    FILE * filePointer;
    int node1DataVal = -1;
    int * node1Data = & node1DataVal;

    int node2DataVal = -1;
    int * node2Data = & node2DataVal;
    int errorArgsVal = 0;
    int * errorArgs = & errorArgsVal;
    // check args are valid
    if (!validateArgs(argc, argv, node1Data, node2Data, errorArgs))
    {
        if (* errorArgs == UNVALID_ARGS_ERROR_VAL)
        {
            fprintf(stderr,
                    UNVALID_ARGS_ERROR_PRINT);
        }
        else if (* errorArgs == INVALID_INPUT_ERROR)
        {
            fprintf(stderr, "Invalid input\n");
        }
        return EXIT_FAILURE;
    }

    // put args in variables.
    char * fileName = argv[1];

    // open file in read mode
    filePointer = fopen(fileName, "r");
    if (filePointer == NULL)
    {

        fclose(filePointer);
        fprintf(stderr, UNVALID_ARGS_ERROR_PRINT);

        return EXIT_FAILURE;
    }

    // now file is open.
    // Check if file content is valid and place in array.

    int maxNumNodes = readAndProcessFirstLine(filePointer, * node1Data, * node2Data);
    if (maxNumNodes <= 0)
    {
        fclose(filePointer);
        // if first line was not valid
        fprintf(stderr, "Invalid input\n");

        return EXIT_FAILURE;
    }

    Node ** nodeArray;
    // create node array
    nodeArray = (Node **) (malloc(sizeof(Node *) * (maxNumNodes)));

    int numNodes = 0;
    int * actualNumNodes = & numNodes;
    int numEdgesVal = 0;
    int * numEdges = & numEdgesVal;

    // read and process rest of file
    // if failed - free array,  print - "Invalid input\n" and exit.
    int errorVal = 0;
    int * error = & errorVal;
    if (!(readAndProcessFileToNodeArray(filePointer, nodeArray, maxNumNodes, actualNumNodes, numEdges, error)))
    {
        freeAll(& nodeArray, * actualNumNodes);
        free(nodeArray);
        nodeArray = NULL;
        fclose(filePointer);
        if (* error == NOT_A_TREE_ERROR_VAL)
        {
            fprintf(stderr, NOT_A_TREE_ERROR_PRINT);
        }
        else if (* error == INVALID_INPUT_ERROR)
        {
            fprintf(stderr, "Invalid input\n");
        }
        return EXIT_FAILURE;
    }
    // if process succed - close file and continue running
    fclose(filePointer);

    //if not failed - check if graph is a tree:
    // 1. check there's only one root (not more than one node doesn't have a father)
    int rootVal = -1;
    int * rootNode = & rootVal;
    if (!rootsNumValidation(nodeArray, * actualNumNodes, rootNode))
    {
        freeAll(& nodeArray, * actualNumNodes);
        free(nodeArray);
        nodeArray = NULL;
        fprintf(stderr, NOT_A_TREE_ERROR_PRINT);
        return EXIT_FAILURE;
    }
    // run DFS to know if there's a circle in the graph.
    // (take the only father and run explore)
    // explore return if graph has a circle, return 1, if doesn't have a circle - return 0
    resetParamsForDFS(& nodeArray, maxNumNodes);
    int hasCircle = exploreDFS(& nodeArray, nodeArray[* rootNode]);
    if (hasCircle == SUCCESS)
    {// there is a circle - so graph cannot be a tree
        freeAll(& nodeArray, * actualNumNodes);
        free(nodeArray);
        nodeArray = NULL;
        fprintf(stderr, NOT_A_TREE_ERROR_PRINT);
        return EXIT_FAILURE;
    }
    // check that sum v= sum e +1 to know it is a tree
    if (actualNumNodes != numEdges + 1)
    {
        fprintf(stderr, NOT_A_TREE_ERROR_PRINT);
        return EXIT_FAILURE;
    }

    // length of minimal branch -
    // -> bfs with root. going over all distances, searching for a minimum dis which is also a leaf.
    int minBranchLength = getLengthMininalBranch(* rootNode, nodeArray, maxNumNodes);

    // length of maximal branch -
    // bfs - with root. going over all distances, returning the max dis which is also a leaf
    int maxBranchLength = getLengthMaximalBranch(* rootNode, nodeArray, maxNumNodes);

    // find shortest path arg
    int * shortestPath;
    shortestPath = (int *) (malloc(sizeof(int) * (maxNumNodes)));
    int shortestPathCounterVal = 0;
    int * shortestPathCounter = & shortestPathCounterVal;
    // update shortest path
    if (* actualNumNodes == 1)
    {
        shortestPath[0] = 0;
        * shortestPathCounter = 1;
    }
    else if (* node1Data == * node2Data)
    {
        shortestPath[0] = * node1Data;
        * shortestPathCounter = 1;

    }
    else
    {
        findShortestPath(nodeArray, maxNumNodes, * node1Data, * node2Data, shortestPath,
                         shortestPathCounter);
    }
    // diameter -
    int diameter = getDiameter(* rootNode, nodeArray, maxNumNodes);

    // print output
    printf("Root Vertex: %d \n", rootVal);
    printf("Vertices Count: %d\n", * actualNumNodes);
    printf("Edges Count: %d\n", * actualNumNodes - 1);
    printf("Length of Minimal Branch: %d\n", minBranchLength);
    printf("Length of Maximal Branch: %d\n", maxBranchLength);
    printf("Diameter Length: %d\n", diameter);
    printf("Shortest Path Between %d and %d:", * node1Data, * node2Data);
    for (int j = 0; j < * shortestPathCounter; j++)
    {
        printf(" %d", shortestPath[j]);
    }

    freeAll(& nodeArray, * actualNumNodes);
    free(shortestPath);
    free(nodeArray);
    nodeArray = NULL;
    return EXIT_SUCCESS;
}


