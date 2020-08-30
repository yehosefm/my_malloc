#include "block_list.h"
#include <limits.h>
#include <stddef.h>
#include <assert.h>

#define TO_ADDRESS(base, numberAddress) ((void *)((char *)(base) + (numberAddress)))
#define TO_NUMBER(base, address) ((unsigned int)(((char *)(address) - (char *)(base))))

void BListInit(BList *_list, void *_base) {
   assert(_list != NULL);
   assert(_base != NULL);
   _list->m_base = _base;
   _list->m_begin.m_next = UINT_MAX;
   _list->m_begin.m_prev = UINT_MAX;
}


void BListPush(BList* _list, Node *newNode) {
    newNode->m_prev = TO_NUMBER(_list->m_base, &(_list->m_begin));
    newNode->m_next = _list->m_begin.m_next;
    
    if(BListIsEmpty(_list)){
        _list->m_begin.m_next = TO_NUMBER(_list->m_base, newNode);
        return;
    }
    ((Node *)TO_ADDRESS(_list->m_base,_list->m_begin.m_next))->m_prev = TO_NUMBER(_list->m_base, newNode);
    _list->m_begin.m_next = TO_NUMBER(_list->m_base, newNode);
}

void * BListPop(BList *_list){
    Node *next,*nextnext;
    if(BListIsEmpty(_list)){
        return NULL;
    }
    next = (Node *)TO_ADDRESS(_list->m_base,_list->m_begin.m_next);
    if(next->m_next == UINT_MAX){
        _list->m_begin.m_next = UINT_MAX;
        return next;
    }
    nextnext = (Node *)TO_ADDRESS(_list->m_base, next->m_next);
    nextnext->m_prev = TO_NUMBER(_list->m_base, &_list->m_begin);
    _list->m_begin.m_next = next->m_next;
    return (void *)next;
}

int BListIsEmpty(BList *_list) {
    if(_list == NULL){
        return 1;
    }
    if(_list->m_begin.m_next == UINT_MAX){
        return 1;
    }
    return 0;
}

int BListRemove(Node *_node,void * _base){
    Node *prev , *next;
    if(_node == NULL){
        return 0;
    }
    prev = TO_ADDRESS( _base ,_node->m_prev);
    prev->m_next = _node->m_next;
    if(_node->m_next != UINT_MAX){
        next = TO_ADDRESS( _base ,_node->m_next);
        next->m_prev = _node->m_prev;
    }
    return 1;
}
