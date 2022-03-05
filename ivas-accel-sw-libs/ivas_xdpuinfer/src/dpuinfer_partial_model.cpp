/*
 * @Author: CatMouse
 * @Date: 2022-01-17 22:02:00
 * @LastEditTime: 2022-01-18 12:07:11
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/ivas_xdpuinfer/src/dpuinfer_partial_model.cpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */


#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

#include <vitis/ai/bounded_queue.hpp>
#include <vitis/ai/env_config.hpp>

extern "C"
{
#include <ivas/ivas_kernel.h>
}
#include <gst/ivas/gstinferencemeta.h>
#include <gst/ivas/gstivasinpinfer.h>



#include "ivas_xdpumodels.hpp"


#include "dpuinfer_partial_model.hpp"

#ifdef ENABLE_CLASSIFICATION
#include "ivas_xclassification.hpp"
#endif
#ifdef ENABLE_YOLOV3
#include "ivas_xyolov3.hpp"
#endif
#ifdef ENABLE_FACEDETECT
#include "ivas_xfacedetect.hpp"
#endif
#ifdef ENABLE_REID
#include "ivas_xreid.hpp"
#endif
#ifdef ENABLE_SSD
#include "ivas_xssd.hpp"
#endif
#ifdef ENABLE_REFINEDET
#include "ivas_xrefinedet.hpp"
#endif
#ifdef ENABLE_TFSSD
#include "ivas_xtfssd.hpp"
#endif
#ifdef ENABLE_YOLOV2
#include "ivas_xyolov2.hpp"
#endif
#ifdef ENABLE_ROADLINE
#include "ivas_xroadline.hpp"
#endif
#ifdef ENABLE_SEGMENTATION
#include "ivas_xsegmentation.hpp"
#endif



using namespace cv;
using namespace std;


inline bool
fileexists (const string & name)
{
  struct stat buffer;
  return (stat (name.c_str (), &buffer) == 0);
}

std::string
modelexits (ivas_xkpriv * kpriv)
{
  auto elf_name =
      kpriv->modelpath + "/" + kpriv->modelname + "/" + kpriv->modelname +
      ".elf";
  auto xmodel_name =
      kpriv->modelpath + "/" + kpriv->modelname + "/" + kpriv->modelname +
      ".xmodel";
  auto prototxt_name =
      kpriv->modelpath + "/" + kpriv->modelname + "/" + kpriv->modelname +
      ".prototxt";

  if (!fileexists (prototxt_name)) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "%s not found",
        prototxt_name.c_str ());
    elf_name = "";
    return elf_name;
  }

  if (fileexists (xmodel_name))
    return xmodel_name;
  else if (fileexists (elf_name))
    return elf_name;
  else {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
        "xmodel or elf file not found");
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "%s", elf_name.c_str ());
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "%s", xmodel_name.c_str ());
    elf_name = "";
  }

  return elf_name;
}


