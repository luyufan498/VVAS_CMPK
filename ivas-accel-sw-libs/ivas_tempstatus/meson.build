
sources =['../cm_package/cmpk_segmentation.cpp',
          '../cm_package/cmpk_json_utils.cpp',
          '../cm_package/cmpk_ffc.cpp',
          '../cm_package/cmpk_platformstats.cpp',
          'src/ivas_tempstatus.cpp',
          'src/common.hpp']

ivas_powerstatus = library('ivas_tempstatus',
  sources,
  cpp_args : [gst_plugins_ivas_args, '-pthread', '-std=c++17'],
  include_directories : [configinc],
  dependencies : [gstvideo_dep, gst_dep, xrt_dep, jansson_dep, ivasutils_dep, gstivasinfermeta_dep, opencv_dep, ivasinputmeta_dep, thread_dep],
  install : true,
)
