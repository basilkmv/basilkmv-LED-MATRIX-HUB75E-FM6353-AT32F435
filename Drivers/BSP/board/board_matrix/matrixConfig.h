
#ifndef MATRIX_CONFIG_H
#define	MATRIX_CONFIG_H

#define BYTE_FOR_PIXEL          4

#define DISPLAY_WIDTH           128*2
#define DISPLAY_HEIGHT          64
#define DISPLAY_SCAN_LINES      DISPLAY_HEIGHT/2

#define MATRIX_BUFF_SIZE        DISPLAY_WIDTH * DISPLAY_SCAN_LINES * BYTE_FOR_PIXEL // the buffers need to hold 16 bit of image data per pixel over 6 outputs (RGB1 + RGB2)  
#define MATRIX_BUFF_LINE_SIZE   DISPLAY_WIDTH * BYTE_FOR_PIXEL
#define MATRIX_NUMBER_CHIPS     DISPLAY_WIDTH / 16

#define CLK_DELAY 2

//-------------------------------------------------------------------
#endif	/* DISPLAYCONFIG_H */

