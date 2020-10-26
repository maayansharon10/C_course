#include <stdio.h>
# include "vector.h"
#include "assert.h"

# define TRUE 1
# define FALSE 0

/**
 * struct represent a vector
 */
typedef struct Vector
{
    void **data;
    size_t vec_size;
    size_t vec_capacity;
    vector_elem_cmp cmp;
    vector_elem_free free_func;
} Vector;


// region Construction & Destruction

/**
 * \brief Allocates a new vector.
 * @param comparator_func A pointer to a function that can compare between two elements of this vector. This
 * pointer should not be NULL.
 * @param free_func A pointer to a function that can free this vector from memory. This value might be NULL.
 * @return The created vector or NULL if the vector could not be created.
 */
vector_t vector_alloc(vector_elem_cmp comparator_func, vector_elem_free free_func)
{
    assert(comparator_func != NULL && "vector_alloc: comparator_func is NULL");
    if (comparator_func == NULL)
    {

        return NULL;
    }
    Vector *newVec = (Vector *) calloc(1, sizeof(Vector));
    assert(newVec != NULL && "vector_alloc: new vector allocation failed");
    if (newVec == NULL)
    {
        return NULL;
    }
    newVec->cmp = comparator_func;
    newVec->free_func = free_func;
    newVec->vec_capacity = VECTOR_INITIAL_SIZE;
    newVec->vec_size = 0;
    void **data = (void **) calloc(VECTOR_INITIAL_SIZE, sizeof(void *));
    assert(data != NULL && "vector_alloc: new vector allocation failed");
    if (data == NULL)
    {
        return NULL;
    }
    newVec->data = data;
    return (vector_t) newVec;
}

/**
 * \brief Frees (de-allocates) the given vector.
 * @param vec The vector.
 */
void vector_free(vector_t *vec)
{
    assert((vec != NULL && *vec != NULL) && "You must provide a valid vector pointer to free.");
    if (vec == NULL)
    {
        return;
    }

    if (*vec == NULL)
    {
        return;
    }
    Vector *curVec = (Vector *) (*vec);
    if (curVec->data != NULL)
    {
        for (size_t i = 0; i < curVec->vec_size; i++)
        {
            if (curVec->free_func != NULL)
            {
                curVec->free_func(curVec->data[i]);
            }
            curVec->data[i] = NULL;
        }
        free(curVec->data);
        curVec->data = NULL;
    }
    free(*vec);
    *vec = NULL;
}

// endregion

// region Accessors

/**
 * \brief Gets the current vector size.
 *
 * @param vec The vector to fetch its size for.
 */
size_t vector_get_size(vector_t vec)
{
    assert(vec != NULL && "vector_get_size: vector_t is NULL");
    if (vec == NULL)
    {
        return 0;
    }
    Vector *curVec = (Vector *) vec;
    return curVec->vec_size;
}

/**
 * \brief Gets the current vector capacity.
 *
 * @param vec The vector to fetch its capacity for.
 * @return The current vector capacity, or -1 if the given vector is invalid.
 */
size_t vector_get_capacity(vector_t vec)
{
    assert(vec != NULL && "vector_get_capacity: vector_t is NULL");
    if (vec == NULL)
    {
        return 0;
    }
    Vector *curVec = (Vector *) vec;
    return curVec->vec_capacity;
}

/**
 * \brief Gets a value that determines whether or not this vector is empty.
 *
 * @param vec The vector to check.
 * @return 1 if the vector is empty, or 0 otherwise.
 */
int vector_is_empty(vector_t vec)
{
    assert(vec != NULL && "vector_is_empty: vector_t is NULL");
    if (vec == NULL)
    {
        return 0;
    }
    Vector *curVec = (Vector *) vec;
    if (curVec->vec_size == 0)
    {
        return 1;
    }
    return 0;
}

/**
 * \brief  Get the vector element that was stored at the given position.
 *
 * @param vec The vector to look for the element in.
 * @param pos The element position (index).
 * @return A pointer to the item in the selected position.
 */
