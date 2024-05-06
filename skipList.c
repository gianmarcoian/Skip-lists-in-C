#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#ifndef EQ
#define EQ(a, b) (a == b)
#endif

#ifndef LTE
#define LTE(a, b) (a < b)
#endif

struct skipLink
{
    int key;
    int value;
    struct skipLink *next;
    struct skipLink *down;
};

struct skipList
{
    struct skipLink *topSentinel;
    int size;
};

/* the public interface */
void test(void);
void initSkipList(struct skipList *slst);
int containsSkipList(struct skipList *slst, int key);
void removeSkipList(struct skipList *slst, int key);
void addSkipList(struct skipList *slst, int key, int value);
int sizeSkipList(struct skipList *slst);
void printSkipList(struct skipList *slst);

/* internal routines */
int flipSkipLink();
struct skipLink *slideRightSkipList(struct skipLink *current, int key);
struct skipLink *skipLinkAdd(struct skipLink *current, int key, int value);
struct skipLink *newSkipLink(int key, int value, struct skipLink *nextLnk, struct skipLink *downLnk);

/* ************************************************************************
Main Function
 ************************************************************************ */
/* Test function:
 param: no parameters
 pre:   no parameres
 post: prints out the contents of the skip list */

int main()
{
    int i = 0;
    /*srand(time(NULL));*/

    /*  Initialize the skip list */
    struct skipList *sl1 = (struct skipList *)malloc(sizeof(struct skipList));
    initSkipList(sl1);

    /*  Add to the skip list  M = 20 random integers in [0,100] */
    for (i = 0; i < 20; i++)
    {
        addSkipList(sl1, rand() % 101, i + 5);
    }
    addSkipList(sl1, 1, 9);

    /*  Print out the contents of the skip list in the breadth-first order, starting from top.
     While printing the elements, move to a new line every time you reach the end of one level,
     and move down to the lower level of the skip list.
     For example, the print out of a skip list with 5 elements should look like

     7
     7 14 29
     3 7 9 14 20

     */
    printf("---------- skipList 1 -----");
    printf("----- size %d -----\n", sizeSkipList(sl1));
    printSkipList(sl1);
    printf("---------- removed %d from skipList -----", sl1->topSentinel->next->key);
    removeSkipList(sl1, sl1->topSentinel->next->key);
    printf("----- size %d -----\n", sizeSkipList(sl1));
    printSkipList(sl1);

    removeSkipList(sl1, 92);
    printf("----- size %d -----\n", sizeSkipList(sl1));
    printSkipList(sl1);

    return 0;
}

/* ************************************************************************
Internal Functions
 ************************************************************************ */

/* Coin toss function:
 param:     no parameters
 pre:   no parameres
 post: output is a random intiger number in {0,1} */
int flipSkipLink(void)
{
    return (rand() % 2);
}

/* Move to the right as long as the next element is smaller than the input key:
 param:     current -- pointer to a place in the list from where we need to slide to the right
 param: key --  input key
 pre:   current is not NULL
 post: returns one link before the link that contains the input key key */
struct skipLink *slideRightSkipList(struct skipLink *current, int key)
{
    while ((current->next != 0) && LTE(current->next->key, key))
        current = current->next;
    return current;
}

/* Create a new skip link for a key
    param: key   -- the key to create a link for
    param: nextLnk   -- the new link's next should point to nextLnk
    param: downLnk -- the new link's down should point to downLnk
    pre:    none
    post:   a link to store the key */
struct skipLink *newSkipLink(int key, int value, struct skipLink *nextLnk, struct skipLink *downLnk)
{
    struct skipLink *tmp = (struct skipLink *)malloc(sizeof(struct skipLink));
    assert(tmp != 0);
    tmp->key = key;
    tmp->value = value;
    tmp->next = nextLnk;
    tmp->down = downLnk;
    return tmp;
}

