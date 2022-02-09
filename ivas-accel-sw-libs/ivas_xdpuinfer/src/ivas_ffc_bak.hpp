#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string>
#define FIFO_WRITE "/tmp/ivasfifo_tomain"
#define FIFO_READ "/tmp/ivasfifo_frommain"
// #define MAX_LEN 128
#define FIFO_MAX_LEN 64


struct fifocommuncation_control{
  std::string txpath;
  std::string rxpath;
  int tx_frame_interval; // unit:frames
  int rx_frame_interval;



  bool report_fps;
};
typedef struct fifocommuncation_control ivas_ffc;


typedef enum
{
  IVAS_FFC_REPORT_FPS,
  IVAS_FFC_REPORT_PREDICTION,
  IVAS_FFC_REPORT_CUSTOM,
  IVAS_FFC_REPORT_UNKNOWN
}IvasFFCReportType;


typedef enum
{
  IVAS_FFC_CONTROL_HIGHER_PRF,
  IVAS_FFC_CONTROL_LOWER_PRE,
  IVAS_FFC_CONTROL_REPORT_MODEL,

  IVAS_FFC_CONTROL_UNKNOWN
}IvasFFCControlType;




//not use
int ivas_fifocommuncation_send(char *data,int len,IvasFFCReportType sendtype);
int ivas_fifocommuncation_send_raw(ivas_ffc ffc,char *data,int len);
int ivas_fifocommuncation_read_raw(ivas_ffc ffc,char *data,int len);
IvasFFCControlType ivas_fifocommuncation_read_ctr(ivas_ffc ffc);