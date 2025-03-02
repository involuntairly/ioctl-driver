#pragma once

#include <stdlib.h>
#include <ntifs.h>
#include <ntddk.h>
#include <ntimage.h>
#include <intrin.h>
#include <cstdint>
#include <stdio.h>

/**
 * @brief Format of a 4-Level Page-Table Entry that Maps a 4-KByte Page
 */

typedef union
{
	struct
	{
		/**
		 * [Bit 0] Present; must be 1 to map a 4-KByte page.
		 */
		UINT64 Present : 1;
#define PTE_64_PRESENT_BIT                                           0
#define PTE_64_PRESENT_FLAG                                          0x01
#define PTE_64_PRESENT_MASK                                          0x01
#define PTE_64_PRESENT(_)                                            (((_) >> 0) & 0x01)

		/**
		 * [Bit 1] Read/write; if 0, writes may not be allowed to the 4-KByte page referenced by this entry.
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 Write : 1;
#define PTE_64_WRITE_BIT                                             1
#define PTE_64_WRITE_FLAG                                            0x02
#define PTE_64_WRITE_MASK                                            0x01
#define PTE_64_WRITE(_)                                              (((_) >> 1) & 0x01)

		/**
		 * [Bit 2] User/supervisor; if 0, user-mode accesses are not allowed to the 4-KByte page referenced by this entry.
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 Supervisor : 1;
#define PTE_64_SUPERVISOR_BIT                                        2
#define PTE_64_SUPERVISOR_FLAG                                       0x04
#define PTE_64_SUPERVISOR_MASK                                       0x01
#define PTE_64_SUPERVISOR(_)                                         (((_) >> 2) & 0x01)

		/**
		 * [Bit 3] Page-level write-through; indirectly determines the memory type used to access the 4-KByte page referenced by
		 * this entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 PageLevelWriteThrough : 1;
#define PTE_64_PAGE_LEVEL_WRITE_THROUGH_BIT                          3
#define PTE_64_PAGE_LEVEL_WRITE_THROUGH_FLAG                         0x08
#define PTE_64_PAGE_LEVEL_WRITE_THROUGH_MASK                         0x01
#define PTE_64_PAGE_LEVEL_WRITE_THROUGH(_)                           (((_) >> 3) & 0x01)

		/**
		 * [Bit 4] Page-level cache disable; indirectly determines the memory type used to access the 4-KByte page referenced by
		 * this entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 PageLevelCacheDisable : 1;
#define PTE_64_PAGE_LEVEL_CACHE_DISABLE_BIT                          4
#define PTE_64_PAGE_LEVEL_CACHE_DISABLE_FLAG                         0x10
#define PTE_64_PAGE_LEVEL_CACHE_DISABLE_MASK                         0x01
#define PTE_64_PAGE_LEVEL_CACHE_DISABLE(_)                           (((_) >> 4) & 0x01)

		/**
		 * [Bit 5] Accessed; indicates whether software has accessed the 4-KByte page referenced by this entry.
		 *
		 * @see Vol3A[4.8(Accessed and Dirty Flags)]
		 */
		UINT64 Accessed : 1;
#define PTE_64_ACCESSED_BIT                                          5
#define PTE_64_ACCESSED_FLAG                                         0x20
#define PTE_64_ACCESSED_MASK                                         0x01
#define PTE_64_ACCESSED(_)                                           (((_) >> 5) & 0x01)

		/**
		 * [Bit 6] Dirty; indicates whether software has written to the 4-KByte page referenced by this entry.
		 *
		 * @see Vol3A[4.8(Accessed and Dirty Flags)]
		 */
		UINT64 Dirty : 1;
#define PTE_64_DIRTY_BIT                                             6
#define PTE_64_DIRTY_FLAG                                            0x40
#define PTE_64_DIRTY_MASK                                            0x01
#define PTE_64_DIRTY(_)                                              (((_) >> 6) & 0x01)

		/**
		 * [Bit 7] Indirectly determines the memory type used to access the 4-KByte page referenced by this entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 Pat : 1;
#define PTE_64_PAT_BIT                                               7
#define PTE_64_PAT_FLAG                                              0x80
#define PTE_64_PAT_MASK                                              0x01
#define PTE_64_PAT(_)                                                (((_) >> 7) & 0x01)

		/**
		 * [Bit 8] Global; if CR4.PGE = 1, determines whether the translation is global; ignored otherwise.
		 *
		 * @see Vol3A[4.10(Caching Translation Information)]
		 */
		UINT64 Global : 1;
#define PTE_64_GLOBAL_BIT                                            8
#define PTE_64_GLOBAL_FLAG                                           0x100
#define PTE_64_GLOBAL_MASK                                           0x01
#define PTE_64_GLOBAL(_)                                             (((_) >> 8) & 0x01)

		/**
		 * [Bits 11:9] Ignored.
		 */
		UINT64 CopyOnWrite : 1;
		UINT64 Unused : 1;
		UINT64 Write1 : 1;
#define PTE_64_IGNORED_1_BIT                                         9
#define PTE_64_IGNORED_1_FLAG                                        0xE00
#define PTE_64_IGNORED_1_MASK                                        0x07
#define PTE_64_IGNORED_1(_)                                          (((_) >> 9) & 0x07)

		/**
		 * [Bits 47:12] Physical address of the 4-KByte page referenced by this entry.
		 */
		UINT64 PageFrameNumber : 36;
#define PTE_64_PAGE_FRAME_NUMBER_BIT                                 12
#define PTE_64_PAGE_FRAME_NUMBER_FLAG                                0xFFFFFFFFF000
#define PTE_64_PAGE_FRAME_NUMBER_MASK                                0xFFFFFFFFF
#define PTE_64_PAGE_FRAME_NUMBER(_)                                  (((_) >> 12) & 0xFFFFFFFFF)
		UINT64 Reserved1 : 4;

		/**
		 * [Bits 58:52] Ignored.
		 */
		UINT64 Ignored2 : 7;
#define PTE_64_IGNORED_2_BIT                                         52
#define PTE_64_IGNORED_2_FLAG                                        0x7F0000000000000
#define PTE_64_IGNORED_2_MASK                                        0x7F
#define PTE_64_IGNORED_2(_)                                          (((_) >> 52) & 0x7F)

		/**
		 * [Bits 62:59] Protection key; if CR4.PKE = 1, determines the protection key of the page; ignored otherwise.
		 *
		 * @see Vol3A[4.6.2(Protection Keys)]
		 */
		UINT64 ProtectionKey : 4;
#define PTE_64_PROTECTION_KEY_BIT                                    59
#define PTE_64_PROTECTION_KEY_FLAG                                   0x7800000000000000
#define PTE_64_PROTECTION_KEY_MASK                                   0x0F
#define PTE_64_PROTECTION_KEY(_)                                     (((_) >> 59) & 0x0F)

		/**
		 * [Bit 63] If IA32_EFER.NXE = 1, execute-disable (if 1, instruction fetches are not allowed from the 1-GByte page
		 * controlled by this entry); otherwise, reserved (must be 0).
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 ExecuteDisable : 1;
#define PTE_64_EXECUTE_DISABLE_BIT                                   63
#define PTE_64_EXECUTE_DISABLE_FLAG                                  0x8000000000000000
#define PTE_64_EXECUTE_DISABLE_MASK                                  0x01
#define PTE_64_EXECUTE_DISABLE(_)                                    (((_) >> 63) & 0x01)
	};

	UINT64 Flags;
} PTE_64;

/**
 * @brief Format of a common Page-Table Entry
 */
typedef union
{
	struct
	{
		UINT64 Present : 1;
#define PT_ENTRY_64_PRESENT_BIT                                      0
#define PT_ENTRY_64_PRESENT_FLAG                                     0x01
#define PT_ENTRY_64_PRESENT_MASK                                     0x01
#define PT_ENTRY_64_PRESENT(_)                                       (((_) >> 0) & 0x01)
		UINT64 Write : 1;
#define PT_ENTRY_64_WRITE_BIT                                        1
#define PT_ENTRY_64_WRITE_FLAG                                       0x02
#define PT_ENTRY_64_WRITE_MASK                                       0x01
#define PT_ENTRY_64_WRITE(_)                                         (((_) >> 1) & 0x01)
		UINT64 Supervisor : 1;
#define PT_ENTRY_64_SUPERVISOR_BIT                                   2
#define PT_ENTRY_64_SUPERVISOR_FLAG                                  0x04
#define PT_ENTRY_64_SUPERVISOR_MASK                                  0x01
#define PT_ENTRY_64_SUPERVISOR(_)                                    (((_) >> 2) & 0x01)
		UINT64 PageLevelWriteThrough : 1;
#define PT_ENTRY_64_PAGE_LEVEL_WRITE_THROUGH_BIT                     3
#define PT_ENTRY_64_PAGE_LEVEL_WRITE_THROUGH_FLAG                    0x08
#define PT_ENTRY_64_PAGE_LEVEL_WRITE_THROUGH_MASK                    0x01
#define PT_ENTRY_64_PAGE_LEVEL_WRITE_THROUGH(_)                      (((_) >> 3) & 0x01)
		UINT64 PageLevelCacheDisable : 1;
#define PT_ENTRY_64_PAGE_LEVEL_CACHE_DISABLE_BIT                     4
#define PT_ENTRY_64_PAGE_LEVEL_CACHE_DISABLE_FLAG                    0x10
#define PT_ENTRY_64_PAGE_LEVEL_CACHE_DISABLE_MASK                    0x01
#define PT_ENTRY_64_PAGE_LEVEL_CACHE_DISABLE(_)                      (((_) >> 4) & 0x01)
		UINT64 Accessed : 1;
#define PT_ENTRY_64_ACCESSED_BIT                                     5
#define PT_ENTRY_64_ACCESSED_FLAG                                    0x20
#define PT_ENTRY_64_ACCESSED_MASK                                    0x01
#define PT_ENTRY_64_ACCESSED(_)                                      (((_) >> 5) & 0x01)
		UINT64 Dirty : 1;
#define PT_ENTRY_64_DIRTY_BIT                                        6
#define PT_ENTRY_64_DIRTY_FLAG                                       0x40
#define PT_ENTRY_64_DIRTY_MASK                                       0x01
#define PT_ENTRY_64_DIRTY(_)                                         (((_) >> 6) & 0x01)
		UINT64 LargePage : 1;
#define PT_ENTRY_64_LARGE_PAGE_BIT                                   7
#define PT_ENTRY_64_LARGE_PAGE_FLAG                                  0x80
#define PT_ENTRY_64_LARGE_PAGE_MASK                                  0x01
#define PT_ENTRY_64_LARGE_PAGE(_)                                    (((_) >> 7) & 0x01)
		UINT64 Global : 1;
#define PT_ENTRY_64_GLOBAL_BIT                                       8
#define PT_ENTRY_64_GLOBAL_FLAG                                      0x100
#define PT_ENTRY_64_GLOBAL_MASK                                      0x01
#define PT_ENTRY_64_GLOBAL(_)                                        (((_) >> 8) & 0x01)

		/**
		 * [Bits 11:9] Ignored.
		 */
		UINT64 Ignored1 : 3;
#define PT_ENTRY_64_IGNORED_1_BIT                                    9
#define PT_ENTRY_64_IGNORED_1_FLAG                                   0xE00
#define PT_ENTRY_64_IGNORED_1_MASK                                   0x07
#define PT_ENTRY_64_IGNORED_1(_)                                     (((_) >> 9) & 0x07)

		/**
		 * [Bits 47:12] Physical address of the 4-KByte page referenced by this entry.
		 */
		UINT64 PageFrameNumber : 36;
#define PT_ENTRY_64_PAGE_FRAME_NUMBER_BIT                            12
#define PT_ENTRY_64_PAGE_FRAME_NUMBER_FLAG                           0xFFFFFFFFF000
#define PT_ENTRY_64_PAGE_FRAME_NUMBER_MASK                           0xFFFFFFFFF
#define PT_ENTRY_64_PAGE_FRAME_NUMBER(_)                             (((_) >> 12) & 0xFFFFFFFFF)
		UINT64 Reserved1 : 4;

		/**
		 * [Bits 58:52] Ignored.
		 */
		UINT64 Ignored2 : 7;
#define PT_ENTRY_64_IGNORED_2_BIT                                    52
#define PT_ENTRY_64_IGNORED_2_FLAG                                   0x7F0000000000000
#define PT_ENTRY_64_IGNORED_2_MASK                                   0x7F
#define PT_ENTRY_64_IGNORED_2(_)                                     (((_) >> 52) & 0x7F)
		UINT64 ProtectionKey : 4;
#define PT_ENTRY_64_PROTECTION_KEY_BIT                               59
#define PT_ENTRY_64_PROTECTION_KEY_FLAG                              0x7800000000000000
#define PT_ENTRY_64_PROTECTION_KEY_MASK                              0x0F
#define PT_ENTRY_64_PROTECTION_KEY(_)                                (((_) >> 59) & 0x0F)
		UINT64 ExecuteDisable : 1;
#define PT_ENTRY_64_EXECUTE_DISABLE_BIT                              63
#define PT_ENTRY_64_EXECUTE_DISABLE_FLAG                             0x8000000000000000
#define PT_ENTRY_64_EXECUTE_DISABLE_MASK                             0x01
#define PT_ENTRY_64_EXECUTE_DISABLE(_)                               (((_) >> 63) & 0x01)
	};

	UINT64 Flags;
} PT_ENTRY_64;

typedef union _ADDRESS_TRANSLATION_HELPER
{
	//
	// Indexes to locate paging-structure entries corresponds to this virtual
	// address.
	//
	struct
	{
		UINT64 Unused : 12;         //< [11:0]
		UINT64 Pt : 9;              //< [20:12]
		UINT64 Pd : 9;              //< [29:21]
		UINT64 Pdpt : 9;            //< [38:30]
		UINT64 Pml4 : 9;            //< [47:39]
	} AsIndex;

	//
	// The page offset for each type of pages. For example, for 4KB pages, bits
	// [11:0] are treated as the page offset and Mapping4Kb can be used for it.
	//
	union
	{
		UINT64 Mapping4Kb : 12;     //< [11:0]
		UINT64 Mapping2Mb : 21;     //< [20:0]
		UINT64 Mapping1Gb : 30;     //< [29:0]
	} AsPageOffset;

	UINT64 AsUInt64;
} ADDRESS_TRANSLATION_HELPER;

/**
 * @defgroup PAGING_64 \
 *           64-Bit (4-Level) Paging
 *
 * A logical processor uses 4-level paging if CR0.PG = 1, CR4.PAE = 1, and IA32_EFER.LME = 1. With 4-level paging, linear
 * address are translated using a hierarchy of in-memory paging structures located using the contents of CR3. 4-level
 * paging translates 48-bit linear addresses to 52-bit physical addresses. Although 52 bits corresponds to 4 PBytes, linear
 * addresses are limited to 48 bits; at most 256 TBytes of linear-address space may be accessed at any given time.
 * 4-level paging uses a hierarchy of paging structures to produce a translation for a linear address. CR3 is used to
 * locate the first paging-structure, the PML4 table. Use of CR3 with 4-level paging depends on whether processcontext
 * identifiers (PCIDs) have been enabled by setting CR4.PCIDE.
 *
 * @see Vol3A[4.5(4-LEVEL PAGING)] (reference)
 * @{
 */
 /**
  * @brief Format of a 4-Level PML4 Entry (PML4E) that References a Page-Directory-Pointer Table
  */
typedef union
{
	struct
	{
		/**
		 * [Bit 0] Present; must be 1 to reference a page-directory-pointer table.
		 */
		UINT64 Present : 1;
#define PML4E_64_PRESENT_BIT                                         0
#define PML4E_64_PRESENT_FLAG                                        0x01
#define PML4E_64_PRESENT_MASK                                        0x01
#define PML4E_64_PRESENT(_)                                          (((_) >> 0) & 0x01)

		/**
		 * [Bit 1] Read/write; if 0, writes may not be allowed to the 512-GByte region controlled by this entry.
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 Write : 1;
#define PML4E_64_WRITE_BIT                                           1
#define PML4E_64_WRITE_FLAG                                          0x02
#define PML4E_64_WRITE_MASK                                          0x01
#define PML4E_64_WRITE(_)                                            (((_) >> 1) & 0x01)

		/**
		 * [Bit 2] User/supervisor; if 0, user-mode accesses are not allowed to the 512-GByte region controlled by this entry.
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 Supervisor : 1;
#define PML4E_64_SUPERVISOR_BIT                                      2
#define PML4E_64_SUPERVISOR_FLAG                                     0x04
#define PML4E_64_SUPERVISOR_MASK                                     0x01
#define PML4E_64_SUPERVISOR(_)                                       (((_) >> 2) & 0x01)

		/**
		 * [Bit 3] Page-level write-through; indirectly determines the memory type used to access the page-directory-pointer table
		 * referenced by this entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 PageLevelWriteThrough : 1;
#define PML4E_64_PAGE_LEVEL_WRITE_THROUGH_BIT                        3
#define PML4E_64_PAGE_LEVEL_WRITE_THROUGH_FLAG                       0x08
#define PML4E_64_PAGE_LEVEL_WRITE_THROUGH_MASK                       0x01
#define PML4E_64_PAGE_LEVEL_WRITE_THROUGH(_)                         (((_) >> 3) & 0x01)

		/**
		 * [Bit 4] Page-level cache disable; indirectly determines the memory type used to access the page-directory-pointer table
		 * referenced by this entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 PageLevelCacheDisable : 1;
#define PML4E_64_PAGE_LEVEL_CACHE_DISABLE_BIT                        4
#define PML4E_64_PAGE_LEVEL_CACHE_DISABLE_FLAG                       0x10
#define PML4E_64_PAGE_LEVEL_CACHE_DISABLE_MASK                       0x01
#define PML4E_64_PAGE_LEVEL_CACHE_DISABLE(_)                         (((_) >> 4) & 0x01)

		/**
		 * [Bit 5] Accessed; indicates whether this entry has been used for linear-address translation.
		 *
		 * @see Vol3A[4.8(Accessed and Dirty Flags)]
		 */
		UINT64 Accessed : 1;
#define PML4E_64_ACCESSED_BIT                                        5
#define PML4E_64_ACCESSED_FLAG                                       0x20
#define PML4E_64_ACCESSED_MASK                                       0x01
#define PML4E_64_ACCESSED(_)                                         (((_) >> 5) & 0x01)
		UINT64 Reserved1 : 1;

		/**
		 * [Bit 7] Reserved (must be 0).
		 */
		UINT64 MustBeZero : 1;
#define PML4E_64_MUST_BE_ZERO_BIT                                    7
#define PML4E_64_MUST_BE_ZERO_FLAG                                   0x80
#define PML4E_64_MUST_BE_ZERO_MASK                                   0x01
#define PML4E_64_MUST_BE_ZERO(_)                                     (((_) >> 7) & 0x01)

		/**
		 * [Bits 11:8] Ignored.
		 */
		UINT64 Ignored1 : 4;
#define PML4E_64_IGNORED_1_BIT                                       8
#define PML4E_64_IGNORED_1_FLAG                                      0xF00
#define PML4E_64_IGNORED_1_MASK                                      0x0F
#define PML4E_64_IGNORED_1(_)                                        (((_) >> 8) & 0x0F)

		/**
		 * [Bits 47:12] Physical address of 4-KByte aligned page-directory-pointer table referenced by this entry.
		 */
		UINT64 PageFrameNumber : 36;
#define PML4E_64_PAGE_FRAME_NUMBER_BIT                               12
#define PML4E_64_PAGE_FRAME_NUMBER_FLAG                              0xFFFFFFFFF000
#define PML4E_64_PAGE_FRAME_NUMBER_MASK                              0xFFFFFFFFF
#define PML4E_64_PAGE_FRAME_NUMBER(_)                                (((_) >> 12) & 0xFFFFFFFFF)
		UINT64 Reserved2 : 4;

		/**
		 * [Bits 62:52] Ignored.
		 */
		UINT64 Ignored2 : 11;
#define PML4E_64_IGNORED_2_BIT                                       52
#define PML4E_64_IGNORED_2_FLAG                                      0x7FF0000000000000
#define PML4E_64_IGNORED_2_MASK                                      0x7FF
#define PML4E_64_IGNORED_2(_)                                        (((_) >> 52) & 0x7FF)

		/**
		 * [Bit 63] If IA32_EFER.NXE = 1, execute-disable (if 1, instruction fetches are not allowed from the 512-GByte region
		 * controlled by this entry); otherwise, reserved (must be 0).
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 ExecuteDisable : 1;
#define PML4E_64_EXECUTE_DISABLE_BIT                                 63
#define PML4E_64_EXECUTE_DISABLE_FLAG                                0x8000000000000000
#define PML4E_64_EXECUTE_DISABLE_MASK                                0x01
#define PML4E_64_EXECUTE_DISABLE(_)                                  (((_) >> 63) & 0x01)
	};

	UINT64 Flags;
} PML4E_64;

/**
 * @brief Format of a 4-Level Page-Directory-Pointer-Table Entry (PDPTE) that References a Page Directory
 */
typedef union
{
	struct
	{
		/**
		 * [Bit 0] Present; must be 1 to reference a page directory.
		 */
		UINT64 Present : 1;
#define PDPTE_64_PRESENT_BIT                                         0
#define PDPTE_64_PRESENT_FLAG                                        0x01
#define PDPTE_64_PRESENT_MASK                                        0x01
#define PDPTE_64_PRESENT(_)                                          (((_) >> 0) & 0x01)

		/**
		 * [Bit 1] Read/write; if 0, writes may not be allowed to the 1-GByte region controlled by this entry.
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 Write : 1;
#define PDPTE_64_WRITE_BIT                                           1
#define PDPTE_64_WRITE_FLAG                                          0x02
#define PDPTE_64_WRITE_MASK                                          0x01
#define PDPTE_64_WRITE(_)                                            (((_) >> 1) & 0x01)

		/**
		 * [Bit 2] User/supervisor; if 0, user-mode accesses are not allowed to the 1-GByte region controlled by this entry.
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 Supervisor : 1;
#define PDPTE_64_SUPERVISOR_BIT                                      2
#define PDPTE_64_SUPERVISOR_FLAG                                     0x04
#define PDPTE_64_SUPERVISOR_MASK                                     0x01
#define PDPTE_64_SUPERVISOR(_)                                       (((_) >> 2) & 0x01)

		/**
		 * [Bit 3] Page-level write-through; indirectly determines the memory type used to access the page directory referenced by
		 * this entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 PageLevelWriteThrough : 1;
#define PDPTE_64_PAGE_LEVEL_WRITE_THROUGH_BIT                        3
#define PDPTE_64_PAGE_LEVEL_WRITE_THROUGH_FLAG                       0x08
#define PDPTE_64_PAGE_LEVEL_WRITE_THROUGH_MASK                       0x01
#define PDPTE_64_PAGE_LEVEL_WRITE_THROUGH(_)                         (((_) >> 3) & 0x01)

		/**
		 * [Bit 4] Page-level cache disable; indirectly determines the memory type used to access the page directory referenced by
		 * this entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 PageLevelCacheDisable : 1;
#define PDPTE_64_PAGE_LEVEL_CACHE_DISABLE_BIT                        4
#define PDPTE_64_PAGE_LEVEL_CACHE_DISABLE_FLAG                       0x10
#define PDPTE_64_PAGE_LEVEL_CACHE_DISABLE_MASK                       0x01
#define PDPTE_64_PAGE_LEVEL_CACHE_DISABLE(_)                         (((_) >> 4) & 0x01)

		/**
		 * [Bit 5] Accessed; indicates whether this entry has been used for linear-address translation.
		 *
		 * @see Vol3A[4.8(Accessed and Dirty Flags)]
		 */
		UINT64 Accessed : 1;
#define PDPTE_64_ACCESSED_BIT                                        5
#define PDPTE_64_ACCESSED_FLAG                                       0x20
#define PDPTE_64_ACCESSED_MASK                                       0x01
#define PDPTE_64_ACCESSED(_)                                         (((_) >> 5) & 0x01)
		UINT64 Reserved1 : 1;

		/**
		 * [Bit 7] Page size; must be 0 (otherwise, this entry maps a 1-GByte page).
		 */
		UINT64 LargePage : 1;
#define PDPTE_64_LARGE_PAGE_BIT                                      7
#define PDPTE_64_LARGE_PAGE_FLAG                                     0x80
#define PDPTE_64_LARGE_PAGE_MASK                                     0x01
#define PDPTE_64_LARGE_PAGE(_)                                       (((_) >> 7) & 0x01)

		/**
		 * [Bits 11:8] Ignored.
		 */
		UINT64 Ignored1 : 4;
#define PDPTE_64_IGNORED_1_BIT                                       8
#define PDPTE_64_IGNORED_1_FLAG                                      0xF00
#define PDPTE_64_IGNORED_1_MASK                                      0x0F
#define PDPTE_64_IGNORED_1(_)                                        (((_) >> 8) & 0x0F)

		/**
		 * [Bits 47:12] Physical address of 4-KByte aligned page directory referenced by this entry.
		 */
		UINT64 PageFrameNumber : 36;
#define PDPTE_64_PAGE_FRAME_NUMBER_BIT                               12
#define PDPTE_64_PAGE_FRAME_NUMBER_FLAG                              0xFFFFFFFFF000
#define PDPTE_64_PAGE_FRAME_NUMBER_MASK                              0xFFFFFFFFF
#define PDPTE_64_PAGE_FRAME_NUMBER(_)                                (((_) >> 12) & 0xFFFFFFFFF)
		UINT64 Reserved2 : 4;

		/**
		 * [Bits 62:52] Ignored.
		 */
		UINT64 Ignored2 : 11;
#define PDPTE_64_IGNORED_2_BIT                                       52
#define PDPTE_64_IGNORED_2_FLAG                                      0x7FF0000000000000
#define PDPTE_64_IGNORED_2_MASK                                      0x7FF
#define PDPTE_64_IGNORED_2(_)                                        (((_) >> 52) & 0x7FF)

		/**
		 * [Bit 63] If IA32_EFER.NXE = 1, execute-disable (if 1, instruction fetches are not allowed from the 1-GByte region
		 * controlled by this entry); otherwise, reserved (must be 0).
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 ExecuteDisable : 1;
#define PDPTE_64_EXECUTE_DISABLE_BIT                                 63
#define PDPTE_64_EXECUTE_DISABLE_FLAG                                0x8000000000000000
#define PDPTE_64_EXECUTE_DISABLE_MASK                                0x01
#define PDPTE_64_EXECUTE_DISABLE(_)                                  (((_) >> 63) & 0x01)
	};

	UINT64 Flags;
} PDPTE_64;

/**
 * @brief Format of a 4-Level Page-Directory Entry that References a Page Table
 */
typedef union
{
	struct
	{
		/**
		 * [Bit 0] Present; must be 1 to reference a page table.
		 */
		UINT64 Present : 1;
#define PDE_64_PRESENT_BIT                                           0
#define PDE_64_PRESENT_FLAG                                          0x01
#define PDE_64_PRESENT_MASK                                          0x01
#define PDE_64_PRESENT(_)                                            (((_) >> 0) & 0x01)

		/**
		 * [Bit 1] Read/write; if 0, writes may not be allowed to the 2-MByte region controlled by this entry.
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 Write : 1;
#define PDE_64_WRITE_BIT                                             1
#define PDE_64_WRITE_FLAG                                            0x02
#define PDE_64_WRITE_MASK                                            0x01
#define PDE_64_WRITE(_)                                              (((_) >> 1) & 0x01)

		/**
		 * [Bit 2] User/supervisor; if 0, user-mode accesses are not allowed to the 2-MByte region controlled by this entry.
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 Supervisor : 1;
#define PDE_64_SUPERVISOR_BIT                                        2
#define PDE_64_SUPERVISOR_FLAG                                       0x04
#define PDE_64_SUPERVISOR_MASK                                       0x01
#define PDE_64_SUPERVISOR(_)                                         (((_) >> 2) & 0x01)

		/**
		 * [Bit 3] Page-level write-through; indirectly determines the memory type used to access the page table referenced by this
		 * entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 PageLevelWriteThrough : 1;
#define PDE_64_PAGE_LEVEL_WRITE_THROUGH_BIT                          3
#define PDE_64_PAGE_LEVEL_WRITE_THROUGH_FLAG                         0x08
#define PDE_64_PAGE_LEVEL_WRITE_THROUGH_MASK                         0x01
#define PDE_64_PAGE_LEVEL_WRITE_THROUGH(_)                           (((_) >> 3) & 0x01)

		/**
		 * [Bit 4] Page-level cache disable; indirectly determines the memory type used to access the page table referenced by this
		 * entry.
		 *
		 * @see Vol3A[4.9.2(Paging and Memory Typing When the PAT is Supported (Pentium III and More Recent Processor Families))]
		 */
		UINT64 PageLevelCacheDisable : 1;
#define PDE_64_PAGE_LEVEL_CACHE_DISABLE_BIT                          4
#define PDE_64_PAGE_LEVEL_CACHE_DISABLE_FLAG                         0x10
#define PDE_64_PAGE_LEVEL_CACHE_DISABLE_MASK                         0x01
#define PDE_64_PAGE_LEVEL_CACHE_DISABLE(_)                           (((_) >> 4) & 0x01)

		/**
		 * [Bit 5] Accessed; indicates whether this entry has been used for linear-address translation.
		 *
		 * @see Vol3A[4.8(Accessed and Dirty Flags)]
		 */
		UINT64 Accessed : 1;
#define PDE_64_ACCESSED_BIT                                          5
#define PDE_64_ACCESSED_FLAG                                         0x20
#define PDE_64_ACCESSED_MASK                                         0x01
#define PDE_64_ACCESSED(_)                                           (((_) >> 5) & 0x01)
		UINT64 Reserved1 : 1;

		/**
		 * [Bit 7] Page size; must be 0 (otherwise, this entry maps a 2-MByte page).
		 */
		UINT64 LargePage : 1;
#define PDE_64_LARGE_PAGE_BIT                                        7
#define PDE_64_LARGE_PAGE_FLAG                                       0x80
#define PDE_64_LARGE_PAGE_MASK                                       0x01
#define PDE_64_LARGE_PAGE(_)                                         (((_) >> 7) & 0x01)

		/**
		 * [Bits 11:8] Ignored.
		 */
		UINT64 Ignored1 : 4;
#define PDE_64_IGNORED_1_BIT                                         8
#define PDE_64_IGNORED_1_FLAG                                        0xF00
#define PDE_64_IGNORED_1_MASK                                        0x0F
#define PDE_64_IGNORED_1(_)                                          (((_) >> 8) & 0x0F)

		/**
		 * [Bits 47:12] Physical address of 4-KByte aligned page table referenced by this entry.
		 */
		UINT64 PageFrameNumber : 36;
#define PDE_64_PAGE_FRAME_NUMBER_BIT                                 12
#define PDE_64_PAGE_FRAME_NUMBER_FLAG                                0xFFFFFFFFF000
#define PDE_64_PAGE_FRAME_NUMBER_MASK                                0xFFFFFFFFF
#define PDE_64_PAGE_FRAME_NUMBER(_)                                  (((_) >> 12) & 0xFFFFFFFFF)
		UINT64 Reserved2 : 4;

		/**
		 * [Bits 62:52] Ignored.
		 */
		UINT64 Ignored2 : 11;
#define PDE_64_IGNORED_2_BIT                                         52
#define PDE_64_IGNORED_2_FLAG                                        0x7FF0000000000000
#define PDE_64_IGNORED_2_MASK                                        0x7FF
#define PDE_64_IGNORED_2(_)                                          (((_) >> 52) & 0x7FF)

		/**
		 * [Bit 63] If IA32_EFER.NXE = 1, execute-disable (if 1, instruction fetches are not allowed from the 2-MByte region
		 * controlled by this entry); otherwise, reserved (must be 0).
		 *
		 * @see Vol3A[4.6(Access Rights)]
		 */
		UINT64 ExecuteDisable : 1;
#define PDE_64_EXECUTE_DISABLE_BIT                                   63
#define PDE_64_EXECUTE_DISABLE_FLAG                                  0x8000000000000000
#define PDE_64_EXECUTE_DISABLE_MASK                                  0x01
#define PDE_64_EXECUTE_DISABLE(_)                                    (((_) >> 63) & 0x01)
	};

	UINT64 Flags;
} PDE_64;

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation = 0,
	SystemProcessorInformation = 1,
	SystemPerformanceInformation = 2,
	SystemTimeOfDayInformation = 3,
	SystemPathInformation = 4,
	SystemProcessInformation = 5,
	SystemCallCountInformation = 6,
	SystemDeviceInformation = 7,
	SystemProcessorPerformanceInformation = 8,
	SystemFlagsInformation = 9,
	SystemCallTimeInformation = 10,
	SystemModuleInformation = 11,
	SystemLocksInformation = 12,
	SystemStackTraceInformation = 13,
	SystemPagedPoolInformation = 14,
	SystemNonPagedPoolInformation = 15,
	SystemHandleInformation = 16,
	SystemObjectInformation = 17,
	SystemPageFileInformation = 18,
	SystemVdmInstemulInformation = 19,
	SystemVdmBopInformation = 20,
	SystemFileCacheInformation = 21,
	SystemPoolTagInformation = 22,
	SystemInterruptInformation = 23,
	SystemDpcBehaviorInformation = 24,
	SystemFullMemoryInformation = 25,
	SystemLoadGdiDriverInformation = 26,
	SystemUnloadGdiDriverInformation = 27,
	SystemTimeAdjustmentInformation = 28,
	SystemSummaryMemoryInformation = 29,
	SystemMirrorMemoryInformation = 30,
	SystemPerformanceTraceInformation = 31,
	SystemObsolete0 = 32,
	SystemExceptionInformation = 33,
	SystemCrashDumpStateInformation = 34,
	SystemKernelDebuggerInformation = 35,
	SystemContextSwitchInformation = 36,
	SystemRegistryQuotaInformation = 37,
	SystemExtendServiceTableInformation = 38,
	SystemPrioritySeperation = 39,
	SystemVerifierAddDriverInformation = 40,
	SystemVerifierRemoveDriverInformation = 41,
	SystemProcessorIdleInformation = 42,
	SystemLegacyDriverInformation = 43,
	SystemCurrentTimeZoneInformation = 44,
	SystemLookasideInformation = 45,
	SystemTimeSlipNotification = 46,
	SystemSessionCreate = 47,
	SystemSessionDetach = 48,
	SystemSessionInformation = 49,
	SystemRangeStartInformation = 50,
	SystemVerifierInformation = 51,
	SystemVerifierThunkExtend = 52,
	SystemSessionProcessInformation = 53,
	SystemLoadGdiDriverInSystemSpace = 54,
	SystemNumaProcessorMap = 55,
	SystemPrefetcherInformation = 56,
	SystemExtendedProcessInformation = 57,
	SystemRecommendedSharedDataAlignment = 58,
	SystemComPlusPackage = 59,
	SystemNumaAvailableMemory = 60,
	SystemProcessorPowerInformation = 61,
	SystemEmulationBasicInformation = 62,
	SystemEmulationProcessorInformation = 63,
	SystemExtendedHandleInformation = 64,
	SystemLostDelayedWriteInformation = 65,
	SystemBigPoolInformation = 66,
	SystemSessionPoolTagInformation = 67,
	SystemSessionMappedViewInformation = 68,
	SystemHotpatchInformation = 69,
	SystemObjectSecurityMode = 70,
	SystemWatchdogTimerHandler = 71,
	SystemWatchdogTimerInformation = 72,
	SystemLogicalProcessorInformation = 73,
	SystemWow64SharedInformation = 74,
	SystemRegisterFirmwareTableInformationHandler = 75,
	SystemFirmwareTableInformation = 76,
	SystemModuleInformationEx = 77,
	SystemVerifierTriageInformation = 78,
	SystemSuperfetchInformation = 79,
	SystemMemoryListInformation = 80,
	SystemFileCacheInformationEx = 81,
	MaxSystemInfoClass = 82
} SYSTEM_INFORMATION_CLASS, * PSYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_MODULE
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[MAXIMUM_FILENAME_LENGTH];
} SYSTEM_MODULE, * PSYSTEM_MODULE;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG NumberOfModules;
	SYSTEM_MODULE Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	LIST_ENTRY HashLinks;
	PVOID SectionPointer;
	ULONG CheckSum;
	ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _KLDR_DATA_TABLE_ENTRY
{
	struct _LIST_ENTRY InLoadOrderLinks;
	VOID* ExceptionTable;
	UINT32 ExceptionTableSize;
	VOID* GpValue;
	struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;
	VOID* DllBase;
	VOID* EntryPoint;
	UINT32 SizeOfImage;
	struct _UNICODE_STRING FullDllName;
	struct _UNICODE_STRING BaseDllName;
	UINT32 Flags;
	UINT16 LoadCount;
	union
	{
		UINT16 SignatureLevel : 4;
		UINT16 SignatureType : 3;
		UINT16 Unused : 9;
		UINT16 EntireField;
	} u1;
	VOID* SectionPointer;
	UINT32 CheckSum;
	UINT32 CoverageSectionSize;
	VOID* CoverageSection;
	VOID* LoadedImports;
	VOID* Spare;
	UINT32 SizeOfImageNotRounded;
	UINT32 TimeDateStamp;
} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;

typedef struct _SYSTEM_BIGPOOL_ENTRY
{
	union {
		PVOID VirtualAddress;
		ULONG_PTR NonPaged : 1;
	};
	ULONG_PTR SizeInBytes;
	union {
		UCHAR Tag[4];
		ULONG TagUlong;
	};
} SYSTEM_BIGPOOL_ENTRY, * PSYSTEM_BIGPOOL_ENTRY;

typedef struct _SYSTEM_BIGPOOL_INFORMATION {
	ULONG Count;
	SYSTEM_BIGPOOL_ENTRY AllocatedInfo[ANYSIZE_ARRAY];
} SYSTEM_BIGPOOL_INFORMATION, * PSYSTEM_BIGPOOL_INFORMATION;

typedef struct _PEB32
{
	UCHAR InheritedAddressSpace;
	UCHAR ReadImageFileExecOptions;
	UCHAR BeingDebugged;
	UCHAR BitField;
	ULONG Mutant;
	ULONG ImageBaseAddress;
	ULONG Ldr;
	ULONG ProcessParameters;
	ULONG SubSystemData;
	ULONG ProcessHeap;
	ULONG FastPebLock;
	ULONG AtlThunkSListPtr;
	ULONG IFEOKey;
	ULONG CrossProcessFlags;
	ULONG UserSharedInfoPtr;
	ULONG SystemReserved;
	ULONG AtlThunkSListPtr32;
	ULONG ApiSetMap;
} PEB32, * PPEB32;

typedef struct _RTL_CRITICAL_SECTION
{
	VOID* DebugInfo;
	LONG LockCount;
	LONG RecursionCount;
	PVOID OwningThread;
	PVOID LockSemaphore;
	ULONG SpinCount;
} RTL_CRITICAL_SECTION, * PRTL_CRITICAL_SECTION;

typedef struct _LDR_DATA_TABLE_ENTRY32
{
	LIST_ENTRY32 InLoadOrderLinks;
	LIST_ENTRY32 InMemoryOrderLinks;
	LIST_ENTRY32 InInitializationOrderLinks;
	ULONG DllBase;
	ULONG EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING32 FullDllName;
	UNICODE_STRING32 BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	LIST_ENTRY32 HashLinks;
	ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY32, * PLDR_DATA_TABLE_ENTRY32;

typedef struct _PEB_LDR_DATA32
{
	ULONG Length;
	UCHAR Initialized;
	ULONG SsHandle;
	LIST_ENTRY32 InLoadOrderModuleList;
	LIST_ENTRY32 InMemoryOrderModuleList;
	LIST_ENTRY32 InInitializationOrderModuleList;
} PEB_LDR_DATA32, * PPEB_LDR_DATA32;

typedef struct _PEB_LDR_DATA {
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY ModuleListLoadOrder;
	LIST_ENTRY ModuleListMemoryOrder;
	LIST_ENTRY ModuleListInitOrder;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB
{
	UCHAR InheritedAddressSpace;
	UCHAR ReadImageFileExecOptions;
	UCHAR BeingDebugged;
	UCHAR BitField;
	ULONG ImageUsesLargePages : 1;
	ULONG IsProtectedProcess : 1;
	ULONG IsLegacyProcess : 1;
	ULONG IsImageDynamicallyRelocated : 1;
	ULONG SpareBits : 4;
	PVOID Mutant;
	PVOID ImageBaseAddress;
	PPEB_LDR_DATA Ldr;
	VOID* ProcessParameters;
	PVOID SubSystemData;
	PVOID ProcessHeap;
	PRTL_CRITICAL_SECTION FastPebLock;
	PVOID AtlThunkSListPtr;
	PVOID IFEOKey;
	ULONG CrossProcessFlags;
	ULONG ProcessInJob : 1;
	ULONG ProcessInitializing : 1;
	ULONG ReservedBits0 : 30;
	union
	{
		PVOID KernelCallbackTable;
		PVOID UserSharedInfoPtr;
	};
	ULONG SystemReserved[1];
	ULONG SpareUlong;
	VOID* FreeList;
	ULONG TlsExpansionCounter;
	PVOID TlsBitmap;
	ULONG TlsBitmapBits[2];
	PVOID ReadOnlySharedMemoryBase;
	PVOID HotpatchInformation;
	VOID** ReadOnlyStaticServerData;
	PVOID AnsiCodePageData;
	PVOID OemCodePageData;
	PVOID UnicodeCaseTableData;
	ULONG NumberOfProcessors;
	ULONG NtGlobalFlag;
	LARGE_INTEGER CriticalSectionTimeout;
	ULONG HeapSegmentReserve;
	ULONG HeapSegmentCommit;
	ULONG HeapDeCommitTotalFreeThreshold;
	ULONG HeapDeCommitFreeBlockThreshold;
	ULONG NumberOfHeaps;
	ULONG MaximumNumberOfHeaps;
	VOID** ProcessHeaps;
	PVOID GdiSharedHandleTable;
	PVOID ProcessStarterHelper;
	ULONG GdiDCAttributeList;
	PRTL_CRITICAL_SECTION LoaderLock;
	ULONG OSMajorVersion;
	ULONG OSMinorVersion;
	unsigned short OSBuildNumber;
	unsigned short OSCSDVersion;
	ULONG OSPlatformId;
	ULONG ImageSubsystem;
	ULONG ImageSubsystemMajorVersion;
	ULONG ImageSubsystemMinorVersion;
	ULONG ImageProcessAffinityMask;
	ULONG GdiHandleBuffer[34];
	PVOID PostProcessInitRoutine;
	PVOID TlsExpansionBitmap;
	ULONG TlsExpansionBitmapBits[32];
	ULONG SessionId;
	ULARGE_INTEGER AppCompatFlags;
	ULARGE_INTEGER AppCompatFlagsUser;
	PVOID pShimData;
	PVOID AppCompatInfo;
	UNICODE_STRING CSDVersion;
	VOID* ActivationContextData;
	VOID* ProcessAssemblyStorageMap;
	VOID* SystemDefaultActivationContextData;
	VOID* SystemAssemblyStorageMap;
	ULONG MinimumStackCommit;
	VOID* FlsCallback;
	LIST_ENTRY FlsListHead;
	PVOID FlsBitmap;
	ULONG FlsBitmapBits[4];
	ULONG FlsHighIndex;
	PVOID WerRegistrationData;
	PVOID WerShipAssertPtr;
} PEB, * PPEB;

typedef struct _MDL_INFORMATION
{
	MDL* mdl;
	uintptr_t va;
}MDL_INFORMATION, * PMDL_INFORMATION;

typedef union _VIRTUAL_ADDRESS
{
	PVOID value;
	struct
	{
		ULONG64 offset : 12;
		ULONG64 pt_index : 9;
		ULONG64 pd_index : 9;
		ULONG64 pdpt_index : 9;
		ULONG64 pml4_index : 9;
		ULONG64 reserved : 16;
	};
} VIRTUAL_ADDRESS, * PVIRTUAL_ADDRESS;
struct _EX_PUSH_LOCK
{
	union
	{
		struct
		{
			ULONG Locked : 1;                                                 //0x0
			ULONG Waiting : 1;                                                //0x0
			ULONG Waking : 1;                                                 //0x0
			ULONG MultipleShared : 1;                                         //0x0
			ULONG Shared : 28;                                                //0x0
		};
		ULONG Value;                                                        //0x0
		VOID* Ptr;                                                          //0x0
	};
};

typedef struct _MMVAD_FLAGS
{
	ULONG Lock : 1;
	ULONG LockContended : 1;
	ULONG DeleteInProgress : 1;
	ULONG NoChange : 1;
	ULONG VadType : 3;
	ULONG Protection : 5;
	ULONG PreferredNode : 6;
	ULONG PageSize : 2;
	ULONG PrivateMemory : 1;
} MMVAD_FLAGS, * PMMVAD_FLAGS;

struct _MMVAD_FLAGS1
{
	unsigned long CommitCharge : 31;
	unsigned long MemCommit : 1;
};

struct _MMVAD_FLAGS2
{
	unsigned long FileOffset : 24;
	unsigned long Large : 1;
	unsigned long TrimBehind : 1;
	unsigned long Inherit : 1;
	unsigned long CopyOnWrite : 1;
	unsigned long NoValidationNeeded : 1;
	unsigned long Spare : 3;
};

union ___unnamed1952 // Size=4
{
	unsigned long LongFlags1; // Size=4 Offset=0
	struct _MMVAD_FLAGS1 VadFlags1; // Size=4 Offset=0
};
union ___unnamed1951 // Size=4
{
	unsigned long LongFlags; // Size=4 Offset=0
	struct _MMVAD_FLAGS VadFlags; // Size=4 Offset=0
};
typedef struct _MMVAD_SHORT
{
	union
	{
		struct
		{
			struct _MMVAD_SHORT* NextVad;
			VOID* ExtraCreateInfo;
		};
		struct _RTL_BALANCED_NODE VadNode;
	};
	ULONG StartingVpn;
	ULONG EndingVpn;
	UCHAR StartingVpnHigh;
	UCHAR EndingVpnHigh;
	UCHAR CommitChargeHigh;
	UCHAR SpareNT64VadUChar;
	LONG ReferenceCount;
	_EX_PUSH_LOCK PushLock;
	union ___unnamed1951 u;
	union ___unnamed1952 u1;
	struct _MI_VAD_EVENT_BLOCK* EventList;
} MMVAD_SHORT, * PMMVAD_SHORT;

typedef struct _MM_AVL_NODE // Size=24
{
	struct _MM_AVL_NODE* LeftChild; // Size=8 Offset=0
	struct _MM_AVL_NODE* RightChild; // Size=8 Offset=8

	union ___unnamed1666 // Size=8
	{
		struct
		{
			__int64 Balance : 2; // Size=8 Offset=0 BitOffset=0 BitCount=2
		};
		struct _MM_AVL_NODE* Parent; // Size=8 Offset=0
	} u1;
} MM_AVL_NODE, * PMM_AVL_NODE, * PMMADDRESS_NODE;

typedef struct _RTL_AVL_TREE // Size=8
{
	PMM_AVL_NODE BalancedRoot;
	void* NodeHint;
	unsigned __int64 NumberGenericTableElements;
} RTL_AVL_TREE, * PRTL_AVL_TREE, MM_AVL_TABLE, * PMM_AVL_TABLE;

typedef struct _MI_ACTIVE_PFN
{
	union
	{
		struct
		{
			struct /* bitfield */
			{
				/* 0x0000 */ unsigned __int64 Tradable : 1; /* bit position: 0 */
				/* 0x0000 */ unsigned __int64 NonPagedBuddy : 43; /* bit position: 1 */
			}; /* bitfield */
		} /* size: 0x0008 */ Leaf;
		struct
		{
			struct /* bitfield */
			{
				/* 0x0000 */ unsigned __int64 Tradable : 1; /* bit position: 0 */
				/* 0x0000 */ unsigned __int64 WsleAge : 3; /* bit position: 1 */
				/* 0x0000 */ unsigned __int64 OldestWsleLeafEntries : 10; /* bit position: 4 */
				/* 0x0000 */ unsigned __int64 OldestWsleLeafAge : 3; /* bit position: 14 */
				/* 0x0000 */ unsigned __int64 NonPagedBuddy : 43; /* bit position: 17 */
			}; /* bitfield */
		} /* size: 0x0008 */ PageTable;
		/* 0x0000 */ unsigned __int64 EntireActiveField;
	}; /* size: 0x0008 */
} MI_ACTIVE_PFN, * PMI_ACTIVE_PFN; /* size: 0x0008 */

typedef struct _MMPTE_HARDWARE
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
		/* 0x0000 */ unsigned __int64 Dirty1 : 1; /* bit position: 1 */
		/* 0x0000 */ unsigned __int64 Owner : 1; /* bit position: 2 */
		/* 0x0000 */ unsigned __int64 WriteThrough : 1; /* bit position: 3 */
		/* 0x0000 */ unsigned __int64 CacheDisable : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned __int64 Accessed : 1; /* bit position: 5 */
		/* 0x0000 */ unsigned __int64 Dirty : 1; /* bit position: 6 */
		/* 0x0000 */ unsigned __int64 LargePage : 1; /* bit position: 7 */
		/* 0x0000 */ unsigned __int64 Global : 1; /* bit position: 8 */
		/* 0x0000 */ unsigned __int64 CopyOnWrite : 1; /* bit position: 9 */
		/* 0x0000 */ unsigned __int64 Unused : 1; /* bit position: 10 */
		/* 0x0000 */ unsigned __int64 Write : 1; /* bit position: 11 */
		/* 0x0000 */ unsigned __int64 PageFrameNumber : 40; /* bit position: 12 */
		/* 0x0000 */ unsigned __int64 ReservedForSoftware : 4; /* bit position: 52 */
		/* 0x0000 */ unsigned __int64 WsleAge : 4; /* bit position: 56 */
		/* 0x0000 */ unsigned __int64 WsleProtection : 3; /* bit position: 60 */
		/* 0x0000 */ unsigned __int64 NoExecute : 1; /* bit position: 63 */
	}; /* bitfield */
} MMPTE_HARDWARE, * PMMPTE_HARDWARE; /* size: 0x0008 */

typedef struct _MMPTE_PROTOTYPE
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
		/* 0x0000 */ unsigned __int64 DemandFillProto : 1; /* bit position: 1 */
		/* 0x0000 */ unsigned __int64 HiberVerifyConverted : 1; /* bit position: 2 */
		/* 0x0000 */ unsigned __int64 ReadOnly : 1; /* bit position: 3 */
		/* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
		/* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
		/* 0x0000 */ unsigned __int64 Combined : 1; /* bit position: 11 */
		/* 0x0000 */ unsigned __int64 Unused1 : 4; /* bit position: 12 */
		/* 0x0000 */ __int64 ProtoAddress : 48; /* bit position: 16 */
	}; /* bitfield */
} MMPTE_PROTOTYPE, * PMMPTE_PROTOTYPE; /* size: 0x0008 */

typedef struct _MMPTE_SOFTWARE
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
		/* 0x0000 */ unsigned __int64 PageFileReserved : 1; /* bit position: 1 */
		/* 0x0000 */ unsigned __int64 PageFileAllocated : 1; /* bit position: 2 */
		/* 0x0000 */ unsigned __int64 ColdPage : 1; /* bit position: 3 */
		/* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
		/* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
		/* 0x0000 */ unsigned __int64 Transition : 1; /* bit position: 11 */
		/* 0x0000 */ unsigned __int64 PageFileLow : 4; /* bit position: 12 */
		/* 0x0000 */ unsigned __int64 UsedPageTableEntries : 10; /* bit position: 16 */
		/* 0x0000 */ unsigned __int64 ShadowStack : 1; /* bit position: 26 */
		/* 0x0000 */ unsigned __int64 Unused : 5; /* bit position: 27 */
		/* 0x0000 */ unsigned __int64 PageFileHigh : 32; /* bit position: 32 */
	}; /* bitfield */
} MMPTE_SOFTWARE, * PMMPTE_SOFTWARE; /* size: 0x0008 */

