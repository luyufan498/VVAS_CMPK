
set_false_path -from [get_cells CTRL_s_axi_U/int_width_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_height_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_video_format_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_background_Y_R_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_background_U_G_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_background_V_B_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_layerEnable_reg[*]]

set_false_path -from [get_cells CTRL_s_axi_U/int_layerAlpha_?_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_layerStartX_?_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_layerStartY_?_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_layerWidth_?_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_layerHeight_?_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_layerStride_?_reg[*]]
set_false_path -from [get_cells CTRL_s_axi_U/int_layerScaleFactor_?_reg[*]]