void *vector_at(vector_t vec, size_t pos)
{
    assert(vec != NULL && "vector_get_capacity: vector_t is NULL");
    if (vec == NULL)
    {
        return NULL;
    }
    assert((pos <= vector_get_size(vec)) &&
           "The position must be between 0 (inclusive) and size (exclusive).");
    if (pos > vector_get_size(vec) )
    {
        return NULL;
    }

    Vector *curVec = (Vector *) vec;
    return curVec->data[pos];
}

/**
 * \brief Attempts to find the given element in the vector.
 * @param vec The vector to look for the element in.
 * @param elem The element we're looking for.
 * @return The index of the element in the vector, or <code>NOT_FOUND</code> if the element could not be located.
 */
size_t vector_find(vector_t vec, void *elem)
{
    assert(vec != NULL && "vector_find: vec is NULL");

    if (vec == NULL)
    {
        return NOT_FOUND;
    }

    if (elem == NULL)
    {
        return NOT_FOUND;
    }

    Vector *curVec = (Vector *) vec;
    if (curVec->data == NULL)
    {
        return NOT_FOUND;
    }

    if (vector_get_size(vec) == 0)
    {
        return NOT_FOUND;
    }
    for (size_t i = 0; i < curVec->vec_size; i++)
    {
        if (curVec->cmp(curVec->data[i], elem) == 0)
        {
            return i;
        }
    }
    return NOT_FOUND;
}


/**
 * \brief Compares the given vectors. Vectors are considered to be equal if their size is equal, they got the same
 * comparator function and they contains the exact same element, compared "by content" and not by pointers.
 * Note that different capacity does not affect the comparison results.
 *
 * @param lhs The left-hand-side vector.
 * @param rhs The right-hand-side vector.
 * @return True if the vectors are equal and false otherwise.
 */
size_t vectors_are_equal(vector_t lhs, vector_t rhs)
{

    assert((lhs != NULL) && "vectors_are_equal: lhs ==NULL");
    if (lhs == NULL)
    {
        return FALSE;
    }
    assert((rhs != NULL) && "vectors_are_equal: rhs ==NULL");

    if (rhs == NULL)
    {
        return FALSE;
    }

    Vector *lVec = (Vector *) lhs;
    Vector *rVec = (Vector *) rhs;

    // check cmp's are the same
    if (lVec->cmp != rVec->cmp)
    {
        return FALSE;
    }
    // check sizes are the same
    if (lVec->vec_size != rVec->vec_size)
    {
        return FALSE;
    }
    if (lVec->vec_size == 0)
    {
        return TRUE;
    }
    for (size_t i = 0; i < lVec->vec_size; i++)
    {
        if (lVec->cmp(lVec->data[i], rVec->data[i]))
        {
            // one element detected as not the same
            return FALSE;
        }
    }
    // went through the entire vector and all elm are the same!
    return TRUE; //true

}

// endregion

// region Modifiers

/**
 * \brief Insert a new element at the end of the vector.
 *
 * @param vec The vector to add the new element into.
 * @param elem The element to add.
 * @return 1 if the item was added successfully, false otherwise.
 */
int vector_push_back(vector_t vec, void *elem)
{
    assert((vec != NULL) && "vector_push_back: vec ==NULL");
    if(vec == NULL)
    {
        return FALSE;
    }
    Vector *curVec = (Vector *) vec;

    // if size==capacity - enlarge, realloc, handle errors.
    if (vector_get_size(curVec) >= vector_get_capacity(curVec))
    {
        curVec->vec_capacity = (size_t) (VECTOR_GROWTH_RATE * (float) (vector_get_capacity(vec) + 1));
        void **newData = (void **) realloc(curVec->data, (curVec->vec_capacity * sizeof(*curVec->data)));
        if (newData == NULL)
        {
            curVec->data = NULL;
            return FALSE;
        }
        curVec->data = newData;
    }
    // capacity is big enough - add element in data[size]
    curVec->data[vector_get_size(curVec)] = elem;
    // One elem was added - size should be updated.
    curVec->vec_size += 1;
    return TRUE;
}

