sources =['../cm_package/cmpk_json_utils.cpp',
          'src/ivas_crop.cpp',
          'src/common.hpp']


ivas_crop = library('ivas_crop',
  sources,
  cpp_args : [gst_plugins_ivas_args, '-pthread', '-std=c++17'],
  include_directories : [configinc],
  dependencies : [gstvideo_dep, gst_dep, xrt_dep, jansson_dep, ivasutils_dep, gstivasinfermeta_dep, opencv_dep, ivasinputmeta_dep, thread_dep],
  install : true,
)
