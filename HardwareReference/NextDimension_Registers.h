/* NextDimension_Registers.h 
 * Register offsets for the NextDimension Video Timing Controller (VTC).
 * Based on NeXTbus slot space mappings.
 */

#ifndef ND_REGISTERS_H
#define ND_REGISTERS_H

/* NeXTbus Slot Space Offsets (0xfs000000) */
#define ND_VTC_OFFSET           0x00000000 /* Assume VTC is at slot base */

/* VTC Register Offsets */
#define ND_PIXEL_PLL_CTRL_REG   0x40 
#define ND_PIXEL_PLL_STATUS_REG 0x44
#define ND_H_TOTAL_REG          0x48
#define ND_H_DISPLAY_REG        0x4C
#define ND_H_SYNC_START_REG     0x58
#define ND_H_SYNC_END_REG       0x5C
#define ND_V_TOTAL_REG          0x50
#define ND_V_DISPLAY_REG        0x54
#define ND_V_SYNC_START_REG     0x64
#define ND_V_SYNC_END_REG       0x68
#define ND_LINE_STRIDE_REG      0x6C
#define ND_DISPLAY_CTRL_REG     0x60

/* Bitmasks */
#define PLL_LOCK                0x00000001
#define MODE_UPDATE             0x80000000

/* NeXTbus Physical Bases (from Mach/07_NeXTbus.md) */
#define NEXTBUS_SLOT_BASE(s)    (0xf0000000 | ((s) << 24))
#define NEXTBUS_BOARD_BASE(s)   ((s) << 28)

#endif /* ND_REGISTERS_H */