labels *
readlabel (ivas_xkpriv * kpriv, char *json_file)
{
  json_t *root = NULL, *karray, *label, *value;
  json_error_t error;
  unsigned int num_labels;
  labels *labelptr;

  /* get root json object */
  root = json_load_file (json_file, JSON_DECODE_ANY, &error);
  if (!root) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
        "failed to load json file(%s) reason %s", json_file, error.text);
    return NULL;
  }

  value = json_object_get (root, "model-name");
  if (json_is_string (value)) {
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "label is for model %s",
        (char *) json_string_value (value));
  }


  value = json_object_get (root, "num-labels");
  if (!value || !json_is_integer (value)) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
        "num-labels not found in %s", json_file);
    return NULL;
  } else {
    num_labels = json_integer_value (value);
    labelptr = (labels *) calloc (num_labels, sizeof (labels));
    kpriv->max_labels = num_labels;
  }

  /* get kernels array */
  karray = json_object_get (root, "labels");
  if (!karray) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
        "failed to find key labels");
    goto error;
  }

  if (!json_is_array (karray)) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
        "labels key is not of array type");
    goto error;
  }


  if (num_labels != json_array_size (karray)) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
        "number of labels(%u) != karray size(%lu)\n", num_labels,
        json_array_size (karray));
    goto error;
  }

  for (unsigned int index = 0; index < num_labels; index++) {
    label = json_array_get (karray, index);
    if (!label) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
          "failed to get label object");
      goto error;
    }
    value = json_object_get (label, "label");
    if (!value || !json_is_integer (value)) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
          "label num found for array %d", index);
      goto error;
    }

    /*label is index of array */
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "label %d",
        (int) json_integer_value (value));
    labels *lptr = labelptr + (int) json_integer_value (value);
    lptr->label = (int) json_integer_value (value);

    value = json_object_get (label, "name");
    if (!json_is_string (value)) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
          "name is not found for array %d", index);
      goto error;
    } else {
      lptr->name = (char *) json_string_value (value);
      LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "name %s",
          lptr->name.c_str ());
    }
    value = json_object_get (label, "display_name");
    if (!json_is_string (value)) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
          "display name is not found for array %d", index);
      goto error;
    } else {
      lptr->display_name = (char *) json_string_value (value);
      LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "display_name %s",
          lptr->display_name.c_str ());
    }

  }
  return labelptr;
error:
  free (labelptr);
  return NULL;
}

void ivas_clean_currentmodel(ivas_xkpriv * kpriv)
{
  if (kpriv->model != NULL)
  {
    kpriv->model->close();
    delete kpriv->model;
    kpriv->model = NULL;
  }

  if (kpriv->labelptr != NULL)
    free (kpriv->labelptr);
}

int
ivas_xclass_to_num (char *name)
{
  int nameslen = 0;
  while (ivas_xmodelclass[nameslen] != NULL) {    
    if (!strcmp (ivas_xmodelclass[nameslen], name))
      return nameslen;
    nameslen++;
  }
  return IVAS_XCLASS_NOTFOUND;
}



IVASVideoFormat
ivas_fmt_to_xfmt (char *name)
{
  if (!strncmp (name, "RGB", 3))
    return IVAS_VFMT_RGB8;
  else if (!strncmp (name, "BGR", 3))
    return IVAS_VFMT_BGR8;
  else
    return IVAS_VMFT_UNKNOWN;
}


long long
get_time ()
{
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return ((long long) tv.tv_sec * 1000000 + tv.tv_usec) +
      42 * 60 * 60 * INT64_C (1000000);
}


int
ivas_updatecaps (ivas_xkpriv * kpriv, ivas_xdpumodel * model)
{
  
  IVASKernel *handle = kpriv->handle;
  kernelpads **pads;
  if (!handle || !handle->padinfo)
    return false;
  handle->padinfo->sinkpads[0]->kcaps[0]->range_height = 0;
  handle->padinfo->sinkpads[0]->kcaps[0]->lower_height = model->requiredheight ();
  handle->padinfo->sinkpads[0]->kcaps[0]->upper_height = 0;
  handle->padinfo->sinkpads[0]->kcaps[0]->lower_width =  model->requiredwidth ();
  handle->padinfo->sinkpads[0]->kcaps[0]->upper_width = 0;

  return true;
}