typedef struct _MMPTE_TIMESTAMP
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned __int64 MustBeZero : 1; /* bit position: 0 */
		/* 0x0000 */ unsigned __int64 Unused : 3; /* bit position: 1 */
		/* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
		/* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
		/* 0x0000 */ unsigned __int64 Transition : 1; /* bit position: 11 */
		/* 0x0000 */ unsigned __int64 PageFileLow : 4; /* bit position: 12 */
		/* 0x0000 */ unsigned __int64 Reserved : 16; /* bit position: 16 */
		/* 0x0000 */ unsigned __int64 GlobalTimeStamp : 32; /* bit position: 32 */
	}; /* bitfield */
} MMPTE_TIMESTAMP, * PMMPTE_TIMESTAMP; /* size: 0x0008 */

typedef struct _MMPTE_TRANSITION
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
		/* 0x0000 */ unsigned __int64 Write : 1; /* bit position: 1 */
		/* 0x0000 */ unsigned __int64 Spare : 1; /* bit position: 2 */
		/* 0x0000 */ unsigned __int64 IoTracker : 1; /* bit position: 3 */
		/* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
		/* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
		/* 0x0000 */ unsigned __int64 Transition : 1; /* bit position: 11 */
		/* 0x0000 */ unsigned __int64 PageFrameNumber : 40; /* bit position: 12 */
		/* 0x0000 */ unsigned __int64 Unused : 12; /* bit position: 52 */
	}; /* bitfield */
} MMPTE_TRANSITION, * PMMPTE_TRANSITION; /* size: 0x0008 */

