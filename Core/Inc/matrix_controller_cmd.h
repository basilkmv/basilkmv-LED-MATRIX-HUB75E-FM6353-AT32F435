/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MATRIX_CMD_H
#define __MATRIX_CMD_H
#include "board_conf.h"

typedef enum{
  CMD_GET = 1,
  CMD_GET_STATE,
  CMD_GET_GEOMETRY,
  CMD_GET_WIDTH,
  CMD_GET_HIGHT,
  CMD_GET_BRIGHT,

  CMD_SET_GEOMETRY,
  CMD_SET_WIDTH,
  CMD_SET_HIGHT,
  CMD_SET_BRIGHT,
  CMD_ON,
  CMD_OFF,
  CMD_SOFT_RESET,
  
  CMD_DRAW_PIXEL,
  CMD_DRAW_LINE,
  CMD_DRAW_RECT,
  CMD_DRAW_IMAGE,
  CMD_DRAW_IMAGE_FORS,
} cmd_type;

typedef struct {
  uint16_t cmd;
  uint16_t arg1;
  uint16_t arg2;
  uint16_t arg3;
  uint16_t arg4;
  uint16_t arg5;
  uint16_t arg6;
  uint16_t arg7;
} matrix_cmd_type;

typedef struct {
  uint16_t cmd;
  uint16_t arg1;
  uint16_t arg2;
  uint16_t arg3;
  uint16_t arg4;
  uint16_t arg5;
  uint16_t arg6;
  uint16_t arg7;
} matrix_sate_type;

#define CMD_SIZE_HELFWORD   sizeof(matrix_cmd_type)/2 

#endif /* __MATRIX_CMD_H */

/*****************************END OF FILE****/
