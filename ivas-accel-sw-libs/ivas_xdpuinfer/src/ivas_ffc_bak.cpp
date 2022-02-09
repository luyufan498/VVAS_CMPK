#include "ivas_ffc.hpp"



int report_unparse(char *dest
                    ,char *src
                    ,int len
                    ,IvasFFCReportType sendtype){
    if(len > FIFO_MAX_LEN - 2 or len == 0){
        return -1;
    }
    //convert to ASCLL code
    dest[0] = sendtype + 48;
    dest[1] = len + 48;
    strncpy(&dest[2],src,len);
    return len + 2;
}




int ivas_fifocommuncation_send_raw(ivas_ffc ffc
                                ,char *data
                                ,int len
                                )
{
    int writeFd = 0;
    
    if(mkfifo(ffc.txpath.c_str (),S_IRWXU|S_IRWXG|S_IRWXO) < 0 && (errno != EEXIST))
    {
        //  perror("make fifo failed:");
         return -1;
    }
    /*关闭管道的读描述符*/
    // use none block
    writeFd = open(ffc.txpath.c_str (),O_WRONLY | O_NONBLOCK,0);
    /*向管道写入数据*/
    // FIFO ISSUE
    if(writeFd < 0)
        return -2;
    write(writeFd,data,len);
    close(writeFd);
    return 0;
}

int ivas_fifocommuncation_send(char *data
                                ,int len
                                ,IvasFFCReportType sendtype)
{

    int writeFd = 0;
    char txbuffer[FIFO_MAX_LEN] = {0};
    
    int txlen = report_unparse(txbuffer,data,len,sendtype);

    if(txlen < 0)
    {
        return -1;
    }
    
    if(mkfifo(FIFO_WRITE,S_IRUSR|S_IWUSR) < 0 && (errno != EEXIST))
    {
        //  perror("make fifo failed:");
         return -1;
    }
    /*关闭管道的读描述符*/
    // use none block
    writeFd = open(FIFO_WRITE,O_WRONLY | O_NONBLOCK,0);
    /*向管道写入数据*/
    // FIFO ISSUE
    if(writeFd < 0)
        return -2;
    write(writeFd,txbuffer,txlen);
    close(writeFd);
    return 0;
}

int ivas_fifocommuncation_read_raw(ivas_ffc ffc
                                ,char *data
                                ,int len
                                )
{
    int readFd = 0;
    
    // if(mkfifo(ffc.rxpath.c_str (),S_IRWXU|S_IRWXG|S_IRWXO) < 0 && (errno != EEXIST))
    // {
    //     //  perror("make fifo failed:");
    //      return -10;
    // }

    readFd= open(ffc.rxpath.c_str (),O_RDONLY | O_NONBLOCK,0);
    
    if(readFd < 0)
    {
        return -20;
    }

    // char buffer[FIFO_MAX_LEN] ={0};

    int readlen = read(readFd, data, len);
    
    //log
    // if(readlen > 0){
    //     printf("%d:%s",readlen,data);
    //     fflush(stdout);
    // }

    close(readFd);
    return readlen;
}




IvasFFCControlType ivas_fifocommuncation_read_ctr(ivas_ffc ffc){

    static const char *ivas_ffc_commands[IVAS_FFC_CONTROL_UNKNOWN + 1] = {
        [IVAS_FFC_CONTROL_HIGHER_PRF] =         "PRF_UP",
        [IVAS_FFC_CONTROL_LOWER_PRE] =          "PRF_DOWN",
        [IVAS_FFC_CONTROL_REPORT_MODEL] =       "RPT_MODEL",
        /* Add ivas_ffc_commands */
        [IVAS_FFC_CONTROL_UNKNOWN] =            "UNKNOWN CMD"
        };


    char buff[FIFO_MAX_LEN] = {0};
    int readlen =  ivas_fifocommuncation_read_raw(ffc,buff,FIFO_MAX_LEN);
    
    if(readlen < 0)
    {
        return IVAS_FFC_CONTROL_UNKNOWN;
    }

    for(IvasFFCControlType i=IVAS_FFC_CONTROL_HIGHER_PRF;
            i < IVAS_FFC_CONTROL_UNKNOWN; 
            i = (IvasFFCControlType)(i+1))
    {
        if(!strcmp (buff, ivas_ffc_commands[i])){
            
            // printf("FIND CMD:%s\n",ivas_ffc_commands[i]);
            // fflush(stdout);
            return i;
        }
            
    }

    // printf("%d:%s\n",readlen,buff);
    // printf("%s\n",ivas_ffc_commands[IVAS_FFC_CONTROL_UNKNOWN]);
    // fflush(stdout);

    return IVAS_FFC_CONTROL_UNKNOWN;
}