typedef struct _MMPTE_SUBSECTION
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
		/* 0x0000 */ unsigned __int64 Unused0 : 3; /* bit position: 1 */
		/* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
		/* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
		/* 0x0000 */ unsigned __int64 ColdPage : 1; /* bit position: 11 */
		/* 0x0000 */ unsigned __int64 Unused1 : 3; /* bit position: 12 */
		/* 0x0000 */ unsigned __int64 ExecutePrivilege : 1; /* bit position: 15 */
		/* 0x0000 */ __int64 SubsectionAddress : 48; /* bit position: 16 */
	}; /* bitfield */
} MMPTE_SUBSECTION, * PMMPTE_SUBSECTION; /* size: 0x0008 */

typedef struct _MMPTE_LIST
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned __int64 Valid : 1; /* bit position: 0 */
		/* 0x0000 */ unsigned __int64 OneEntry : 1; /* bit position: 1 */
		/* 0x0000 */ unsigned __int64 filler0 : 2; /* bit position: 2 */
		/* 0x0000 */ unsigned __int64 SwizzleBit : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned __int64 Protection : 5; /* bit position: 5 */
		/* 0x0000 */ unsigned __int64 Prototype : 1; /* bit position: 10 */
		/* 0x0000 */ unsigned __int64 Transition : 1; /* bit position: 11 */
		/* 0x0000 */ unsigned __int64 filler1 : 16; /* bit position: 12 */
		/* 0x0000 */ unsigned __int64 NextEntry : 36; /* bit position: 28 */
	}; /* bitfield */
} MMPTE_LIST, * PMMPTE_LIST; /* size: 0x0008 */

