#ifndef GLOBAL_H
#define GLOBAL_H

#include "mcu_api.h"
#include "protocol.h"
#include "Tuyav.h"

#include "wifi.h"
typedef struct
{
  unsigned char dp_id;   //dp number
  unsigned char dp_type; //dp type
} DOWNLOAD_CMD_S;

extern Tuyav tuyav;
extern const DOWNLOAD_CMD_S download_cmd[];

const int PIN_UNUSED = -1;



#endif
