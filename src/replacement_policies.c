//
// This file contains all of the implementations of the replacement_policy
// constructors from the replacement_policies.h file.
//
// It also contains stubs of all of the functions that are added to each
// replacement_policy struct at construction time.
//
// ============================================================================
// NOTE: It is recommended that you read the comments in the
// replacement_policies.h file for further context on what each function is
// for.
// ============================================================================
//

#include "replacement_policies.h"

// LRU Replacement Policy
// ============================================================================
// TODO feel free to create additional structs/enums as necessary

//------------------------Access Record Matrix Initialization-----------------------------
#define MAX_ROWS 1048576
#define MAX_COLS 64

typedef struct
{
    uint32_t accessed_before;
    uint32_t tag;
    uint32_t mod;
    uint32_t evict;
} access_record;

access_record Record[MAX_ROWS][MAX_COLS];
int rows, cols;

void set_Record_array_size(int sets, int associativity)
{
    rows = sets;
    cols = associativity;
}

void allocateMemory()
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Record[i][j].accessed_before = 0;
            Record[i][j].tag = 0;
            Record[i][j].mod = 0;
            Record[i][j].evict = 0;
        }
    }
}

void modifyRecord(int row, int col, int accessed_before, int tag, int mod, int evict) {
    Record[row][col].accessed_before = accessed_before;
    Record[row][col].tag = tag;
    Record[row][col].mod = mod;
    Record[row][col].evict = evict;
}

void printArray(int set_idx, int cols) {
        for (int j = 0; j < cols; j++) {
            printf("(%d, %d, %d, %d) ", Record[set_idx][j].accessed_before, Record[set_idx][j].tag, Record[set_idx][j].mod, Record[set_idx][j].evict);
        }
        printf("\n");
}

//------------------------Access Record Matrix Initialization-----------------------------

void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    printf("LRU_CACHE_ACCESS\n");
    //printf("\n Accessed set index: %d\n", set_idx);
    //printf("\n Accessed tag %d\n", tag);

    for (int i = 0; i < cols; i++)
    {
        if (Record[set_idx][i].tag != 0)
        {
            Record[set_idx][i].accessed_before += 1;
        }
    }

    int success = 0;
    for (int i = 0; i < cols; i++)
    {
        if (Record[set_idx][i].tag == tag)
        {
            Record[set_idx][i].accessed_before = 0;
            success = 1;
            break;
        }
    }
    if (success == 0)
    {
        for (int i = 0; i < cols; i++)
        {
            if (Record[set_idx][i].tag == 0)
            {
                modifyRecord(set_idx, i, 0, tag, 0, 0);
                success = 1;
                break;
            }
        }
    }
    
    printArray(set_idx, cols);

    // TODO update the LRU replacement policy state given a new memory access

    uint32_t set_start = set_idx * cache_system->associativity;
    struct cache_line *setstart = &cache_system->cache_lines[set_start];
    int oldest = 0;
    for (int i = 0; i < cache_system->associativity; i++)
    {
        if ((setstart + i)->tag == tag)
        {
            for (int j = 0; j < cache_system->associativity; j++)
            {
                if ((setstart + j)->data >= oldest)
                {
                    oldest = (setstart+j)->data;
                }
            }
            (setstart+i)->data = oldest+1;
        }
    }
}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    //printf("LRU_EVICTION_INDEX\n");

    printf("Set Full, need evict.\n");
    uint32_t oldest = 0;
    int evict_index = 0;
    for (int i = 0; i < cols; i++)
    {
        if (Record[set_idx][i].accessed_before > oldest)
        {
            oldest = Record[set_idx][i].accessed_before;
            evict_index = i;
        }
    }
    Record[set_idx][evict_index].evict = 1;
    printArray(set_idx, cols);
    modifyRecord(set_idx, evict_index, 0, tag, 0, 0);
    printArray(set_idx, cols);
    // TODO return the index within the set that should be evicted.
    uint32_t set_start = set_idx * cache_system->associativity;
    struct cache_line *setstart = &cache_system->cache_lines[set_start];
    uint32_t min = 2147483647;
    evict_index = 0;
    for (int i = 0; i < cache_system->associativity; i++)
    {
        if ((setstart + i)->data < min)
        {
            min = (setstart + i)->data;
            evict_index = i;
        }
    }

    return evict_index;
}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    //printf("LRU_REPLACEMENT_POLICY_CLEANUP\n");
    // TODO cleanup any additional memory that you allocated in the
    // lru_replacement_policy_new function.
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Initiate the access record matrix
    set_Record_array_size(sets, associativity);
    allocateMemory();

    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_rp->data.

    return lru_rp;
}

// RAND Replacement Policy
// ============================================================================
void rand_cache_access(struct replacement_policy *replacement_policy,
                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO update the RAND replacement policy state given a new memory access
}

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO return the index within the set that should be evicted.

    return ((int)((double)rand()/((double)RAND_MAX +1)*cache_system->associativity));
}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // rand_replacement_policy_new function.
}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Seed randomness
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // rand_rp->data.

    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // TODO update the LRU_PREFER_CLEAN replacement policy state given a new
    // memory access
    // NOTE: you may be able to share code with the LRU policy

    for (int i = 0; i < cols; i++)
    {
        if (Record[set_idx][i].tag != 0)
        {
            Record[set_idx][i].accessed_before += 1;
        }
    }

    int success = 0;
    for (int i = 0; i < cols; i++)
    {
        if (Record[set_idx][i].tag == tag)
        {
            Record[set_idx][i].accessed_before = 0;
            success = 1;
            break;
        }
    }
    if (success == 0)
    {
        for (int i = 0; i < cols; i++)
        {
            if (Record[set_idx][i].tag == 0)
            {
                modifyRecord(set_idx, i, 0, tag, 0, 0);
                success = 1;
                break;
            }
        }
    }
    
    printArray(set_idx, cols);

}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    printf("Set Full, need evict.\n");

    uint32_t set_start = set_idx * cache_system->associativity;
    struct cache_line *start = &cache_system->cache_lines[set_start];

    uint32_t oldest = 0;
    int evict_index = -1;
    for (int i = 0; i < cols; i++)
    {
        if (Record[set_idx][i].accessed_before > oldest & (start+i)->status != MODIFIED)
        {
            oldest = Record[set_idx][i].accessed_before;
            evict_index = i;
        }
    }
    if (oldest == 0)
    {
        for (int i = 0; i < cols; i++)
        {
            if (Record[set_idx][i].accessed_before > oldest)
            {
                oldest = Record[set_idx][i].accessed_before;
                evict_index = i;
            }
        }
    }
    Record[set_idx][evict_index].evict = 1;
    printArray(set_idx, cols);
    modifyRecord(set_idx, evict_index, 0, tag, 0, 0);
    printArray(set_idx, cols);
    // TODO return the index within the set that should be evicted.

    return evict_index;
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_prefer_clean_replacement_policy_new function.
}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    set_Record_array_size(sets, associativity);
    allocateMemory();

    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_prefer_clean_rp->data.

    return lru_prefer_clean_rp;
}
