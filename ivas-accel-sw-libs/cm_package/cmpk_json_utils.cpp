#include "cmpk_json_utils.hpp"






int XkprivGetJsonData_int(json_t *jconfig, int *dataptr, char const* targetlabel, int default_data, int log_level)
{
    json_t *val;
    val = json_object_get (jconfig, targetlabel);
    if (!val || !json_is_integer (val))  {      
        *dataptr = default_data;
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "can not find '%s', use default val:%d",targetlabel,default_data);
        return false;
    }
    else
    {
        *dataptr =  json_integer_value (val);
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "read %s sucess, val: %d",targetlabel,*dataptr);
        return true;
    }
}

int XkprivGetJsonData_int(json_t *jconfig, uint *dataptr, char const* targetlabel, int default_data, int log_level){


    return XkprivGetJsonData_uint(jconfig,dataptr,targetlabel,default_data,log_level);
}


int XkprivGetJsonData_uint(json_t *jconfig, uint *dataptr, char const* targetlabel, uint default_data, int log_level)
{
    json_t *val;
    val = json_object_get (jconfig, targetlabel);
    if (!val || !json_is_integer (val))  {      
        *dataptr = default_data;
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "can not find '%s', use default val:%d",targetlabel,default_data);
        return false;
    }
    else
    {
        int realdata = json_integer_value (val);

        if(realdata > 0)
        {
            *dataptr = (uint)realdata;
        }
        else
        {
            *dataptr = 0;
        }
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "read %s sucess, val: %d",targetlabel,*dataptr);
        return true;
    }
}



int XkprivGetJsonData_float(json_t *jconfig, float *dataptr, char const* targetlabel, float default_data, int log_level)
{
    
    json_t *val;
    val = json_object_get (jconfig, targetlabel);
    if (!val || !json_is_number (val))  {      
        *dataptr = default_data;
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "can not find '%s', use default val:%d",targetlabel,default_data);
        return false;
    }
    else
    {
        *dataptr =  (float)json_number_value (val);
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "read %s sucess, val: %f",targetlabel,*dataptr);
        return true;
    }
}


int XkprivGetJsonData_bool(json_t *jconfig, bool *dataptr, char const* targetlabel, bool default_data, int log_level){
    json_t *val;
    val = json_object_get (jconfig, targetlabel);
    if (!val || !json_is_boolean (val)){                
        *dataptr = default_data;
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "can not find '%s', use default val:%d",targetlabel,default_data);
        return false;
    }
    else
    {
        *dataptr =  json_boolean_value (val);
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "read %s sucess, val: %d",targetlabel,*dataptr);
        return true;
    }
}

typedef int (*xkprivStringProcessAPI)(char *);  
int XkprivGetJsonData_string2Int(json_t *jconfig, int *dataptr, char const* targetlabel, xkprivStringProcessAPI api_string_to_num, int default_data, int log_level){
    json_t *val;
    val = json_object_get (jconfig, targetlabel);
    if (!json_is_string (val))
    {
        *dataptr = default_data;
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "can not find '%s', use default val:%d",targetlabel,default_data);
        return false;
    }

    *dataptr = api_string_to_num((char *) json_string_value (val));
    LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "read %s sucess, val: %d",targetlabel,*dataptr);
    return true;
}

int XkprivGetJsonData_string(json_t *jconfig, string *dataptr, char const* targetlabel, char const* default_data, int log_level){
    json_t *val;
    val = json_object_get (jconfig, targetlabel);
    if (!json_is_string (val))
    {
        *dataptr = default_data;
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "can not find '%s', use default val:%s",targetlabel,default_data);
        return false;
    }
    *dataptr =  (char *) json_string_value (val);
    LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "read %s sucess: %s", targetlabel, dataptr->c_str ());
    return true;

}



int XkprivGetJsonData_chars(json_t *jconfig, char *dataptr, char const* targetlabel, char const* default_data, int log_level){
    json_t *val;
    val = json_object_get (jconfig, targetlabel);
    if (!json_is_string (val))
    {
        strcpy (dataptr, default_data);
        LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "can not find '%s', use default val:%s",targetlabel,default_data);
        return false;
    }
    char* str = (char*)json_string_value (val);
    strcpy (dataptr, (char*)str);
    // *dataptr =  (char *) json_string_value (val);  
    LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "Read %s sucess: %s", targetlabel,dataptr);
    return true;

}




int JsonGet_rgb(json_t *jconfig, uint *red, uint *green, uint *blue, char const* targetlabel, uint default_rgb, int log_level){
    json_t *karray = NULL;
    karray = json_object_get (jconfig, targetlabel);
    if (!karray) {
      
      *blue = default_rgb & 0xff;
      *green = (default_rgb & 0xff00) >> 8;
      *red = (default_rgb & 0xff0000) >> 16;
      LOG_MESSAGE (LOG_LEVEL_DEBUG,log_level, "failed to find %s, use deflaut RGB:(%d,%d,%d)", targetlabel,*red,*green,*blue);
      return false;
    }
    else{      
        XkprivGetJsonData_uint(karray,blue,"blue",default_rgb & 0xff,0);
        XkprivGetJsonData_uint(karray,green,"green",(default_rgb & 0xff00) >> 8,0);
        XkprivGetJsonData_uint(karray,red,"red",(default_rgb & 0xff0000) >> 16,0);
        LOG_MESSAGE (LOG_LEVEL_DEBUG,log_level, "find %s, RGB:(%d,%d,%d)", targetlabel,*red,*green,*blue);
        return true;
    }
}


int JsonGetT(json_t *jconfig, int *dataptr, char const* targetlabel, int default_data, int log_level){
    return XkprivGetJsonData_int(jconfig,dataptr,targetlabel,default_data,log_level);
}

int JsonGetT(json_t *jconfig, uint *dataptr, char const* targetlabel, int default_data, int log_level){
    return XkprivGetJsonData_uint(jconfig,dataptr,targetlabel,default_data,log_level);
}

int JsonGetT(json_t *jconfig, float *dataptr, char const* targetlabel, float default_data, int log_level){
    return XkprivGetJsonData_float(jconfig,dataptr,targetlabel,default_data,log_level);
}

int JsonGetT(json_t *jconfig, bool *dataptr, char const* targetlabel, bool default_data, int log_level){

    return XkprivGetJsonData_bool(jconfig,dataptr,targetlabel,default_data,log_level);
}


int JsonGet_int(json_t *jconfig, int *dataptr, char const* targetlabel, int default_data, int log_level){
    return XkprivGetJsonData_int(jconfig,dataptr,targetlabel,default_data,log_level);
}


int JsonGet_int(json_t *jconfig, uint *dataptr, char const* targetlabel, int default_data, int log_level){
    return XkprivGetJsonData_uint(jconfig,dataptr,targetlabel,default_data,log_level);
}

int JsonGet_float(json_t *jconfig, float *dataptr, char const* targetlabel, float default_data, int log_level){
    return XkprivGetJsonData_float(jconfig,dataptr,targetlabel,default_data,log_level);
}

int JsonGet_bool(json_t *jconfig, bool *dataptr, char const* targetlabel, bool default_data, int log_level){

    return XkprivGetJsonData_bool(jconfig,dataptr,targetlabel,default_data,log_level);
}

int JsonGet_string(json_t *jconfig, string *dataptr, char const* targetlabel, char const* default_data, int log_level){

    return XkprivGetJsonData_string(jconfig, dataptr, targetlabel,default_data, log_level);
}
