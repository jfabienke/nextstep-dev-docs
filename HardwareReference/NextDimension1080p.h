/* NextDimension1080p.h 
 * Refined Driver Kit driver for 1080p output on NextDimension hardware.
 */

#import "reconstructed_mach_kernel.h"
#import "NextDimension_Registers.h"

/* We subclass IODirectDevice as it's the base for all hardware-controlling drivers */
@interface NextDimension1080p : IOFrameBufferDisplay
{
    int             slotNumber;
    
    /* Virtual pointers to hardware spaces */
    volatile unsigned int *vtcBase; /* Mapped slot space (registers) */
    volatile void         *vramBase; /* Mapped board space (pixels) */
    
    vm_size_t       vramSize;
}

/* Override required by Driver Kit */
+ (BOOL)probe:(IODeviceDescription *)deviceDescription;
- (BOOL)initFromDeviceDescription:(IODeviceDescription *)deviceDescription;

/* Display methods */
- (void)enterLinearMode;
- (void)revertToVGAMode;

/* Timing configuration */
- (void)setVideoTiming1080p;

@end
