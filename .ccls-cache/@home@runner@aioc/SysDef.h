/*******************************************************************************
 *
 *  File Name   :  SysDef.h
 *
 *  Version     :  SVN $Revision:: 530    $
 *
 *  Description :  Common definitions and declarations in the system
 *
 *  Author      :  Mo, Kenneth
 *
 *  Note        :  Curtiss-Wright Controls, Inc. - All right reserved
 *
 *
 *  Date      PCR#  User         Change Description
 *  --------  ----  -----------  -----------------------------------------------
 *  03/11/09        kmo          Initial creation
 *  05/27/11  414   J.Squiers    Added SET_BIT and CLR_BIT macros.
 *  09/12/11  678   Z.Willms     Updated revision history.
 *
 ******************************************************************************/

// =============================================================================
// CSCI/CSC/Program  : CH-53K DCU System Definition program
// Organization      : Mercury Systems Inc.
// File Name         : SysDef.h
// Description       : Header File for CH-53K DCU System Definition program
//
// Revision History
//
// Author              Date        Reason
// ------------------  ----------  ---------------------------------------------
// David B. Lim        2022-06-30  Initial
// Somebody            202x-xx-xx  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// =============================================================================

#ifndef _SYSDEF_H
#define _SYSDEF_H

/*******************************************************************************
 *                              # I n c l u d e s                              *
 ******************************************************************************/

/* none */

/*******************************************************************************
 *                      E x t e r n a l   # D e f i n e s                      *
 ******************************************************************************/

// Bit values
#define   BIT31       0x80000000
#define   BIT30       0x40000000
#define   BIT29       0x20000000
#define   BIT28       0x10000000
#define   BIT27       0x8000000
#define   BIT26       0x4000000
#define   BIT25       0x2000000
#define   BIT24       0x1000000
#define   BIT23       0x800000
#define   BIT22       0x400000
#define   BIT21       0x200000
#define   BIT20       0x100000
#define   BIT19       0x80000
#define   BIT18       0x40000
#define   BIT17       0x20000
#define   BIT16       0x10000
#define   BIT15       0x8000
#define   BIT14       0x4000
#define   BIT13       0x2000
#define   BIT12       0x1000
#define   BIT11       0x800
#define   BIT10       0x400
#define   BIT9        0x200
#define   BIT8        0x100
#define   BIT7        0x80
#define   BIT6        0x40
#define   BIT5        0x20
#define   BIT4        0x10
#define   BIT3        0x8
#define   BIT2        0x4
#define   BIT1        0x2
#define   BIT0        0x1

// ARINC 429 Bit values
#define   ARINC_BIT32       0x80000000
#define   ARINC_BIT31       0x40000000
#define   ARINC_BIT30       0x20000000
#define   ARINC_BIT29       0x10000000
#define   ARINC_BIT28       0x8000000
#define   ARINC_BIT27       0x4000000
#define   ARINC_BIT26       0x2000000
#define   ARINC_BIT25       0x1000000
#define   ARINC_BIT24       0x800000
#define   ARINC_BIT23       0x400000
#define   ARINC_BIT22       0x200000
#define   ARINC_BIT21       0x100000
#define   ARINC_BIT20       0x80000
#define   ARINC_BIT19       0x40000
#define   ARINC_BIT18       0x20000
#define   ARINC_BIT17       0x10000
#define   ARINC_BIT16       0x8000
#define   ARINC_BIT15       0x4000
#define   ARINC_BIT14       0x2000
#define   ARINC_BIT13       0x1000
#define   ARINC_BIT12       0x800
#define   ARINC_BIT11       0x400
#define   ARINC_BIT10       0x200
#define   ARINC_BIT9        0x100
#define   ARINC_BIT8        0x80
#define   ARINC_BIT7        0x40
#define   ARINC_BIT6        0x20
#define   ARINC_BIT5        0x10
#define   ARINC_BIT4        0x8
#define   ARINC_BIT3        0x4
#define   ARINC_BIT2        0x2
#define   ARINC_BIT1        0x1

// Number of bits
#define BITS_PER_BYTE         8
#define BITS_PER_WORD        16
#define BITS_PER_2WORDS      32
#define BYTES_PER_WORD        2

#define IN_HALF               2

#define NIBBLE_LEN            4

#define NIBBLE_MASK         0xF
#define ALL_8BITS           0xFF
#define ALL_16BITS          0xFFFF
#define ALL_32BITS          0xFFFFFFFF

#define   CLR_BIT(x, bitnum)  ((x) &= ~(1<<(bitnum)))
#define   SET_BIT(x, bitnum)  ((x) |= (1<<(bitnum)))

/*******************************************************************************
 *             E x t e r n a l   T y p e   D e c l a r a t i o n s             *
 ******************************************************************************/

