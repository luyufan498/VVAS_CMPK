#include "cmpk_ffc.hpp"
// C语音风格读写IO
// C语音风格 FIFO
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// C语言风格处理字符串
#include <cstring>

#include<iostream>
#include <string>


using namespace std;

namespace cmpk{

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




int ivas_fifocommuncation_send_raw(fifocom ffc, const char *data
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

int ivas_fifocommuncation_send(char *data,int len,IvasFFCReportType sendtype)
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

// 旧版本读取数据 非阻塞
int ivas_fifocommuncation_read_raw(fifocom ffc,char *data,int len)
{
    int readFd = 0;
    // 读取时候不需要创建文件，文件由控制线程来创建
    readFd= open(ffc.rxpath.c_str (),O_RDONLY | O_NONBLOCK,0);
    
    if(readFd < 0)
    {
        return -1;
    }
    int readlen = read(readFd, data, len);  
    if(readlen>0){
        data[readlen] = '\0';
        // cout<<data<<endl;
    }
            
    
    

    //确保清空缓冲区
    char buff[64];
    while (read(readFd, buff, 64)>0){}   
   
    close(readFd);
    return readlen;
}



IvasFFCControlType ivas_fifocommuncation_read_ctr(fifocom ffc){

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

//*******************************************************************************//
//*******************************************************************************//
//*******************************************************************************//
//**************************新版本 / 旧版本 分割线*********************************//
//*******************************************************************************//
//*******************************************************************************//
//*******************************************************************************//



/**
 * @description: 检查文件是否存在 
 * @param {*}
 * @return {*}
 */
inline bool
fileexists (const string & name)
{
  struct stat buffer;
  return (stat (name.c_str (), &buffer) == 0);
}






// 读行数
int fifoComRead2lines(fifocom *ffc,std::vector<std::string> &strs){
    char buff[FIFO_MAX_LEN];
    //清空字符串
    strs.clear();
    // 不考虑溢出的情况
    int readlen = ivas_fifocommuncation_read_raw(*ffc,buff,FIFO_MAX_LEN);
    if(readlen <= 0)
    {
        return 0;
    }
    // ','来进行分割字符串
    char *delim = ",";
    char *p;
    // 第一次执行需要添加BUff
    p = strtok(buff, delim);
    strs.push_back(std::string(p));

    // 后续使用NULL作为参数
    while((p = strtok(NULL, delim)))
    {
        strs.push_back(std::string(p));
    }    
    return strs.size();
}

// 读数据 并且分割好后 放在 缓冲区
int fifoComRead(fifocom *ffc)
{
    fifoComRead2lines(ffc, ffc->lines_buffer);
    return 0;
}




int fifoComReadNB_splitbuffers(fifocom *ffc){
    fifoComRead2lines(ffc, ffc->lines_buffer);
    return 0;
}

// 旧版本更新得到
// 非阻塞输出
int fifoComWriteNB_string(fifocom *ffc, std::string str){
    return ivas_fifocommuncation_send_raw(*ffc,str.c_str(),str.size());    
}

int fifoComWriteNB_string(fifocom *ffc, const char* str, int len){
    return ivas_fifocommuncation_send_raw(*ffc,str,len);
}


int fifoComWriteNB_string(fifocom *ffc, const char* str){
    return ivas_fifocommuncation_send_raw(*ffc,str,strlen(str));
}



int fifoComReportNB_fps(fifocom *ffc,float fps,const char *info){
    // char *header = "reportFPS";
    char buff[255] = {0};
    sprintf(buff,"reportFPS,%f,%s\n",fps,info);
    return fifoComWriteNB_string(ffc,buff);
}


int fifoComReportNB_fps(fifocom *ffc,float fps){
    return fifoComReportNB_fps(ffc,fps,"");
}




}