void
ivas_caps_print_MY (IVASKernel * handle)
{
  int i, j, k;
  kernelpads **pads;
  if (!handle || !handle->padinfo)
    return;

  pads = handle->padinfo->sinkpads;
  LOG_MESSAGE (LOG_LEVEL_DEBUG, 3,"total sinkpad %d",
      handle->padinfo->nu_sinkpad);
  if (pads) {
    for (i = 0; i < handle->padinfo->nu_sinkpad; i++) {
      LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "nu of caps for sinkpad[%d] = %d", i,
          pads[i]->nu_caps);
      LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "value are");
      LOG_MESSAGE (LOG_LEVEL_DEBUG,3,
          "range_height\tlower_height\tupper_height\trange_width\tlower_width\tupper_width\tfmt...");
      for (j = 0; j < pads[i]->nu_caps; j++) {

        LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t ",
            pads[i]->kcaps[j]->range_height, pads[i]->kcaps[j]->lower_height,
            pads[i]->kcaps[j]->upper_height, pads[i]->kcaps[j]->range_width,
            pads[i]->kcaps[j]->lower_width, pads[i]->kcaps[j]->upper_width);
        for (k = 0; k < pads[i]->kcaps[j]->num_fmt; k++)
          LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "%d ", pads[i]->kcaps[j]->fmt[k]);
      }
    }
  }

  pads = handle->padinfo->srcpads;
  LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "total srcpad %d", handle->padinfo->nu_srcpad);
  if (pads) {
    for (i = 0; i < handle->padinfo->nu_srcpad; i++) {
      LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "nu of caps for srcpad[%d] = %d", i,
          pads[i]->nu_caps);
      LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "value are");
      LOG_MESSAGE (LOG_LEVEL_DEBUG,3,
          "range_height\tlower_height\tupper_height\trange_width\tlower_width\tupper_width\tfmt...");
      for (j = 0; j < pads[i]->nu_caps; j++) {

        LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t ",
            pads[i]->kcaps[j]->range_height, pads[i]->kcaps[j]->lower_height,
            pads[i]->kcaps[j]->upper_height, pads[i]->kcaps[j]->range_width,
            pads[i]->kcaps[j]->lower_width, pads[i]->kcaps[j]->upper_width);
        for (k = 0; k < pads[i]->kcaps[j]->num_fmt; k++)
          LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "%d ", pads[i]->kcaps[j]->fmt[k]);
      }
    }
  }
}







int
ivas_xsetcaps (ivas_xkpriv * kpriv, ivas_xdpumodel * model)
{
  kernelcaps *new_caps;
  IVASKernel *handle = kpriv->handle;

  // ivas_caps_free(kpriv->handle);

  if(ivas_caps_get_num_sinkpads(handle)>0 &&  ivas_caps_get_num_caps(handle,0)>1){

    LOG_MESSAGE (LOG_LEVEL_DEBUG,3, "Update instead of new");
    int result = ivas_updatecaps(kpriv,model);
    ivas_caps_print_MY (handle);
    return result;
  }
  ivas_caps_set_pad_nature (handle, IVAS_PAD_RIGID);

  new_caps =
      ivas_caps_new (false, model->requiredheight (), 0, false,
      model->requiredwidth (), 0, kpriv->modelfmt, 0);
  if (!new_caps)
    return false;
  if (ivas_caps_add_to_sink (handle, new_caps, 0) == false) {
    ivas_caps_free (handle);
    return false;
  }
  new_caps =
      ivas_caps_new (true, 1, 1024, true, 1, 1920, IVAS_VFMT_BGR8,
      IVAS_VFMT_RGB8, 0);
  if (!new_caps)
    return false;
  if (ivas_caps_add_to_sink (handle, new_caps, 0) == false) {
    ivas_caps_free (handle);
    return false;
  }
  if (kpriv->log_level >= LOG_LEVEL_WARNING)
    ivas_caps_print_MY (handle);

  return true;
}





/**
 * ivas_xinitmodel() - Initialize the required models
 *
 * This function calls the constructor of the CLASS provided in the json file
 * and calls create () of the dpu library of respective model.
 * Along with that it check the return from constructor either
 * label file is needed or not.
 */