typedef struct _MMPTE
{
	union
	{
		union
		{
			/* 0x0000 */ unsigned __int64 Long;
			/* 0x0000 */ volatile unsigned __int64 VolatileLong;
			/* 0x0000 */ struct _MMPTE_HARDWARE Hard;
			/* 0x0000 */ struct _MMPTE_PROTOTYPE Proto;
			/* 0x0000 */ struct _MMPTE_SOFTWARE Soft;
			/* 0x0000 */ struct _MMPTE_TIMESTAMP TimeStamp;
			/* 0x0000 */ struct _MMPTE_TRANSITION Trans;
			/* 0x0000 */ struct _MMPTE_SUBSECTION Subsect;
			/* 0x0000 */ struct _MMPTE_LIST List;
		}; /* size: 0x0008 */
	} /* size: 0x0008 */ u;
} MMPTE, * PMMPTE; /* size: 0x0008 */

typedef struct _MIPFNBLINK
{
	union
	{
		struct /* bitfield */
		{
			/* 0x0000 */ unsigned __int64 Blink : 40; /* bit position: 0 */
			/* 0x0000 */ unsigned __int64 NodeBlinkLow : 19; /* bit position: 40 */
			/* 0x0000 */ unsigned __int64 TbFlushStamp : 3; /* bit position: 59 */
			/* 0x0000 */ unsigned __int64 PageBlinkDeleteBit : 1; /* bit position: 62 */
			/* 0x0000 */ unsigned __int64 PageBlinkLockBit : 1; /* bit position: 63 */
		}; /* bitfield */
		struct /* bitfield */
		{
			/* 0x0000 */ unsigned __int64 ShareCount : 62; /* bit position: 0 */
			/* 0x0000 */ unsigned __int64 PageShareCountDeleteBit : 1; /* bit position: 62 */
			/* 0x0000 */ unsigned __int64 PageShareCountLockBit : 1; /* bit position: 63 */
		}; /* bitfield */
		/* 0x0000 */ unsigned __int64 EntireField;
		/* 0x0000 */ volatile __int64 Lock;
		struct /* bitfield */
		{
			/* 0x0000 */ unsigned __int64 LockNotUsed : 62; /* bit position: 0 */
			/* 0x0000 */ unsigned __int64 DeleteBit : 1; /* bit position: 62 */
			/* 0x0000 */ unsigned __int64 LockBit : 1; /* bit position: 63 */
		}; /* bitfield */
	}; /* size: 0x0008 */
} MIPFNBLINK, * PMIPFNBLINK; /* size: 0x0008 */