// Integer types with no specified signedness
typedef char                            INT8;
typedef short                           INT16;
typedef int                            INT32;

// Signed integer types
typedef signed char                     SINT8;
typedef signed short                    SINT16;
typedef signed int                     SINT32;

// Unsigned integer types
typedef unsigned char                   UINT8;
typedef unsigned short                  UINT16;
typedef unsigned int                   UINT32;

// Constant integer types with no specified signedness
typedef const char                      CINT8;
typedef const short                     CINT16;
typedef const int                      CINT32;

// Constant signed integer types
typedef const signed char               CSINT8;
typedef const signed short              CSINT16;
typedef const signed int               CSINT32;

// Constant unsigned integer types
typedef const unsigned char             CUINT8;
typedef const unsigned short            CUINT16;
typedef const unsigned int             CUINT32;

// Volatile integer types with no specified signedness
typedef volatile char                   VINT8;
typedef volatile short                  VINT16;
typedef volatile int                   VINT32;

// Volatile signed integer types
typedef volatile signed char            VSINT8;
typedef volatile signed short           VSINT16;
typedef volatile signed int            VSINT32;

// Volatile unsigned integer types
typedef volatile unsigned char          VUINT8;
typedef volatile unsigned short         VUINT16;
typedef volatile unsigned int          VUINT32;

// Bitfield structure type for 8-bit data
typedef union
{
  struct
  {
    UINT8 Bit0          : 1;
    UINT8 Bit1          : 1;
    UINT8 Bit2          : 1;
    UINT8 Bit3          : 1;
    UINT8 Bit4          : 1;
    UINT8 Bit5          : 1;
    UINT8 Bit6          : 1;
    UINT8 Bit7          : 1;
  } Unpacked;

  UINT8 Packed;

} UINT8_UNION_TYPE;

// Bitfield structure type for 16-bit data
typedef union
{
  struct
  {
    UINT16 Bit0          : 1;
    UINT16 Bit1          : 1;
    UINT16 Bit2          : 1;
    UINT16 Bit3          : 1;
    UINT16 Bit4          : 1;
    UINT16 Bit5          : 1;
    UINT16 Bit6          : 1;
    UINT16 Bit7          : 1;
    UINT16 Bit8          : 1;
    UINT16 Bit9          : 1;
    UINT16 Bit10         : 1;
    UINT16 Bit11         : 1;
    UINT16 Bit12         : 1;
    UINT16 Bit13         : 1;
    UINT16 Bit14         : 1;
    UINT16 Bit15         : 1;
  } Unpacked;

  UINT16 Packed;

} UINT16_UNION_TYPE;

// Bitfield structure type for 32-bit data
typedef union
{
  struct
  {
    UINT32 Bit0          : 1;
    UINT32 Bit1          : 1;
    UINT32 Bit2          : 1;
    UINT32 Bit3          : 1;
    UINT32 Bit4          : 1;
    UINT32 Bit5          : 1;
    UINT32 Bit6          : 1;
    UINT32 Bit7          : 1;
    UINT32 Bit8          : 1;
    UINT32 Bit9          : 1;
    UINT32 Bit10         : 1;
    UINT32 Bit11         : 1;
    UINT32 Bit12         : 1;
    UINT32 Bit13         : 1;
    UINT32 Bit14         : 1;
    UINT32 Bit15         : 1;
    UINT32 Bit16         : 1;
    UINT32 Bit17         : 1;
    UINT32 Bit18         : 1;
    UINT32 Bit19         : 1;
    UINT32 Bit20         : 1;
    UINT32 Bit21         : 1;
    UINT32 Bit22         : 1;
    UINT32 Bit23         : 1;
    UINT32 Bit24         : 1;
    UINT32 Bit25         : 1;
    UINT32 Bit26         : 1;
    UINT32 Bit27         : 1;
    UINT32 Bit28         : 1;
    UINT32 Bit29         : 1;
    UINT32 Bit30         : 1;
    UINT32 Bit31         : 1;
  } Unpacked;

  UINT32 Packed;

} UINT32_UNION_TYPE;

// Enumerated values for test results
typedef enum
{
  PASS = 0,
  FAIL = 1
} RESULTS;

// Enumerated values for boolean entities
typedef enum
{
  FALSE = 0,
  TRUE  = 1
} BOOL;

// Enumerated values for the status of an operation
typedef enum
{
   SUCCESS = 0,
   FAILED  = 1
} STATUS_TYPE;

// Pointer type for accessing a function
typedef void (*FN_PTR_TYPE) (void);

/*******************************************************************************
 *             E x t e r n a l   D a t a   D e c l a r a t i o n s             *
 ******************************************************************************/

/* none */

/*******************************************************************************
 *           E x t e r n a l   F u n c t i o n   P r o t o t y p e s           *
 ******************************************************************************/

/* none */

#endif /* _SYSDEF_H */
