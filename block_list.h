


typedef struct Node Node; 

typedef struct BList BList;

struct Node {
    unsigned int m_prev;
    unsigned int m_next;
};

struct BList {
    void * m_base;
    Node  m_begin;/* UINT_MAX is saved address*/
};



void BListInit(BList *_list, void *_base);

void BListPush(BList *_list,Node *_newNode);

void * BListPop(BList *_list);

int BListIsEmpty(BList *_list);

int BListRemove(Node *_node,void * _base);