/** pagequeue.c
 * ===========================================================
 * Name: Noah Chavez, __ ___ 2026
 * Section: CS483 / M4
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue* q = malloc(sizeof(PageQueue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    q->maxSize = maxSize;
    return q;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.

    PqNode* temp1 = pq->tail;
    PqNode* temp2 = NULL;
    long depth = -1;
    bool hit = false;

    for(int i = 1; i <= pq->size; i++){ 
        if(!hit){
            //hit occurs
            if(temp1->pageNum == pageNum){
                depth = i;
                // hit at tail or one element
                if(temp2 == NULL){
                    hit = true;
                }
                // hit at head
                else if(temp1->prev == NULL){
                    pq->head = temp2; 
                    pq->tail->next = temp1;
                    temp1->prev = pq->tail;
                    pq->tail = temp1;  
                    temp2->prev = NULL;
                    temp1->next = NULL;
                    hit = true;
                }
                // hit in middle
                else{
                    temp2->next = temp1->prev;
                    temp1->prev->next = temp2;
                    pq->tail->next = temp1;
                    temp1->prev = pq->tail;
                    pq->tail = temp1;
                    temp1->next = NULL;
                    hit = true;
                }
                
            }
            else{
                temp2 = temp1;
                temp1 = temp1->prev;
            }
        }
    }
    //no hit, put new node at tail
    if(!hit){
        depth = -1;
        PqNode* new = malloc(sizeof(PqNode));
        new->pageNum = pageNum;
        new->next = NULL;
        new->prev = NULL;
        // add on tail id not at max size
        /*if(pq->size < pq->maxSize){
            if(pq->size == 0){
                pq->head = new;
                pq->tail = new;
            }
            else{
                new->prev = pq->tail;
                pq->tail->next = new;
                pq->tail = new;
                new->next = NULL;
            }
            pq->size++;
        }
        // get rid of head then add on tail if over max size
        else{
            PqNode* temp3 = pq->head;
            pq->head = pq->head->next;
            if(pq->head != NULL){
                pq->head->prev = NULL;
            }
            free(temp3);

            new->prev = pq->tail;
            pq->tail->next = new;
            pq->tail = new;
            new->next = NULL;
        }*/
       if(pq->size == 0){
            pq->head = new;
            pq->tail = new;
        }
        else{
            new->prev = pq->tail;
            pq->tail->next = new;
            pq->tail = new;
            new->next = NULL;
        }
        pq->size++;
    }
    return depth;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode* temp1 = pq->head;
    PqNode* temp2 = temp1->next;
    int num = pq->size;
    for(int i = 0; i < num; i++){
        temp2 = temp1->next;
        free(temp1);
        temp1 = temp2;
    }
    free(temp1);
    free(temp2);
    free(pq);
    pq = NULL;
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    PqNode* temp = pq->head;
    for(int i = 0; i < pq->size; i++){
        printf("%ld\n", temp->pageNum);
        temp = temp->next;
    }
}
