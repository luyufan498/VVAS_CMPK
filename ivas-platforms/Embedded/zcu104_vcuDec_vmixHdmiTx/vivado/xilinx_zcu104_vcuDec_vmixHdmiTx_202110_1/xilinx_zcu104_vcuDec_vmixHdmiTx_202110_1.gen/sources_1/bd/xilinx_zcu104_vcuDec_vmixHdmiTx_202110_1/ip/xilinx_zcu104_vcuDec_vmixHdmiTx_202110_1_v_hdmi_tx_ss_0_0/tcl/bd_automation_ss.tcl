proc bd_automation {parentCell} {
  
  set gt_ip_name hdmitx_gt_ip0  
  set pll_type_rx [get_property CONFIG.C_EXDES_RX_PLL_SELECTION $parentCell ]
  set pll_type_tx [get_property CONFIG.C_EXDES_TX_PLL_SELECTION $parentCell ]
  #set lane_count [get_property CONFIG.LANE_COUNT $parentCell ]
  set lane_count 3
#  set link_rate  [get_property CONFIG.LINK_RATE $parentCell ]
  set ppc  [get_property CONFIG.C_INPUT_PIXELS_PER_CLOCK $parentCell ]
  set gt_ip_inst [bd::gt_utils::create_unique_name $gt_ip_name]

  create_bd_cell -vlnv xilinx.com:ip:hdmi_gt_controller $gt_ip_inst
  #set_property -dict [list CONFIG.c_protocol {0} CONFIG.c_enable_tx {1} CONFIG.c_tx_num_ch $lane_count CONFIG.c_rx_num_ch {1} CONFIG.c_tx_rate $link_rate CONFIG.c_rx_rate {1.62}] [get_bd_cells ${gt_ip_inst}]
  set_property -dict [list CONFIG.C_Rx_Protocol {None} CONFIG.C_GT_DIRECTION {SIMPLEX_TX} CONFIG.C_INPUT_PIXELS_PER_CLOCK $ppc CONFIG.C_TX_PLL_SELECTION $pll_type_tx CONFIG.C_RX_PLL_SELECTION $pll_type_rx CONFIG.C_Tx_No_Of_Channels {4} CONFIG.C_Use_GT_CH4_HDMI {true} ] [get_bd_cells ${gt_ip_inst}]

  connect_bd_intf_net [get_bd_intf_pins ${parentCell}/SB_STATUS_IN] [get_bd_intf_pins ${gt_ip_inst}/status_sb_tx]
  connect_bd_intf_net [get_bd_intf_pins ${parentCell}/LINK_DATA0_OUT] [get_bd_intf_pins ${gt_ip_inst}/tx_axi4s_ch0]
  connect_bd_intf_net [get_bd_intf_pins ${parentCell}/LINK_DATA1_OUT] [get_bd_intf_pins ${gt_ip_inst}/tx_axi4s_ch1]
  connect_bd_intf_net [get_bd_intf_pins ${parentCell}/LINK_DATA2_OUT] [get_bd_intf_pins ${gt_ip_inst}/tx_axi4s_ch2]

#  connect_bd_net [get_bd_pins ${parentCell}/link_clk] [get_bd_pins ${gt_ip_inst}/tx_axi4s_aclk]
# [get_bd_pins ${gt_ip_inst}/gt_txusrclk]
  connect_bd_net [get_bd_pins ${parentCell}/video_clk] [get_bd_pins ${gt_ip_inst}/tx_video_clk]
  connect_bd_net [get_bd_pins ${parentCell}/s_axi_cpu_aclk] [get_bd_pins ${gt_ip_inst}/sb_aclk] [get_bd_pins ${gt_ip_inst}/apb_clk] [get_bd_pins ${gt_ip_inst}/axi4lite_aclk]
  connect_bd_net [get_bd_pins ${parentCell}/s_axi_cpu_aresetn] [get_bd_pins ${gt_ip_inst}/sb_aresetn] [get_bd_pins ${gt_ip_inst}/axi4lite_aresetn]
#  connect_bd_net [get_bd_pins ${parentCell}/tx_gt_ctrl_out] [get_bd_pins ${gt_ip_inst}/dp_gt_ctrl]

}

