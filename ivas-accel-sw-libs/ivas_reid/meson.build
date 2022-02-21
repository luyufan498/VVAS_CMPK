sources = [
  'src/ivas_reid.cpp'
]

sources +=['../cm_package/cmpk_segmentation.cpp',
'../cm_package/cmpk_json_utils.cpp',
'../cm_package/cmpk_ffc.cpp'
]






#CMPK
reid_dep = cc.find_library('libvitis_ai_library-reid', dirs : ['/usr/lib/'])
reidtrack_dep = cc.find_library('libvitis_ai_library-reidtracker', dirs : ['/usr/lib/'])


vartutil_dep = cc.find_library('vart-util', dirs : ['/usr/lib/'])
xnnpp_dep = cc.find_library('vitis_ai_library-xnnpp', dirs : ['/usr/lib/'])
vitisconfig_dep = cc.find_library('vitis_ai_library-model_config', dirs : ['/usr/lib/'])
dputask_dep = cc.find_library('vitis_ai_library-dpu_task', dirs : ['/usr/lib/'])
opencvcore_dep = cc.find_library('opencv_core')

#vitisinc_dir = include_directories('/proj/ipeng3/saurabhs/nobkup/2020_1_sysroot/sysroots/aarch64-xilinx-linux/usr/include/vitis')

ivas_xdpuinfer = library('ivas_reid',
  sources,
  cpp_args : [gst_plugins_ivas_args, '-std=c++17'],
  include_directories : [configinc,reidtrackinc],
  dependencies : [glog_dep,gstvideo_dep, gst_dep, xrt_dep, jansson_dep, ivasutils_dep, gstivasinfermeta_dep, ivasinputmeta_dep, opencv_dep, opencvcore_dep, vartutil_dep, xnnpp_dep, vitisconfig_dep, dputask_dep,reid_dep,reidtrack_dep],
  install : true,
)