/**
 * \brief Insert a new element to the vector, at the given position.=
 *
 * @param vec The vector to add the new element into.
 * @param pos The position of new element in the vector.
 * @param elem The element to add.
 * @return 1 if the item was added successfully, false otherwise.
 */
int vector_insert(vector_t vec, size_t pos, void *elem)
{
    assert(vec != NULL && "vector_insert: vec == NULL");
    if (vec == NULL)
    {
        return FALSE;
    }
    Vector *curVec = (Vector *) vec;
    // limits of pos must be : 0<= pos<vector_get_size(curVec)
    assert((pos <= curVec->vec_size) && "The position must be between 0 (inclusive) and size (exclusive).");
    if (pos > curVec->vec_size)
    {

        return FALSE;
    }
    // if size==capacity - enlarge, realloc, handle errors.
    if (curVec->vec_size >= curVec->vec_capacity )
    {
        curVec->vec_capacity = (size_t) (VECTOR_GROWTH_RATE * (float )(vector_get_capacity(vec) + 1));

        void **newData = (void **) realloc(curVec->data, (curVec->vec_capacity * sizeof(*curVec->data)));

        if (newData == NULL)
        {

            return FALSE;
        }
        curVec->data = newData;
    }
    for (size_t i = curVec->vec_size; i > pos; i--)
    {
        curVec->data[i] = curVec->data[i - 1];
    }
    curVec->data[pos] = elem;
    curVec->vec_size += 1;
    return TRUE;
}

/**
 * \brief Removes the last element of the vector. If the vector size is zero, nothing will happen.
 *
 * @param vec The vector to remove the last element from.
 * @return 1 if the item was erased successfully, false otherwise.
 */
int vector_pop_back(vector_t vec)
{
    assert(vec != NULL && "vector_pop_back: vec == NULL");
    if (vec == NULL)
    {
        return FALSE;
    }

    Vector *curVec = (Vector *) vec;
    int size = vector_get_size(vec);
    assert(curVec->vec_size > 0 && "vector_pop_back: vector size is 0");
    if (curVec->vec_size <= 0)
    {
        return FALSE;
    }
    // assuming size>0:
    if(curVec->free_func !=NULL)
    {
        curVec->free_func(curVec->data[curVec->vec_size - 1]);
    }
    curVec->data[size - 1] = NULL;
    curVec->vec_size -= 1;
    return TRUE;

}

/**
 * \brief Remove the elements that corresponds to the given position from the vector.
 *
 * @param vec The vector to remove the last element from.
 * @param pos The element position.
 * @return 1 if the item was erased successfully, false otherwise.
 */
int vector_erase(vector_t vec, size_t pos)
{
    assert(vec != NULL && "vector_erase: vec == NULL");
    if (vec == NULL)
    {
        return FALSE;
    }

    // limits of pos must be : 0<= pos<vector_get_size(curVec)
    assert((pos <= vector_get_size(vec)) && "vector_erase: pos is invalid");
    if (pos >= vector_get_size(vec))
    {
        return FALSE;
    }
    Vector *curVec = (Vector *) vec;
    size_t size = vector_get_size(vec);

    for (size_t i = pos; i < size; i++)
    {
        curVec->data[i] = curVec->data[i + 1];
    }
    curVec->free_func(curVec->data[size - 1]);
    curVec->data[size - 1] = NULL;  //not a must
    curVec->vec_size -= 1;
    return TRUE;
}

/**
 * \brief Removes all of the elements from the vector.
 *
 * This function removes all of the elements from vector, leaving the vector empty, and thus it size will be zero.
 * Memory allocated for vector data remains the same.
 *
 * @param vec The vector to clear.
 * @return 1 if the vector was cleared successfully, false otherwise.
 */
int vector_clear(vector_t vec)
{

    assert(vec != NULL && "vector_clear: vec == NULL");
    if (vec == NULL)
    {
        return FALSE;
    }

    Vector *curVec = (Vector *) vec;
    size_t size = vector_get_size(vec);

    for (size_t i = 0; i < size; i++)
    {
        if (curVec->free_func != NULL)
        {
            curVec->free_func(curVec->data[i]);
        }
        curVec->data[i] = NULL;
    }

    return TRUE;
}


// endregion

