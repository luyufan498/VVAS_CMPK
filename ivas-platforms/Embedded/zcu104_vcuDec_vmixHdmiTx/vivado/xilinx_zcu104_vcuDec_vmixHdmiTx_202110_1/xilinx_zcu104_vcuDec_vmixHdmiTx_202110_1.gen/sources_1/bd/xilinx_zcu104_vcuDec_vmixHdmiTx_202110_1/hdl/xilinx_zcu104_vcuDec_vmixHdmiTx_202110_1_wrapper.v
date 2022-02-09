//Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2021.1.1 (lin64) Build 3286242 Wed Jul 28 13:09:46 MDT 2021
//Date        : Mon Nov 15 18:10:09 2021
//Host        : new-3090-server running 64-bit Ubuntu 18.04.5 LTS
//Command     : generate_target xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_wrapper.bd
//Design      : xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_wrapper
   (HDMI_CTRL_IIC_scl_io,
    HDMI_CTRL_IIC_sda_io,
    HDMI_TX_CLK_N,
    HDMI_TX_CLK_P,
    HDMI_TX_DAT_N,
    HDMI_TX_DAT_P,
    LED0,
    LED1,
    LED3,
    SI5319_LOL,
    SI5319_RST,
    TX_DDC_scl_io,
    TX_DDC_sda_io,
    TX_EN,
    TX_HPD,
    TX_REFCLK_N,
    TX_REFCLK_P);
  inout HDMI_CTRL_IIC_scl_io;
  inout HDMI_CTRL_IIC_sda_io;
  output HDMI_TX_CLK_N;
  output HDMI_TX_CLK_P;
  output [2:0]HDMI_TX_DAT_N;
  output [2:0]HDMI_TX_DAT_P;
  output [0:0]LED0;
  output [0:0]LED1;
  output [0:0]LED3;
  input SI5319_LOL;
  output [0:0]SI5319_RST;
  inout TX_DDC_scl_io;
  inout TX_DDC_sda_io;
  output [0:0]TX_EN;
  input TX_HPD;
  input TX_REFCLK_N;
  input TX_REFCLK_P;

  wire HDMI_CTRL_IIC_scl_i;
  wire HDMI_CTRL_IIC_scl_io;
  wire HDMI_CTRL_IIC_scl_o;
  wire HDMI_CTRL_IIC_scl_t;
  wire HDMI_CTRL_IIC_sda_i;
  wire HDMI_CTRL_IIC_sda_io;
  wire HDMI_CTRL_IIC_sda_o;
  wire HDMI_CTRL_IIC_sda_t;
  wire HDMI_TX_CLK_N;
  wire HDMI_TX_CLK_P;
  wire [2:0]HDMI_TX_DAT_N;
  wire [2:0]HDMI_TX_DAT_P;
  wire [0:0]LED0;
  wire [0:0]LED1;
  wire [0:0]LED3;
  wire SI5319_LOL;
  wire [0:0]SI5319_RST;
  wire TX_DDC_scl_i;
  wire TX_DDC_scl_io;
  wire TX_DDC_scl_o;
  wire TX_DDC_scl_t;
  wire TX_DDC_sda_i;
  wire TX_DDC_sda_io;
  wire TX_DDC_sda_o;
  wire TX_DDC_sda_t;
  wire [0:0]TX_EN;
  wire TX_HPD;
  wire TX_REFCLK_N;
  wire TX_REFCLK_P;

  IOBUF HDMI_CTRL_IIC_scl_iobuf
       (.I(HDMI_CTRL_IIC_scl_o),
        .IO(HDMI_CTRL_IIC_scl_io),
        .O(HDMI_CTRL_IIC_scl_i),
        .T(HDMI_CTRL_IIC_scl_t));
  IOBUF HDMI_CTRL_IIC_sda_iobuf
       (.I(HDMI_CTRL_IIC_sda_o),
        .IO(HDMI_CTRL_IIC_sda_io),
        .O(HDMI_CTRL_IIC_sda_i),
        .T(HDMI_CTRL_IIC_sda_t));
  IOBUF TX_DDC_scl_iobuf
       (.I(TX_DDC_scl_o),
        .IO(TX_DDC_scl_io),
        .O(TX_DDC_scl_i),
        .T(TX_DDC_scl_t));
  IOBUF TX_DDC_sda_iobuf
       (.I(TX_DDC_sda_o),
        .IO(TX_DDC_sda_io),
        .O(TX_DDC_sda_i),
        .T(TX_DDC_sda_t));
  xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1 xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_i
       (.HDMI_CTRL_IIC_scl_i(HDMI_CTRL_IIC_scl_i),
        .HDMI_CTRL_IIC_scl_o(HDMI_CTRL_IIC_scl_o),
        .HDMI_CTRL_IIC_scl_t(HDMI_CTRL_IIC_scl_t),
        .HDMI_CTRL_IIC_sda_i(HDMI_CTRL_IIC_sda_i),
        .HDMI_CTRL_IIC_sda_o(HDMI_CTRL_IIC_sda_o),
        .HDMI_CTRL_IIC_sda_t(HDMI_CTRL_IIC_sda_t),
        .HDMI_TX_CLK_N(HDMI_TX_CLK_N),
        .HDMI_TX_CLK_P(HDMI_TX_CLK_P),
        .HDMI_TX_DAT_N(HDMI_TX_DAT_N),
        .HDMI_TX_DAT_P(HDMI_TX_DAT_P),
        .LED0(LED0),
        .LED1(LED1),
        .LED3(LED3),
        .SI5319_LOL(SI5319_LOL),
        .SI5319_RST(SI5319_RST),
        .TX_DDC_scl_i(TX_DDC_scl_i),
        .TX_DDC_scl_o(TX_DDC_scl_o),
        .TX_DDC_scl_t(TX_DDC_scl_t),
        .TX_DDC_sda_i(TX_DDC_sda_i),
        .TX_DDC_sda_o(TX_DDC_sda_o),
        .TX_DDC_sda_t(TX_DDC_sda_t),
        .TX_EN(TX_EN),
        .TX_HPD(TX_HPD),
        .TX_REFCLK_N(TX_REFCLK_N),
        .TX_REFCLK_P(TX_REFCLK_P));
endmodule