ivas_xdpumodel *
ivas_xinitmodel (ivas_xkpriv * kpriv, int modelclass)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  ivas_xdpumodel *model = NULL;
  kpriv->labelptr = NULL;
  kpriv->labelflags = IVAS_XLABEL_NOT_REQUIRED;

  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "Creating model %s",
      kpriv->modelname.c_str ());

  const auto labelfile =
      kpriv->modelpath + "/" + kpriv->modelname + "/" + "label.json";
  if (fileexists (labelfile)) {
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level,
        "Label file %s found\n", labelfile.c_str ());
    kpriv->labelptr = readlabel (kpriv, (char *) labelfile.c_str ());
  }

  switch (modelclass) {
#ifdef ENABLE_CLASSIFICATION
    case IVAS_XCLASS_CLASSIFICATION:
    {
      model =
          new ivas_xclassification (kpriv, kpriv->elfname,
          kpriv->need_preprocess);
      break;
    }
#endif
#ifdef ENABLE_YOLOV3
    case IVAS_XCLASS_YOLOV3:
    {
      model = new ivas_xyolov3 (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif
#ifdef ENABLE_FACEDETECT
    case IVAS_XCLASS_FACEDETECT:
    {
      model =
          new ivas_xfacedetect (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif
#ifdef ENABLE_REID
    case IVAS_XCLASS_REID:
    {
      model = new ivas_xreid (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif
#ifdef ENABLE_SSD
    case IVAS_XCLASS_SSD:
    {
      model = new ivas_xssd (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif
#ifdef ENABLE_REFINEDET
    case IVAS_XCLASS_REFINEDET:
    {
      model =
          new ivas_xrefinedet (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif
#ifdef ENABLE_TFSSD
    case IVAS_XCLASS_TFSSD:
    {
      // model = new ivas_xtfssd (kpriv, kpriv->elfname, kpriv->need_preprocess);
      model = new ivas_xroadline (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif
#ifdef ENABLE_YOLOV2
    case IVAS_XCLASS_YOLOV2:
    {
      model = new ivas_xyolov2 (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif

#ifdef ENABLE_ROADLINE
    case IVAS_XCLASS_ROADLINE:
    {
      model = new ivas_xroadline (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif


#ifdef ENABLE_SEGMENTATION
    case IVAS_XCLASS_SEGMENTATION:
    {
      model = new ivas_xsegmentation (kpriv, kpriv->elfname, kpriv->need_preprocess);
      break;
    }
#endif



    default:
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "Not supported model");
      free (kpriv);
      return NULL;
  }

  if ((kpriv->labelflags & IVAS_XLABEL_REQUIRED)
      && (kpriv->labelflags & IVAS_XLABEL_NOT_FOUND)) {
    kpriv->model->close ();
    delete kpriv->model;
    kpriv->model = NULL;
    kpriv->modelclass = IVAS_XCLASS_NOTFOUND;

    if (kpriv->labelptr != NULL)
      free (kpriv->labelptr);

    return NULL;
  }

  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "Init XModel END %p",model);
  
//   if(kpriv->xsetcaps_done == false){
//       ivas_xsetcaps (kpriv, model);
//       kpriv->xsetcaps_done = true;
   
//   }

  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "before reture %p",model);

  return model;
}





void cloneInferBuff(ivas_xkpriv * kpriv, GstInferenceMeta * infer_meta){
  if(!kpriv->buff_en) return;
  kpriv->prediction_buff = gst_inference_prediction_copy(infer_meta->prediction);
}



/**
 * ivas_xrunmodel() - Run respective model
 */
int
ivas_xrunmodel (ivas_xkpriv * kpriv, cv::Mat & image,
    GstInferenceMeta * infer_meta, IVASFrame * inframe)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  static int frame_cnt = 0;
  frame_cnt ++;

  // 检查是否需要跳过某些帧
  if(frame_cnt%kpriv->interval_frames!=0)
  {
    if(kpriv->buff_en)
      infer_meta->prediction = gst_inference_prediction_copy(kpriv->prediction_buff);
    return true;
  }
    
  
  auto start_time = get_time ();

  ivas_xdpumodel *model = (ivas_xdpumodel *) kpriv->model;


  if (model->run (kpriv, image, infer_meta) != true) {
    LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "Model run failed %s",
        kpriv->modelname.c_str ());
    return -1;
  }

  if(kpriv->buff_en)
      kpriv->prediction_buff = gst_inference_prediction_copy(infer_meta->prediction);



  auto end_time = get_time ();
  kpriv->pf.excution_time_dpu = end_time - start_time;
  LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "DPU excution time:%d us",kpriv->pf.excution_time_dpu);

  return true;
}

