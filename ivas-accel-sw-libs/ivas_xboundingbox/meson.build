

sources =['../cm_package/cmpk_segmentation.cpp']


ivas_xboundingbox = library('ivas_xboundingbox',
  'src/ivas_xboundingbox.cpp',
  sources,
  cpp_args : [gst_plugins_ivas_args, '-std=c++11'],
  include_directories : [configinc],
  dependencies : [gstvideo_dep,gst_dep, jansson_dep, ivasutils_dep, gstivasinfermeta_dep,ivasinputmeta_dep, opencv_dep],
  install : true,
)