typedef struct _MMPFNENTRY1
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned char PageLocation : 3; /* bit position: 0 */
		/* 0x0000 */ unsigned char WriteInProgress : 1; /* bit position: 3 */
		/* 0x0000 */ unsigned char Modified : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned char ReadInProgress : 1; /* bit position: 5 */
		/* 0x0000 */ unsigned char CacheAttribute : 2; /* bit position: 6 */
	}; /* bitfield */
} MMPFNENTRY1, * PMMPFNENTRY1; /* size: 0x0001 */

typedef struct _MMPFNENTRY3
{
	struct /* bitfield */
	{
		/* 0x0000 */ unsigned char Priority : 3; /* bit position: 0 */
		/* 0x0000 */ unsigned char OnProtectedStandby : 1; /* bit position: 3 */
		/* 0x0000 */ unsigned char InPageError : 1; /* bit position: 4 */
		/* 0x0000 */ unsigned char SystemChargedPage : 1; /* bit position: 5 */
		/* 0x0000 */ unsigned char RemovalRequested : 1; /* bit position: 6 */
		/* 0x0000 */ unsigned char ParityError : 1; /* bit position: 7 */
	}; /* bitfield */
} MMPFNENTRY3, * PMMPFNENTRY3; /* size: 0x0001 */

