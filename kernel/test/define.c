#define CHUNKS_FLAG_SIZE                (1)
#define CHUNKS_FLAG_BUDDY               (0)
#define CHUNKS_FLAG_SLAB                (1)

#define CHUNKS_STATUS_SIZE              (1)
#define CHUNKS_STATUS_INUSE             (0)
#define CHUNKS_STATUS_UNUSE             (1)

#define CHUNKS_IDX_SIZE                 (sizeof(uintptr_t) * 8 - CHUNKS_STATUS_SIZE - CHUNKS_FLAG_SIZE)

#define CHUNKS_IDX_MASK                 ((((uintptr_t)1) << (CHUNKS_IDX_SIZE)) - 1)
#define CHUNKS_STATUS_MASK              ((((uintptr_t)1) << (CHUNKS_IDX_SIZE + CHUNKS_STATUS_SIZE)) - 1 - CHUNKS_IDX_MASK)
#define CHUNKS_FLAG_MASK                ((~((uintptr_t)0)) - CHUNKS_IDX_MASK - CHUNKS_STATUS_MASK)


#define CHUNKS_VAL_GET_IDX(val)         (((uintptr_t)(val)) & CHUNKS_IDX_MASK)
#define CHUNKS_VAL_GET_STATUS(val)      ((((uintptr_t)(val)) & CHUNKS_STATUS_MASK) >> (CHUNKS_IDX_SIZE))
#define CHUNKS_VAL_GET_FLAG(val)        ((((uintptr_t)(val)) & CHUNKS_FLAG_MASK) >> (CHUNKS_IDX_SIZE + CHUNKS_STATUS_SIZE))

#define CHUNKS_VAL_SET_IDX(ptr, val) \
    (*((uintptr_t*)(ptr))) &= (~CHUNKS_IDX_MASK); \
    (*((uintptr_t*)(ptr))) |= (((uintptr_t)(val)) & CHUNKS_IDX_MASK)
#define CHUNKS_VAL_SET_STATUS(ptr, val) \
    (*((uintptr_t*)(ptr))) &= (~CHUNKS_STATUS_MASK); \
    (*((uintptr_t*)(ptr))) |= ((((uintptr_t)(val)) << (CHUNKS_IDX_SIZE)) & CHUNKS_STATUS_MASK)
#define CHUNKS_VAL_SET_FLAG(ptr, val) \
    (*((uintptr_t*)(ptr))) &= (~CHUNKS_FLAG_MASK); \
    (*((uintptr_t*)(ptr))) |= ((((uintptr_t)(val)) << (CHUNKS_IDX_SIZE + CHUNKS_STATUS_SIZE)) & CHUNKS_FLAG_MASK)

#define CHUNKS_GET_IDX(addr)                (CHUNKS_VAL_GET_IDX(chunks[(((uintptr_t)(addr)) - chunks_base) / PAGESIZE]))
#define CHUNKS_GET_STATUS(addr)             (CHUNKS_VAL_GET_STATUS(chunks[(((uintptr_t)(addr)) - chunks_base) / PAGESIZE]))
#define CHUNKS_GET_FLAG(addr)               (CHUNKS_VAL_GET_FLAG(chunks[(((uintptr_t)(addr)) - chunks_base) / PAGESIZE]))
#define CHUNKS_SET_IDX(addr, idx)           CHUNKS_VAL_SET_IDX(chunks + ((((uintptr_t)(addr)) - chunks_base) / PAGESIZE), (idx))
#define CHUNKS_SET_STATUS(addr, status)     CHUNKS_VAL_SET_STATUS(chunks + ((((uintptr_t)(addr)) - chunks_base) / PAGESIZE), (status))
#define CHUNKS_SET_FLAG(addr, flag)         CHUNKS_VAL_SET_FLAG(chunks + ((((uintptr_t)(addr)) - chunks_base) / PAGESIZE), (flag))
int main(){
    CHUNKS_SET_IDX(1, 2);
    return 0;
}