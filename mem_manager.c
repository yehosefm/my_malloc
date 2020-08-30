#include "mem_manager.h"
#include "block_list.h"
#include <assert.h>
#include <limits.h>

#define FREE -1
#define ALLOC 1

#define INIT_MODE (g_memManager.m_blocksAlloc < g_memManager.m_size)
#define SIZE_OF(block)(block + sizeof(Header))
#define INVALID_SIZE_BLOCK(size)(size != BLOCK1 && size != BLOCK2 && size != BLOCK3)

#define LIST1 ((BList*)(g_memManager.m_base))
#define LIST2 ((BList*)((char *)g_memManager.m_base)+ sizeof(BList))
#define LIST3 ((BList*)((char *)g_memManager.m_base)+2 *sizeof(BList))

typedef signed char  Header;

typedef struct MemManager MemManager; 

struct MemManager {
    void * m_base;
    unsigned int m_size;
    unsigned int m_blocksAlloc;
};

MemManager g_memManager;

void Init(void *_pblock, size_t _size) {
    assert(_pblock != NULL);
    assert(_size > 0 && _size < UINT_MAX);

    g_memManager.m_base = _pblock;
    g_memManager.m_size = _size / SIZE_OF(BLOCK1);

    BListInit(LIST1,_pblock);
    BListInit(LIST2, _pblock);
    BListInit(LIST3,_pblock);
    g_memManager.m_blocksAlloc = 1;     
}

static void* MallocFromLists(size_t _size);
static void* MallocFromSegment();

void* MyMalloc(size_t _size) {
    if(INVALID_SIZE_BLOCK(_size)) {
        return NULL;
    }
    if(_size == BLOCK1 && INIT_MODE) {
        return MallocFromSegment();
    }
    return  MallocFromLists(_size);
}
static void MergeBlockIf(void *_block);
void MyFree(void *_pblock) {
    Header * header;
    if(_pblock == NULL) {
        assert(!"free NULL pointer");
    }  

    header = (Header *)_pblock - 1;
    switch (*header)
    {
    case (BLOCK1 * ALLOC):
        BListPush(LIST1 ,_pblock);
        *header = BLOCK1 * FREE;
        break;
    case (BLOCK2 * ALLOC):
        BListPush(LIST2 ,_pblock);
        *header = BLOCK2 * FREE;
        MergeBlockIf(header);
        break;
    case (BLOCK3 * ALLOC):
        BListPush(LIST3 ,_pblock);
        *header = BLOCK3 * FREE;
        MergeBlockIf(header);
        break;
    default:
        assert(!"free invlaid Block");
    }
}

/**********************************************************************
 * STATIC FUNCTION
 */

static void* MallocFromSegment() {
    void *currBlock = (char *)g_memManager.m_base + SIZE_OF(BLOCK1) * g_memManager.m_blocksAlloc;
    *(Header *)currBlock = BLOCK1 * ALLOC;
    ++g_memManager.m_blocksAlloc;
    return (char *)currBlock +1 ;
}


static void SplitBlock(size_t _size) {
    size_t i;
    BList *list;
    void *block = MyMalloc(BLOCK1);
    if( NULL == block ) {
        return;
    }
    block = (char *)block - sizeof(Header);  
    if(_size == BLOCK2){
        list = LIST2;
    } else
    {
        list = LIST3;
    }
    for(i = 0; i <= (SIZE_OF(BLOCK1) - SIZE_OF(_size)); i += SIZE_OF(_size) ){
        *(Header *)((char *)block + i) = _size * FREE;
            BListPush(list, (Node *)((char *)block + i + sizeof(Header)));
    }
}

static void* MallocFromLists(size_t _size) {
    void *res,* header;
    switch(_size) {
        case BLOCK1:
            res = BListPop(LIST1); 
            break;
        case BLOCK2:
            res = BListPop(LIST2);
            if(res == NULL) {
                SplitBlock(_size);
            }
            res = BListPop(LIST2);
            break;
        case BLOCK3:
            res = BListPop(LIST3);
            if(res == NULL) {
                SplitBlock(_size);
            }
            res = BListPop(LIST3);
            break;
    }
    if (res == NULL){
        return NULL;
    }
    header = (char *)res - sizeof(Header);
    *(Header *)header = _size * ALLOC;
    return res;
}

static void *FindBeginBlock(void *_begin) {
    size_t indentOfBase = (size_t)g_memManager.m_base % SIZE_OF(BLOCK1);
    size_t indentOfAddress = (size_t)((char *)_begin-indentOfBase)% SIZE_OF(BLOCK1);
    return (char *)_begin - indentOfAddress;
}

static int BlockFree(void  *_beginBlock) {
    size_t i, sizeOfInBlocks =  *(Header *)_beginBlock;
    assert(!INVALID_SIZE_BLOCK(sizeOfInBlocks));  
     for(i = 0; i <= (SIZE_OF(BLOCK1) - SIZE_OF(sizeOfInBlocks)); i += SIZE_OF(sizeOfInBlocks) ){
        if(*(Header *)((char *)_beginBlock + i) != sizeOfInBlocks * FREE){
            return 0;
        } 
    }
    return 1;
}

static void Merge(void  *_beginBlock) {
    size_t i, sizeOfInBlocks;
    sizeOfInBlocks =  *(Header *)_beginBlock; 
     for(i = 0; i <= (SIZE_OF(BLOCK1) - SIZE_OF(sizeOfInBlocks)); i += SIZE_OF(sizeOfInBlocks) ){
        BListRemove((Node *)((char *)_beginBlock + i + sizeof(Header)), g_memManager.m_base);
    }
}

static void MergeBlockIf(void *_block) {
    void *beginBlock =  FindBeginBlock(_block);
    if(BlockFree(beginBlock)){
        Merge(beginBlock);
    }
}