/* Add a new skip link recursively
 param: current -- pointer to a place in the list where the new element should be inserted
 param: key  -- the key to create a link for
 pre:   current is not NULL
 post: a link to store the key */
struct skipLink *skipLinkAdd(struct skipLink *current, int key, int value)
{
    struct skipLink *newLink, *downLink;
    newLink = 0;
    if (current->down == 0)
    {
        newLink = newSkipLink(key, value, current->next, 0);
        current->next = newLink;
    }
    else
    {
        downLink = skipLinkAdd(slideRightSkipList(current->down, key), key, value);
        if (downLink != 0 && flipSkipLink())
        {
            newLink = newSkipLink(key, value, current->next, downLink);
            current->next = newLink;
        }
    }
    return newLink;
}

/* ************************************************************************
Public Functions
 ************************************************************************ */

/* Initialize skip list:
 param:  slst -- pointer to the skip list
 pre:   slst is not null
 post: the sentinels are allocated, the pointers are set, and the list size equals zero */
void initSkipList(struct skipList *slst)
{
    assert(slst != NULL);
    slst->topSentinel = (struct skipLink *)malloc(sizeof(struct skipLink));
    slst->topSentinel->next = 0;
    slst->topSentinel->down = 0;
    slst->size = 0;
}

/* Checks if an element is in the skip list:
 param: slst -- pointer to the skip list
 param: key -- element to be checked
 pre:   slst is not null
 post: returns true or false  */
int containsSkipList(struct skipList *slst, int key)
{
    struct skipLink *current = slst->topSentinel;
    while (current)
    {
        current = slideRightSkipList(current, key);
        if ((current->next != 0) && EQ(current->next->key, key))
            return 1;
        current = current->down;
    }
    return 0;
}

/* Remove an element from the skip list:
 param: slst -- pointer to the skip list
 param: key -- element to be removed
 pre:   slst is not null
 post: the new element is removed from all internal sorted lists */
void removeSkipList(struct skipList *slst, int key)
{
    struct skipLink *current, *temp;
    current = slst->topSentinel;

    while (current)
    {
        current = slideRightSkipList(current, key);
        if ((current->next != 0) && EQ(current->next->key, key))
        {
            temp = current->next;
            current->next = current->next->next;
            free(temp);
            if (current->down == NULL)
                slst->size--;
        }
        current = current->down;
    }
}

/* Add a new element to the skip list:
    param: slst -- pointer to the skip list
    param: key -- element to be added
    pre:    slst is not null
    post:   the new element is added to the lowest list and randomly to higher-level lists */
void addSkipList(struct skipList *slst, int key, int value)
{
    struct skipLink *downLink, *newLink;
    downLink = skipLinkAdd(slideRightSkipList(slst->topSentinel, key), key, value);
    if (downLink != 0 && flipSkipLink())
    {
        struct skipLink *newTopSentinel = (struct skipLink *)malloc(sizeof(struct skipLink));
        newLink = newSkipLink(key, value, 0, downLink);
        newTopSentinel->next = newLink;
        newTopSentinel->down = slst->topSentinel;
        slst->topSentinel = newTopSentinel;
    }
    slst->size++;
}

/* Find the number of elements in the skip list:
 param: slst -- pointer to the skip list
 pre:   slst is not null
 post: the number of elements */
int sizeSkipList(struct skipList *slst)
{
    return slst->size;
}

/* Print the links in the skip list:
    param: slst -- pointer to the skip list
    pre:    slst is not null and slst is not empty
    post: the links in the skip list are printed breadth-first, top-down */
void printSkipList(struct skipList *slst)
{
    struct skipLink *currentlist = slst->topSentinel;
    struct skipLink *currentlink;
    while (currentlist != NULL)
    {
        currentlink = currentlist->next;
        while (currentlink != NULL)
        {
            printf("{%d,%d}", currentlink->key, currentlink->value);
            currentlink = currentlink->next;
        }
        currentlist = currentlist->down;
        printf("\n");
        printf("\n");
    }
}