typedef struct _MI_PFN_ULONG5
{
	union
	{
		/* 0x0000 */ unsigned long EntireField;
		struct
		{
			struct /* bitfield */
			{
				/* 0x0000 */ unsigned long NodeBlinkHigh : 21; /* bit position: 0 */
				/* 0x0000 */ unsigned long NodeFlinkMiddle : 11; /* bit position: 21 */
			}; /* bitfield */
		} /* size: 0x0004 */ StandbyList;
		struct
		{
			/* 0x0000 */ unsigned char ModifiedListBucketIndex : 4; /* bit position: 0 */
		} /* size: 0x0001 */ MappedPageList;
		struct
		{
			struct /* bitfield */
			{
				/* 0x0000 */ unsigned char AnchorLargePageSize : 2; /* bit position: 0 */
				/* 0x0000 */ unsigned char Spare1 : 6; /* bit position: 2 */
			}; /* bitfield */
			/* 0x0001 */ unsigned char ViewCount;
			/* 0x0002 */ unsigned short Spare2;
		} /* size: 0x0004 */ Active;
	}; /* size: 0x0004 */
} MI_PFN_ULONG5, * PMI_PFN_ULONG5; /* size: 0x0004 */

typedef struct _MMPFN
{
	union
	{
		/* 0x0000 */ struct _LIST_ENTRY ListEntry;
		/* 0x0000 */ struct _RTL_BALANCED_NODE TreeNode;
		struct
		{
			union
			{
				union
				{
					/* 0x0000 */ struct _SINGLE_LIST_ENTRY NextSlistPfn;
					/* 0x0000 */ void* Next;
					struct /* bitfield */
					{
						/* 0x0000 */ unsigned __int64 Flink : 40; /* bit position: 0 */
						/* 0x0000 */ unsigned __int64 NodeFlinkLow : 24; /* bit position: 40 */
					}; /* bitfield */
					/* 0x0000 */ struct _MI_ACTIVE_PFN Active;
				}; /* size: 0x0008 */
			} /* size: 0x0008 */ u1;
			union
			{
				/* 0x0008 */ struct _MMPTE* PteAddress;
				/* 0x0008 */ unsigned __int64 PteLong;
			}; /* size: 0x0008 */
			/* 0x0010 */ struct _MMPTE OriginalPte;
		}; /* size: 0x0018 */
	}; /* size: 0x0018 */
	/* 0x0018 */ struct _MIPFNBLINK u2;
	union
	{
		union
		{
			struct
			{
				/* 0x0020 */ unsigned short ReferenceCount;
				/* 0x0022 */ struct _MMPFNENTRY1 e1;
				/* 0x0023 */ struct _MMPFNENTRY3 e3;
			}; /* size: 0x0004 */
			struct
			{
				/* 0x0020 */ unsigned short ReferenceCount;
			} /* size: 0x0002 */ e2;
			struct
			{
				/* 0x0020 */ unsigned long EntireField;
			} /* size: 0x0004 */ e4;
		}; /* size: 0x0004 */
	} /* size: 0x0004 */ u3;
	/* 0x0024 */ struct _MI_PFN_ULONG5 u5;
	union
	{
		union
		{
			struct /* bitfield */
			{
				/* 0x0028 */ unsigned __int64 PteFrame : 40; /* bit position: 0 */
				/* 0x0028 */ unsigned __int64 ResidentPage : 1; /* bit position: 40 */
				/* 0x0028 */ unsigned __int64 Unused1 : 1; /* bit position: 41 */
				/* 0x0028 */ unsigned __int64 Unused2 : 1; /* bit position: 42 */
				/* 0x0028 */ unsigned __int64 Partition : 10; /* bit position: 43 */
				/* 0x0028 */ unsigned __int64 FileOnly : 1; /* bit position: 53 */
				/* 0x0028 */ unsigned __int64 PfnExists : 1; /* bit position: 54 */
				/* 0x0028 */ unsigned __int64 NodeFlinkHigh : 5; /* bit position: 55 */
				/* 0x0028 */ unsigned __int64 PageIdentity : 3; /* bit position: 60 */
				/* 0x0028 */ unsigned __int64 PrototypePte : 1; /* bit position: 63 */
			}; /* bitfield */
			/* 0x0028 */ unsigned __int64 EntireField;
		}; /* size: 0x0008 */
	} /* size: 0x0008 */ u4;
} MMPFN, * PMMPFN; /* size: 0x0030 */