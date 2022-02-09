proc bd_automation {parentCell} {
  

  #Get the GT_QUAD instance connected to DP_GT_CONTROLLER
  set gt_ip_inst [bd::gt_utils::find_connected_core [get_bd_intf_pins ${parentCell}/LINK_DATA0_OUT]]
  set GT_QUAD [bd::gt_utils::find_connected_core [get_bd_intf_pins ${gt_ip_inst}/gt_tx0]]
  #set lane_count [get_property CONFIG.LANE_COUNT $parentCell ]
  set lane_count 4
  set GT_DEBUG_PORT_EN [string tolower [get_property CONFIG.C_GT_DEBUG_PORT_EN ${gt_ip_inst} ]]
  set TX_PLL_SELECTION [get_property CONFIG.C_TX_PLL_SELECTION $gt_ip_inst ]

  if { $TX_PLL_SELECTION == 7 } {
    connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_lcpll0_lock] [get_bd_pins $GT_QUAD/hsclk0_lcplllock]
    connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_lcpll1_lock] [get_bd_pins $GT_QUAD/hsclk1_lcplllock]
    connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_lcpll0_reset] [get_bd_pins $GT_QUAD/hsclk0_lcpllreset]
    connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_lcpll1_reset] [get_bd_pins $GT_QUAD/hsclk1_lcpllreset]
   }

  if { $TX_PLL_SELECTION == 8 } {
    connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll0_lock] [get_bd_pins $GT_QUAD/hsclk0_rplllock]
    connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll1_lock] [get_bd_pins $GT_QUAD/hsclk1_rplllock]
    connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll0_reset] [get_bd_pins $GT_QUAD/hsclk0_rpllreset]
    connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll1_reset] [get_bd_pins $GT_QUAD/hsclk1_rpllreset]
   }
  connect_bd_intf_net [get_bd_intf_pins ${gt_ip_inst}/gt_debug] [get_bd_intf_pins $GT_QUAD/GT_DEBUG]
  if {$GT_DEBUG_PORT_EN == "true"} {
    connect_bd_intf_net [get_bd_intf_pins ${gt_ip_inst}/ch0_debug]  [get_bd_intf_pins $GT_QUAD/CH0_DEBUG]
    connect_bd_intf_net [get_bd_intf_pins ${gt_ip_inst}/ch1_debug]  [get_bd_intf_pins $GT_QUAD/CH1_DEBUG]
    connect_bd_intf_net [get_bd_intf_pins ${gt_ip_inst}/ch2_debug]  [get_bd_intf_pins $GT_QUAD/CH2_DEBUG]
  }
  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_ch0_ilo_resetdone] [get_bd_pins $GT_QUAD/ch0_iloresetdone]
  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_ch1_ilo_resetdone] [get_bd_pins $GT_QUAD/ch1_iloresetdone]
  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_ch2_ilo_resetdone] [get_bd_pins $GT_QUAD/ch2_iloresetdone]
  #bd::utils::enable_dbg
  #bd::utils::dbg "pin value ${pin_val}"  

  #set rplllock_pin_val [bd::gt_utils::find_connected_pin [get_bd_pins $GT_QUAD/hsclk0_rplllock]]
  #if {$rplllock_pin_val eq ""} {
  #  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll0_lock] [get_bd_pins $GT_QUAD/hsclk0_rplllock]
  #} else {
  #  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll1_lock] [get_bd_pins $GT_QUAD/hsclk1_rplllock]
  #} 
  #set rpllrst_pin_val [bd::gt_utils::find_connected_pin [get_bd_pins $GT_QUAD/hsclk0_rpllreset]]
  #if {$rpllrst_pin_val eq ""} {
  #  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll0_reset] [get_bd_pins $GT_QUAD/hsclk0_rpllreset]
  #} else {
  #  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll1_reset] [get_bd_pins $GT_QUAD/hsclk1_rpllreset]
  #} 
# # connect_bd_net [get_bd_pins ${gt_ip_inst}/gtpowergood] [get_bd_pins $GT_QUAD/gtpowergood]
  #if { $lane_count >= 2 } {
  #  #connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_txusrclk] [get_bd_pins $GT_QUAD/ch1_txusrclk]
  #} 
  #if { $lane_count == 4 } {
  #  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll1_lock] [get_bd_pins $GT_QUAD/hsclk1_rplllock]
  #  connect_bd_net [get_bd_pins ${gt_ip_inst}/gt_rpll1_reset] [get_bd_pins $GT_QUAD/hsclk1_rpllreset]
  #} 

  set bufg_gt_inst [bd::gt_utils::find_connected_core [get_bd_pins ${gt_ip_inst}/gt_txusrclk]]
  connect_bd_net [get_bd_pins ${gt_ip_inst}/tx_axi4s_aclk] [get_bd_pins ${parentCell}/link_clk] [get_bd_pins $bufg_gt_inst/usrclk]
  
} 
