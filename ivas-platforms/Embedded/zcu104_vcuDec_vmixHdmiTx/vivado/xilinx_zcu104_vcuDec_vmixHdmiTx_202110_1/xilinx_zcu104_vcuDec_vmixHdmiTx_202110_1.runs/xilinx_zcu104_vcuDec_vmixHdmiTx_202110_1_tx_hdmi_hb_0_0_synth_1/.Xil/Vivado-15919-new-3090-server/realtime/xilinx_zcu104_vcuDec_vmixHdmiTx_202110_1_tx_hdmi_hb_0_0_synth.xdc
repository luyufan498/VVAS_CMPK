set_property SRC_FILE_INFO {cfile:/home/catmouse/VVAS/ivas-platforms/Embedded/zcu104_vcuDec_vmixHdmiTx/vivado/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1.gen/sources_1/bd/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1/ip/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_tx_hdmi_hb_0_0/constrs_1/new/hdmi_hb_ooc.xdc rfile:../../../../../xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1.gen/sources_1/bd/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1/ip/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_tx_hdmi_hb_0_0/constrs_1/new/hdmi_hb_ooc.xdc id:1 order:EARLY scoped_inst:inst} [current_design]
set_property SRC_FILE_INFO {cfile:/home/catmouse/VVAS/ivas-platforms/Embedded/zcu104_vcuDec_vmixHdmiTx/vivado/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1.gen/sources_1/bd/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1/ip/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_tx_hdmi_hb_0_0/constrs_1/new/hdmi_hb.xdc rfile:../../../../../xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1.gen/sources_1/bd/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1/ip/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_tx_hdmi_hb_0_0/constrs_1/new/hdmi_hb.xdc id:2 order:EARLY scoped_inst:inst} [current_design]
set_property SRC_FILE_INFO {cfile:/home/catmouse/VVAS/ivas-platforms/Embedded/zcu104_vcuDec_vmixHdmiTx/vivado/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1.runs/xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_tx_hdmi_hb_0_0_synth_1/dont_touch.xdc rfile:../../../dont_touch.xdc id:3} [current_design]
set_property src_info {type:SCOPED_XDC file:1 line:16 export:INPUT save:INPUT read:FILTER_OUT_OF_CONTEXT} [current_design]
create_clock -period 10.000 -name status_sb_aclk [get_ports status_sb_aclk]
set_property src_info {type:SCOPED_XDC file:1 line:19 export:INPUT save:INPUT read:FILTER_OUT_OF_CONTEXT} [current_design]
create_clock -period 3.333 -name link_clk [get_ports link_clk]
set_property src_info {type:SCOPED_XDC file:2 line:16 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -to [get_pins [list {inst/hdmi_hb_cnt_reg[0]/CLR} \
          {inst/hdmi_hb_cnt_reg[10]/CLR} \
          {inst/hdmi_hb_cnt_reg[11]/CLR} \
          {inst/hdmi_hb_cnt_reg[12]/CLR} \
          {inst/hdmi_hb_cnt_reg[13]/CLR} \
          {inst/hdmi_hb_cnt_reg[14]/CLR} \
          {inst/hdmi_hb_cnt_reg[15]/CLR} \
          {inst/hdmi_hb_cnt_reg[16]/CLR} \
          {inst/hdmi_hb_cnt_reg[17]/CLR} \
          {inst/hdmi_hb_cnt_reg[18]/CLR} \
          {inst/hdmi_hb_cnt_reg[19]/CLR} \
          {inst/hdmi_hb_cnt_reg[1]/CLR} \
          {inst/hdmi_hb_cnt_reg[20]/CLR} \
          {inst/hdmi_hb_cnt_reg[21]/CLR} \
          {inst/hdmi_hb_cnt_reg[22]/CLR} \
          {inst/hdmi_hb_cnt_reg[23]/CLR} \
          {inst/hdmi_hb_cnt_reg[2]/CLR} \
          {inst/hdmi_hb_cnt_reg[3]/CLR} \
          {inst/hdmi_hb_cnt_reg[4]/CLR} \
          {inst/hdmi_hb_cnt_reg[5]/CLR} \
          {inst/hdmi_hb_cnt_reg[6]/CLR} \
          {inst/hdmi_hb_cnt_reg[7]/CLR} \
          {inst/hdmi_hb_cnt_reg[8]/CLR} \
          {inst/hdmi_hb_cnt_reg[9]/CLR}]]
set_property src_info {type:SCOPED_XDC file:2 line:17 export:INPUT save:INPUT read:READ} [current_design]
set_false_path -to [get_pins inst/hdmi_hb_i_reg/CLR]
set_property src_info {type:XDC file:3 line:9 export:INPUT save:INPUT read:READ} [current_design]
set_property KEEP_HIERARCHY SOFT [get_cells inst]
