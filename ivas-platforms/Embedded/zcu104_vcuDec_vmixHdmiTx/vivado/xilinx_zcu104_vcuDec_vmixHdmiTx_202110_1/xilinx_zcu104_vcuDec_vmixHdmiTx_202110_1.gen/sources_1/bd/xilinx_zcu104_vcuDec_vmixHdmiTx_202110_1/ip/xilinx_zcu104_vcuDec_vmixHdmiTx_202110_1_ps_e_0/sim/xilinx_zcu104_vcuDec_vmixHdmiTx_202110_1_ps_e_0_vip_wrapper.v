 








// (c) Copyright 1995-2017 Xilinx, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.

`timescale 1ns/1ps

module xilinx_zcu104_vcuDec_vmixHdmiTx_202110_1_ps_e_0 (
maxihpm0_fpd_aclk, 
maxigp0_awid, 
maxigp0_awaddr, 
maxigp0_awlen, 
maxigp0_awsize, 
maxigp0_awburst, 
maxigp0_awlock, 
maxigp0_awcache, 
maxigp0_awprot, 
maxigp0_awvalid, 
maxigp0_awuser, 
maxigp0_awready, 
maxigp0_wdata, 
maxigp0_wstrb, 
maxigp0_wlast, 
maxigp0_wvalid, 
maxigp0_wready, 
maxigp0_bid, 
maxigp0_bresp, 
maxigp0_bvalid, 
maxigp0_bready, 
maxigp0_arid, 
maxigp0_araddr, 
maxigp0_arlen, 
maxigp0_arsize, 
maxigp0_arburst, 
maxigp0_arlock, 
maxigp0_arcache, 
maxigp0_arprot, 
maxigp0_arvalid, 
maxigp0_aruser, 
maxigp0_arready, 
maxigp0_rid, 
maxigp0_rdata, 
maxigp0_rresp, 
maxigp0_rlast, 
maxigp0_rvalid, 
maxigp0_rready, 
maxigp0_awqos, 
maxigp0_arqos, 
maxihpm0_lpd_aclk, 
maxigp2_awid, 
maxigp2_awaddr, 
maxigp2_awlen, 
maxigp2_awsize, 
maxigp2_awburst, 
maxigp2_awlock, 
maxigp2_awcache, 
maxigp2_awprot, 
maxigp2_awvalid, 
maxigp2_awuser, 
maxigp2_awready, 
maxigp2_wdata, 
maxigp2_wstrb, 
maxigp2_wlast, 
maxigp2_wvalid, 
maxigp2_wready, 
maxigp2_bid, 
maxigp2_bresp, 
maxigp2_bvalid, 
maxigp2_bready, 
maxigp2_arid, 
maxigp2_araddr, 
maxigp2_arlen, 
maxigp2_arsize, 
maxigp2_arburst, 
maxigp2_arlock, 
maxigp2_arcache, 
maxigp2_arprot, 
maxigp2_arvalid, 
maxigp2_aruser, 
maxigp2_arready, 
maxigp2_rid, 
maxigp2_rdata, 
maxigp2_rresp, 
maxigp2_rlast, 
maxigp2_rvalid, 
maxigp2_rready, 
maxigp2_awqos, 
maxigp2_arqos, 
saxihpc0_fpd_aclk, 
saxigp0_aruser, 
saxigp0_awuser, 
saxigp0_awid, 
saxigp0_awaddr, 
saxigp0_awlen, 
saxigp0_awsize, 
saxigp0_awburst, 
saxigp0_awlock, 
saxigp0_awcache, 
saxigp0_awprot, 
saxigp0_awvalid, 
saxigp0_awready, 
saxigp0_wdata, 
saxigp0_wstrb, 
saxigp0_wlast, 
saxigp0_wvalid, 
saxigp0_wready, 
saxigp0_bid, 
saxigp0_bresp, 
saxigp0_bvalid, 
saxigp0_bready, 
saxigp0_arid, 
saxigp0_araddr, 
saxigp0_arlen, 
saxigp0_arsize, 
saxigp0_arburst, 
saxigp0_arlock, 
saxigp0_arcache, 
saxigp0_arprot, 
saxigp0_arvalid, 
saxigp0_arready, 
saxigp0_rid, 
saxigp0_rdata, 
saxigp0_rresp, 
saxigp0_rlast, 
saxigp0_rvalid, 
saxigp0_rready, 
saxigp0_awqos, 
saxigp0_arqos, 
saxihp1_fpd_aclk, 
saxigp3_aruser, 
saxigp3_awuser, 
saxigp3_awid, 
saxigp3_awaddr, 
saxigp3_awlen, 
saxigp3_awsize, 
saxigp3_awburst, 
saxigp3_awlock, 
saxigp3_awcache, 
saxigp3_awprot, 
saxigp3_awvalid, 
saxigp3_awready, 
saxigp3_wdata, 
saxigp3_wstrb, 
saxigp3_wlast, 
saxigp3_wvalid, 
saxigp3_wready, 
saxigp3_bid, 
saxigp3_bresp, 
saxigp3_bvalid, 
saxigp3_bready, 
saxigp3_arid, 
saxigp3_araddr, 
saxigp3_arlen, 
saxigp3_arsize, 
saxigp3_arburst, 
saxigp3_arlock, 
saxigp3_arcache, 
saxigp3_arprot, 
saxigp3_arvalid, 
saxigp3_arready, 
saxigp3_rid, 
saxigp3_rdata, 
saxigp3_rresp, 
saxigp3_rlast, 
saxigp3_rvalid, 
saxigp3_rready, 
saxigp3_awqos, 
saxigp3_arqos, 
saxi_lpd_aclk, 
saxigp6_aruser, 
saxigp6_awuser, 
saxigp6_awid, 
saxigp6_awaddr, 
saxigp6_awlen, 
saxigp6_awsize, 
saxigp6_awburst, 
saxigp6_awlock, 
saxigp6_awcache, 
saxigp6_awprot, 
saxigp6_awvalid, 
saxigp6_awready, 
saxigp6_wdata, 
saxigp6_wstrb, 
saxigp6_wlast, 
saxigp6_wvalid, 
saxigp6_wready, 
saxigp6_bid, 
saxigp6_bresp, 
saxigp6_bvalid, 
saxigp6_bready, 
saxigp6_arid, 
saxigp6_araddr, 
saxigp6_arlen, 
saxigp6_arsize, 
saxigp6_arburst, 
saxigp6_arlock, 
saxigp6_arcache, 
saxigp6_arprot, 
saxigp6_arvalid, 
saxigp6_arready, 
saxigp6_rid, 
saxigp6_rdata, 
saxigp6_rresp, 
saxigp6_rlast, 
saxigp6_rvalid, 
saxigp6_rready, 
saxigp6_awqos, 
saxigp6_arqos, 
emio_gpio_i, 
emio_gpio_o, 
emio_gpio_t, 
pl_ps_irq0, 
pl_ps_irq1, 
pl_resetn0, 
pl_clk0 
);
input maxihpm0_fpd_aclk;
output [15 : 0] maxigp0_awid;
output [39 : 0] maxigp0_awaddr;
output [7 : 0] maxigp0_awlen;
output [2 : 0] maxigp0_awsize;
output [1 : 0] maxigp0_awburst;
output maxigp0_awlock;
output [3 : 0] maxigp0_awcache;
output [2 : 0] maxigp0_awprot;
output maxigp0_awvalid;
output [15 : 0] maxigp0_awuser;
input maxigp0_awready;
output [127 : 0] maxigp0_wdata;
output [15 : 0] maxigp0_wstrb;
output maxigp0_wlast;
output maxigp0_wvalid;
input maxigp0_wready;
input [15 : 0] maxigp0_bid;
input [1 : 0] maxigp0_bresp;
input maxigp0_bvalid;
output maxigp0_bready;
output [15 : 0] maxigp0_arid;
output [39 : 0] maxigp0_araddr;
output [7 : 0] maxigp0_arlen;
output [2 : 0] maxigp0_arsize;
output [1 : 0] maxigp0_arburst;
output maxigp0_arlock;
output [3 : 0] maxigp0_arcache;
output [2 : 0] maxigp0_arprot;
output maxigp0_arvalid;
output [15 : 0] maxigp0_aruser;
input maxigp0_arready;
input [15 : 0] maxigp0_rid;
input [127 : 0] maxigp0_rdata;
input [1 : 0] maxigp0_rresp;
input maxigp0_rlast;
input maxigp0_rvalid;
output maxigp0_rready;
output [3 : 0] maxigp0_awqos;
output [3 : 0] maxigp0_arqos;
input maxihpm0_lpd_aclk;
output [15 : 0] maxigp2_awid;
output [39 : 0] maxigp2_awaddr;
output [7 : 0] maxigp2_awlen;
output [2 : 0] maxigp2_awsize;
output [1 : 0] maxigp2_awburst;
output maxigp2_awlock;
output [3 : 0] maxigp2_awcache;
output [2 : 0] maxigp2_awprot;
output maxigp2_awvalid;
output [15 : 0] maxigp2_awuser;
input maxigp2_awready;
output [31 : 0] maxigp2_wdata;
output [3 : 0] maxigp2_wstrb;
output maxigp2_wlast;
output maxigp2_wvalid;
input maxigp2_wready;
input [15 : 0] maxigp2_bid;
input [1 : 0] maxigp2_bresp;
input maxigp2_bvalid;
output maxigp2_bready;
output [15 : 0] maxigp2_arid;
output [39 : 0] maxigp2_araddr;
output [7 : 0] maxigp2_arlen;
output [2 : 0] maxigp2_arsize;
output [1 : 0] maxigp2_arburst;
output maxigp2_arlock;
output [3 : 0] maxigp2_arcache;
output [2 : 0] maxigp2_arprot;
output maxigp2_arvalid;
output [15 : 0] maxigp2_aruser;
input maxigp2_arready;
input [15 : 0] maxigp2_rid;
input [31 : 0] maxigp2_rdata;
input [1 : 0] maxigp2_rresp;
input maxigp2_rlast;
input maxigp2_rvalid;
output maxigp2_rready;
output [3 : 0] maxigp2_awqos;
output [3 : 0] maxigp2_arqos;
input saxihpc0_fpd_aclk;
input saxigp0_aruser;
input saxigp0_awuser;
input [5 : 0] saxigp0_awid;
input [48 : 0] saxigp0_awaddr;
input [7 : 0] saxigp0_awlen;
input [2 : 0] saxigp0_awsize;
input [1 : 0] saxigp0_awburst;
input saxigp0_awlock;
input [3 : 0] saxigp0_awcache;
input [2 : 0] saxigp0_awprot;
input saxigp0_awvalid;
output saxigp0_awready;
input [127 : 0] saxigp0_wdata;
input [15 : 0] saxigp0_wstrb;
input saxigp0_wlast;
input saxigp0_wvalid;
output saxigp0_wready;
output [5 : 0] saxigp0_bid;
output [1 : 0] saxigp0_bresp;
output saxigp0_bvalid;
input saxigp0_bready;
input [5 : 0] saxigp0_arid;
input [48 : 0] saxigp0_araddr;
input [7 : 0] saxigp0_arlen;
input [2 : 0] saxigp0_arsize;
input [1 : 0] saxigp0_arburst;
input saxigp0_arlock;
input [3 : 0] saxigp0_arcache;
input [2 : 0] saxigp0_arprot;
input saxigp0_arvalid;
output saxigp0_arready;
output [5 : 0] saxigp0_rid;
output [127 : 0] saxigp0_rdata;
output [1 : 0] saxigp0_rresp;
output saxigp0_rlast;
output saxigp0_rvalid;
input saxigp0_rready;
input [3 : 0] saxigp0_awqos;
input [3 : 0] saxigp0_arqos;
input saxihp1_fpd_aclk;
input saxigp3_aruser;
input saxigp3_awuser;
input [5 : 0] saxigp3_awid;
input [48 : 0] saxigp3_awaddr;
input [7 : 0] saxigp3_awlen;
input [2 : 0] saxigp3_awsize;
input [1 : 0] saxigp3_awburst;
input saxigp3_awlock;
input [3 : 0] saxigp3_awcache;
input [2 : 0] saxigp3_awprot;
input saxigp3_awvalid;
output saxigp3_awready;
input [127 : 0] saxigp3_wdata;
input [15 : 0] saxigp3_wstrb;
input saxigp3_wlast;
input saxigp3_wvalid;
output saxigp3_wready;
output [5 : 0] saxigp3_bid;
output [1 : 0] saxigp3_bresp;
output saxigp3_bvalid;
input saxigp3_bready;
input [5 : 0] saxigp3_arid;
input [48 : 0] saxigp3_araddr;
input [7 : 0] saxigp3_arlen;
input [2 : 0] saxigp3_arsize;
input [1 : 0] saxigp3_arburst;
input saxigp3_arlock;
input [3 : 0] saxigp3_arcache;
input [2 : 0] saxigp3_arprot;
input saxigp3_arvalid;
output saxigp3_arready;
output [5 : 0] saxigp3_rid;
output [127 : 0] saxigp3_rdata;
output [1 : 0] saxigp3_rresp;
output saxigp3_rlast;
output saxigp3_rvalid;
input saxigp3_rready;
input [3 : 0] saxigp3_awqos;
input [3 : 0] saxigp3_arqos;
input saxi_lpd_aclk;
input saxigp6_aruser;
input saxigp6_awuser;
input [5 : 0] saxigp6_awid;
input [48 : 0] saxigp6_awaddr;
input [7 : 0] saxigp6_awlen;
input [2 : 0] saxigp6_awsize;
input [1 : 0] saxigp6_awburst;
input saxigp6_awlock;
input [3 : 0] saxigp6_awcache;
input [2 : 0] saxigp6_awprot;
input saxigp6_awvalid;
output saxigp6_awready;
input [31 : 0] saxigp6_wdata;
input [3 : 0] saxigp6_wstrb;
input saxigp6_wlast;
input saxigp6_wvalid;
output saxigp6_wready;
output [5 : 0] saxigp6_bid;
output [1 : 0] saxigp6_bresp;
output saxigp6_bvalid;
input saxigp6_bready;
input [5 : 0] saxigp6_arid;
input [48 : 0] saxigp6_araddr;
input [7 : 0] saxigp6_arlen;
input [2 : 0] saxigp6_arsize;
input [1 : 0] saxigp6_arburst;
input saxigp6_arlock;
input [3 : 0] saxigp6_arcache;
input [2 : 0] saxigp6_arprot;
input saxigp6_arvalid;
output saxigp6_arready;
output [5 : 0] saxigp6_rid;
output [31 : 0] saxigp6_rdata;
output [1 : 0] saxigp6_rresp;
output saxigp6_rlast;
output saxigp6_rvalid;
input saxigp6_rready;
input [3 : 0] saxigp6_awqos;
input [3 : 0] saxigp6_arqos;
input [94 : 0] emio_gpio_i;
output [94 : 0] emio_gpio_o;
output [94 : 0] emio_gpio_t;
input [0 : 0] pl_ps_irq0;
input [7 : 0] pl_ps_irq1;
output pl_resetn0;
output pl_clk0;
wire pl_clk_t[3:0] ;

wire saxihpc0_fpd_rclk_temp;
wire saxihpc0_fpd_wclk_temp;
wire saxihpc1_fpd_rclk_temp;
wire saxihpc1_fpd_wclk_temp;
wire saxihp0_fpd_rclk_temp;
wire saxihp0_fpd_wclk_temp;
wire saxihp1_fpd_rclk_temp;
wire saxihp1_fpd_wclk_temp;
wire saxihp2_fpd_rclk_temp;
wire saxihp2_fpd_wclk_temp;
wire saxihp3_fpd_rclk_temp;
wire saxihp3_fpd_wclk_temp;
wire saxi_lpd_rclk_temp;
wire saxi_lpd_wclk_temp;


assign pl_clk0 = pl_clk_t[0] ;

 assign  pl_clk1 = 1'b0 ;

 assign  pl_clk2 = 1'b0 ;

 assign  pl_clk3 = 1'b0 ;

     assign saxihpc0_fpd_rclk_temp  =  saxihpc0_fpd_aclk ;
	assign saxihpc0_fpd_wclk_temp  =  saxihpc0_fpd_aclk ;
  
   
   
    assign saxihp1_fpd_rclk_temp  =  saxihp1_fpd_aclk ;
	assign saxihp1_fpd_wclk_temp  =  saxihp1_fpd_aclk ;
   
   
   
    assign saxi_lpd_rclk_temp  =  saxi_lpd_aclk ;
	assign saxi_lpd_wclk_temp  =  saxi_lpd_aclk ;


  


  zynq_ultra_ps_e_vip_v1_0_10 #(
    .C_USE_M_AXI_GP0(1),
    .C_USE_M_AXI_GP1(0),
    .C_USE_M_AXI_GP2(1),
    .C_USE_S_AXI_GP0(1),
    .C_USE_S_AXI_GP1(0),
    .C_USE_S_AXI_GP2(0),
    .C_USE_S_AXI_GP3(1),
    .C_USE_S_AXI_GP4(0),
    .C_USE_S_AXI_GP5(0),
    .C_USE_S_AXI_GP6(1),
    .C_USE_S_AXI_ACP(0),
    .C_USE_S_AXI_ACE(0),
    .C_M_AXI_GP0_DATA_WIDTH(128),
    .C_M_AXI_GP1_DATA_WIDTH(128),
    .C_M_AXI_GP2_DATA_WIDTH(32),
    .C_S_AXI_GP0_DATA_WIDTH(128),
    .C_S_AXI_GP1_DATA_WIDTH(128),
    .C_S_AXI_GP2_DATA_WIDTH(128),
    .C_S_AXI_GP3_DATA_WIDTH(128),
    .C_S_AXI_GP4_DATA_WIDTH(128),
    .C_S_AXI_GP5_DATA_WIDTH(128),
    .C_S_AXI_GP6_DATA_WIDTH(32),
    .C_FCLK_CLK0_FREQ(100.000000),
    .C_FCLK_CLK1_FREQ(100),
    .C_FCLK_CLK2_FREQ(100),
    .C_FCLK_CLK3_FREQ(100)
  ) inst (
    .MAXIGP0ARVALID(maxigp0_arvalid),
    .MAXIGP0AWVALID(maxigp0_awvalid),
    .MAXIGP0BREADY(maxigp0_bready),
    .MAXIGP0RREADY(maxigp0_rready),
    .MAXIGP0WLAST(maxigp0_wlast),
    .MAXIGP0WVALID(maxigp0_wvalid),
    .MAXIGP0ARID(maxigp0_arid),
    .MAXIGP0ARUSER(maxigp0_aruser),
    .MAXIGP0AWID(maxigp0_awid),
    .MAXIGP0ARBURST(maxigp0_arburst),
    .MAXIGP0ARLOCK(maxigp0_arlock),
    .MAXIGP0ARSIZE(maxigp0_arsize),
    .MAXIGP0AWBURST(maxigp0_awburst),
    .MAXIGP0AWLOCK(maxigp0_awlock),
    .MAXIGP0AWSIZE(maxigp0_awsize),
    .MAXIGP0ARPROT(maxigp0_arprot),
    .MAXIGP0AWPROT(maxigp0_awprot),
    .MAXIGP0ARADDR(maxigp0_araddr),
    .MAXIGP0AWADDR(maxigp0_awaddr),
    .MAXIGP0WDATA(maxigp0_wdata),
    .MAXIGP0AWUSER(maxigp0_awuser),
    .MAXIGP0ARCACHE(maxigp0_arcache),
    .MAXIGP0ARLEN(maxigp0_arlen),
    .MAXIGP0ARQOS(maxigp0_arqos),
    .MAXIGP0AWCACHE(maxigp0_awcache),
    .MAXIGP0AWLEN(maxigp0_awlen),
    .MAXIGP0AWQOS(maxigp0_awqos),
    .MAXIGP0WSTRB(maxigp0_wstrb),
    .MAXIGP0ACLK(maxihpm0_fpd_aclk),
    .MAXIGP0ARREADY(maxigp0_arready),
    .MAXIGP0AWREADY(maxigp0_awready),
    .MAXIGP0BVALID(maxigp0_bvalid),
    .MAXIGP0RLAST(maxigp0_rlast),
    .MAXIGP0RVALID(maxigp0_rvalid),
    .MAXIGP0WREADY(maxigp0_wready),
    .MAXIGP0BID(maxigp0_bid),
    .MAXIGP0RID(maxigp0_rid),
    .MAXIGP0BRESP(maxigp0_bresp),
    .MAXIGP0RRESP(maxigp0_rresp),
    .MAXIGP0RDATA(maxigp0_rdata),
    .MAXIGP1ARVALID(),
    .MAXIGP1AWVALID(),
    .MAXIGP1BREADY(),
    .MAXIGP1RREADY(),
    .MAXIGP1WLAST(),
    .MAXIGP1WVALID(),
    .MAXIGP1ARID(),
    .MAXIGP1ARUSER(),
    .MAXIGP1AWID(),
    .MAXIGP1ARBURST(),
    .MAXIGP1ARLOCK(),
    .MAXIGP1ARSIZE(),
    .MAXIGP1AWBURST(),
    .MAXIGP1AWLOCK(),
    .MAXIGP1AWSIZE(),
    .MAXIGP1ARPROT(),
    .MAXIGP1AWPROT(),
    .MAXIGP1ARADDR(),
    .MAXIGP1AWADDR(),
    .MAXIGP1WDATA(),
    .MAXIGP1AWUSER(),
    .MAXIGP1ARCACHE(),
    .MAXIGP1ARLEN(),
    .MAXIGP1ARQOS(),
    .MAXIGP1AWCACHE(),
    .MAXIGP1AWLEN(),
    .MAXIGP1AWQOS(),
    .MAXIGP1WSTRB(),
    .MAXIGP1ACLK(1'B0),
    .MAXIGP1ARREADY(1'B0),
    .MAXIGP1AWREADY(1'B0),
    .MAXIGP1BVALID(1'B0),
    .MAXIGP1RLAST(1'B0),
    .MAXIGP1RVALID(1'B0),
    .MAXIGP1WREADY(1'B0),
    .MAXIGP1BID(12'B0),
    .MAXIGP1RID(12'B0),
    .MAXIGP1BRESP(2'B0),
    .MAXIGP1RRESP(2'B0),
    .MAXIGP1RDATA(32'B0),

    .MAXIGP2ARVALID(maxigp2_arvalid),
    .MAXIGP2AWVALID(maxigp2_awvalid),
    .MAXIGP2BREADY(maxigp2_bready),
    .MAXIGP2RREADY(maxigp2_rready),
    .MAXIGP2WLAST(maxigp2_wlast),
    .MAXIGP2WVALID(maxigp2_wvalid),
    .MAXIGP2ARID(maxigp2_arid),
    .MAXIGP2ARUSER(maxigp2_aruser),
    .MAXIGP2AWID(maxigp2_awid),
    .MAXIGP2ARBURST(maxigp2_arburst),
    .MAXIGP2ARLOCK(maxigp2_arlock),
    .MAXIGP2ARSIZE(maxigp2_arsize),
    .MAXIGP2AWBURST(maxigp2_awburst),
    .MAXIGP2AWLOCK(maxigp2_awlock),
    .MAXIGP2AWSIZE(maxigp2_awsize),
    .MAXIGP2ARPROT(maxigp2_arprot),
    .MAXIGP2AWPROT(maxigp2_awprot),
    .MAXIGP2ARADDR(maxigp2_araddr),
    .MAXIGP2AWADDR(maxigp2_awaddr),
    .MAXIGP2WDATA(maxigp2_wdata),
    .MAXIGP2AWUSER(maxigp2_awuser),
    .MAXIGP2ARCACHE(maxigp2_arcache),
    .MAXIGP2ARLEN(maxigp2_arlen),
    .MAXIGP2ARQOS(maxigp2_arqos),
    .MAXIGP2AWCACHE(maxigp2_awcache),
    .MAXIGP2AWLEN(maxigp2_awlen),
    .MAXIGP2AWQOS(maxigp2_awqos),
    .MAXIGP2WSTRB(maxigp2_wstrb),
    .MAXIGP2ACLK(maxihpm0_lpd_aclk),
    .MAXIGP2ARREADY(maxigp2_arready),
    .MAXIGP2AWREADY(maxigp2_awready),
    .MAXIGP2BVALID(maxigp2_bvalid),
    .MAXIGP2RLAST(maxigp2_rlast),
    .MAXIGP2RVALID(maxigp2_rvalid),
    .MAXIGP2WREADY(maxigp2_wready),
    .MAXIGP2BID(maxigp2_bid),
    .MAXIGP2RID(maxigp2_rid),
    .MAXIGP2BRESP(maxigp2_bresp),
    .MAXIGP2RRESP(maxigp2_rresp),
    .MAXIGP2RDATA(maxigp2_rdata),

    .SAXIGP0RCLK(saxihpc0_fpd_rclk_temp),
    .SAXIGP0WCLK(saxihpc0_fpd_wclk_temp),
    .SAXIGP0ARUSER(saxigp0_aruser),
    .SAXIGP0AWUSER(saxigp0_awuser),
    .SAXIGP0RACOUNT(saxigp0_racount),
    .SAXIGP0WACOUNT(saxigp0_wacount),
    .SAXIGP0RCOUNT(saxigp0_rcount),
    .SAXIGP0WCOUNT(saxigp0_wcount),
    .SAXIGP0ARREADY(saxigp0_arready),
    .SAXIGP0AWREADY(saxigp0_awready),
    .SAXIGP0BVALID(saxigp0_bvalid),
    .SAXIGP0RLAST(saxigp0_rlast),
    .SAXIGP0RVALID(saxigp0_rvalid),
    .SAXIGP0WREADY(saxigp0_wready),
    .SAXIGP0BRESP(saxigp0_bresp),
    .SAXIGP0RRESP(saxigp0_rresp),
    .SAXIGP0RDATA(saxigp0_rdata),
    .SAXIGP0BID(saxigp0_bid),
    .SAXIGP0RID(saxigp0_rid),
    .SAXIGP0ARVALID(saxigp0_arvalid),
    .SAXIGP0AWVALID(saxigp0_awvalid),
    .SAXIGP0BREADY(saxigp0_bready),
    .SAXIGP0RREADY(saxigp0_rready),
    .SAXIGP0WLAST(saxigp0_wlast),
    .SAXIGP0WVALID(saxigp0_wvalid),
    .SAXIGP0ARBURST(saxigp0_arburst),
    .SAXIGP0ARLOCK(saxigp0_arlock),
    .SAXIGP0ARSIZE(saxigp0_arsize),
    .SAXIGP0AWBURST(saxigp0_awburst),
    .SAXIGP0AWLOCK(saxigp0_awlock),
    .SAXIGP0AWSIZE(saxigp0_awsize),
    .SAXIGP0ARPROT(saxigp0_arprot),
    .SAXIGP0AWPROT(saxigp0_awprot),
    .SAXIGP0ARADDR(saxigp0_araddr),
    .SAXIGP0AWADDR(saxigp0_awaddr),
    .SAXIGP0WDATA(saxigp0_wdata),
    .SAXIGP0ARCACHE(saxigp0_arcache),
    .SAXIGP0ARLEN(saxigp0_arlen),
    .SAXIGP0ARQOS(saxigp0_arqos),
    .SAXIGP0AWCACHE(saxigp0_awcache),
    .SAXIGP0AWLEN(saxigp0_awlen),
    .SAXIGP0AWQOS(saxigp0_awqos),
    .SAXIGP0WSTRB(saxigp0_wstrb),
    .SAXIGP0ARID(saxigp0_arid),
    .SAXIGP0AWID(saxigp0_awid),
    .SAXIGP1RCLK(),
    .SAXIGP1WCLK(),
    .SAXIGP1ARUSER(),
    .SAXIGP1AWUSER(),
    .SAXIGP1RACOUNT(),
    .SAXIGP1WACOUNT(),
    .SAXIGP1RCOUNT(),
    .SAXIGP1WCOUNT(),
    .SAXIGP1ARREADY(),
    .SAXIGP1AWREADY(),
    .SAXIGP1BVALID(),
    .SAXIGP1RLAST(),
    .SAXIGP1RVALID(),
    .SAXIGP1WREADY(),
    .SAXIGP1BRESP(),
    .SAXIGP1RRESP(),
    .SAXIGP1RDATA(),
    .SAXIGP1BID(),
    .SAXIGP1RID(),
    .SAXIGP1ARVALID(1'B0),
    .SAXIGP1AWVALID(1'B0),
    .SAXIGP1BREADY(1'B0),
    .SAXIGP1RREADY(1'B0),
    .SAXIGP1WLAST(1'B0),
    .SAXIGP1WVALID(1'B0),
    .SAXIGP1ARBURST(2'B0),
    .SAXIGP1ARLOCK(2'B0),
    .SAXIGP1ARSIZE(3'B0),
    .SAXIGP1AWBURST(2'B0),
    .SAXIGP1AWLOCK(2'B0),
    .SAXIGP1AWSIZE(3'B0),
    .SAXIGP1ARPROT(3'B0),
    .SAXIGP1AWPROT(3'B0),
    .SAXIGP1ARADDR(32'B0),
    .SAXIGP1AWADDR(32'B0),
    .SAXIGP1WDATA(32'B0),
    .SAXIGP1ARCACHE(4'B0),
    .SAXIGP1ARLEN(4'B0),
    .SAXIGP1ARQOS(4'B0),
    .SAXIGP1AWCACHE(4'B0),
    .SAXIGP1AWLEN(4'B0),
    .SAXIGP1AWQOS(4'B0),
    .SAXIGP1WSTRB(4'B0),
    .SAXIGP1ARID(6'B0),
    .SAXIGP1AWID(6'B0),
    .SAXIGP2RCLK(),
    .SAXIGP2WCLK(),
    .SAXIGP2ARUSER(),
    .SAXIGP2AWUSER(),
    .SAXIGP2RACOUNT(),
    .SAXIGP2WACOUNT(),
    .SAXIGP2RCOUNT(),
    .SAXIGP2WCOUNT(),
    .SAXIGP2ARREADY(),
    .SAXIGP2AWREADY(),
    .SAXIGP2BVALID(),
    .SAXIGP2RLAST(),
    .SAXIGP2RVALID(),
    .SAXIGP2WREADY(),
    .SAXIGP2BRESP(),
    .SAXIGP2RRESP(),
    .SAXIGP2RDATA(),
    .SAXIGP2BID(),
    .SAXIGP2RID(),
    .SAXIGP2ARVALID(1'B0),
    .SAXIGP2AWVALID(1'B0),
    .SAXIGP2BREADY(1'B0),
    .SAXIGP2RREADY(1'B0),
    .SAXIGP2WLAST(1'B0),
    .SAXIGP2WVALID(1'B0),
    .SAXIGP2ARBURST(2'B0),
    .SAXIGP2ARLOCK(2'B0),
    .SAXIGP2ARSIZE(3'B0),
    .SAXIGP2AWBURST(2'B0),
    .SAXIGP2AWLOCK(2'B0),
    .SAXIGP2AWSIZE(3'B0),
    .SAXIGP2ARPROT(3'B0),
    .SAXIGP2AWPROT(3'B0),
    .SAXIGP2ARADDR(32'B0),
    .SAXIGP2AWADDR(32'B0),
    .SAXIGP2WDATA(32'B0),
    .SAXIGP2ARCACHE(4'B0),
    .SAXIGP2ARLEN(4'B0),
    .SAXIGP2ARQOS(4'B0),
    .SAXIGP2AWCACHE(4'B0),
    .SAXIGP2AWLEN(4'B0),
    .SAXIGP2AWQOS(4'B0),
    .SAXIGP2WSTRB(4'B0),
    .SAXIGP2ARID(6'B0),
    .SAXIGP2AWID(6'B0),
    .SAXIGP3RCLK(saxihp1_fpd_rclk_temp),
    .SAXIGP3WCLK(saxihp1_fpd_wclk_temp),
    .SAXIGP3ARUSER(saxigp3_aruser),
    .SAXIGP3AWUSER(saxigp3_awuser),
    .SAXIGP3RACOUNT(saxigp3_racount),
    .SAXIGP3WACOUNT(saxigp3_wacount),
    .SAXIGP3RCOUNT(saxigp3_rcount),
    .SAXIGP3WCOUNT(saxigp3_wcount),
    .SAXIGP3ARREADY(saxigp3_arready),
    .SAXIGP3AWREADY(saxigp3_awready),
    .SAXIGP3BVALID(saxigp3_bvalid),
    .SAXIGP3RLAST(saxigp3_rlast),
    .SAXIGP3RVALID(saxigp3_rvalid),
    .SAXIGP3WREADY(saxigp3_wready),
    .SAXIGP3BRESP(saxigp3_bresp),
    .SAXIGP3RRESP(saxigp3_rresp),
    .SAXIGP3RDATA(saxigp3_rdata),
    .SAXIGP3BID(saxigp3_bid),
    .SAXIGP3RID(saxigp3_rid),
    .SAXIGP3ARVALID(saxigp3_arvalid),
    .SAXIGP3AWVALID(saxigp3_awvalid),
    .SAXIGP3BREADY(saxigp3_bready),
    .SAXIGP3RREADY(saxigp3_rready),
    .SAXIGP3WLAST(saxigp3_wlast),
    .SAXIGP3WVALID(saxigp3_wvalid),
    .SAXIGP3ARBURST(saxigp3_arburst),
    .SAXIGP3ARLOCK(saxigp3_arlock),
    .SAXIGP3ARSIZE(saxigp3_arsize),
    .SAXIGP3AWBURST(saxigp3_awburst),
    .SAXIGP3AWLOCK(saxigp3_awlock),
    .SAXIGP3AWSIZE(saxigp3_awsize),
    .SAXIGP3ARPROT(saxigp3_arprot),
    .SAXIGP3AWPROT(saxigp3_awprot),
    .SAXIGP3ARADDR(saxigp3_araddr),
    .SAXIGP3AWADDR(saxigp3_awaddr),
    .SAXIGP3WDATA(saxigp3_wdata),
    .SAXIGP3ARCACHE(saxigp3_arcache),
    .SAXIGP3ARLEN(saxigp3_arlen),
    .SAXIGP3ARQOS(saxigp3_arqos),
    .SAXIGP3AWCACHE(saxigp3_awcache),
    .SAXIGP3AWLEN(saxigp3_awlen),
    .SAXIGP3AWQOS(saxigp3_awqos),
    .SAXIGP3WSTRB(saxigp3_wstrb),
    .SAXIGP3ARID(saxigp3_arid),
    .SAXIGP3AWID(saxigp3_awid),
    .SAXIGP4RCLK(),
    .SAXIGP4WCLK(),
    .SAXIGP4ARUSER(),
    .SAXIGP4AWUSER(),
    .SAXIGP4RACOUNT(),
    .SAXIGP4WACOUNT(),
    .SAXIGP4RCOUNT(),
    .SAXIGP4WCOUNT(),
    .SAXIGP4ARREADY(),
    .SAXIGP4AWREADY(),
    .SAXIGP4BVALID(),
    .SAXIGP4RLAST(),
    .SAXIGP4RVALID(),
    .SAXIGP4WREADY(),
    .SAXIGP4BRESP(),
    .SAXIGP4RRESP(),
    .SAXIGP4RDATA(),
    .SAXIGP4BID(),
    .SAXIGP4RID(),
    .SAXIGP4ARVALID(1'B0),
    .SAXIGP4AWVALID(1'B0),
    .SAXIGP4BREADY(1'B0),
    .SAXIGP4RREADY(1'B0),
    .SAXIGP4WLAST(1'B0),
    .SAXIGP4WVALID(1'B0),
    .SAXIGP4ARBURST(2'B0),
    .SAXIGP4ARLOCK(2'B0),
    .SAXIGP4ARSIZE(3'B0),
    .SAXIGP4AWBURST(2'B0),
    .SAXIGP4AWLOCK(2'B0),
    .SAXIGP4AWSIZE(3'B0),
    .SAXIGP4ARPROT(3'B0),
    .SAXIGP4AWPROT(3'B0),
    .SAXIGP4ARADDR(32'B0),
    .SAXIGP4AWADDR(32'B0),
    .SAXIGP4WDATA(32'B0),
    .SAXIGP4ARCACHE(4'B0),
    .SAXIGP4ARLEN(4'B0),
    .SAXIGP4ARQOS(4'B0),
    .SAXIGP4AWCACHE(4'B0),
    .SAXIGP4AWLEN(4'B0),
    .SAXIGP4AWQOS(4'B0),
    .SAXIGP4WSTRB(4'B0),
    .SAXIGP4ARID(6'B0),
    .SAXIGP4AWID(6'B0),
    .SAXIGP5RCLK(),
    .SAXIGP5WCLK(),
    .SAXIGP5ARUSER(),
    .SAXIGP5AWUSER(),
    .SAXIGP5RACOUNT(),
    .SAXIGP5WACOUNT(),
    .SAXIGP5RCOUNT(),
    .SAXIGP5WCOUNT(),
    .SAXIGP5ARREADY(),
    .SAXIGP5AWREADY(),
    .SAXIGP5BVALID(),
    .SAXIGP5RLAST(),
    .SAXIGP5RVALID(),
    .SAXIGP5WREADY(),
    .SAXIGP5BRESP(),
    .SAXIGP5RRESP(),
    .SAXIGP5RDATA(),
    .SAXIGP5BID(),
    .SAXIGP5RID(),
    .SAXIGP5ARVALID(1'B0),
    .SAXIGP5AWVALID(1'B0),
    .SAXIGP5BREADY(1'B0),
    .SAXIGP5RREADY(1'B0),
    .SAXIGP5WLAST(1'B0),
    .SAXIGP5WVALID(1'B0),
    .SAXIGP5ARBURST(2'B0),
    .SAXIGP5ARLOCK(2'B0),
    .SAXIGP5ARSIZE(3'B0),
    .SAXIGP5AWBURST(2'B0),
    .SAXIGP5AWLOCK(2'B0),
    .SAXIGP5AWSIZE(3'B0),
    .SAXIGP5ARPROT(3'B0),
    .SAXIGP5AWPROT(3'B0),
    .SAXIGP5ARADDR(32'B0),
    .SAXIGP5AWADDR(32'B0),
    .SAXIGP5WDATA(32'B0),
    .SAXIGP5ARCACHE(4'B0),
    .SAXIGP5ARLEN(4'B0),
    .SAXIGP5ARQOS(4'B0),
    .SAXIGP5AWCACHE(4'B0),
    .SAXIGP5AWLEN(4'B0),
    .SAXIGP5AWQOS(4'B0),
    .SAXIGP5WSTRB(4'B0),
    .SAXIGP5ARID(6'B0),
    .SAXIGP5AWID(6'B0),
    .SAXIGP6RCLK(saxi_lpd_rclk_temp),
    .SAXIGP6WCLK(saxi_lpd_wclk_temp),
    .SAXIGP6ARUSER(saxigp6_aruser),
    .SAXIGP6AWUSER(saxigp6_awuser),
    .SAXIGP6RACOUNT(saxigp6_racount),
    .SAXIGP6WACOUNT(saxigp6_wacount),
    .SAXIGP6RCOUNT(saxigp6_rcount),
    .SAXIGP6WCOUNT(saxigp6_wcount),
    .SAXIGP6ARREADY(saxigp6_arready),
    .SAXIGP6AWREADY(saxigp6_awready),
    .SAXIGP6BVALID(saxigp6_bvalid),
    .SAXIGP6RLAST(saxigp6_rlast),
    .SAXIGP6RVALID(saxigp6_rvalid),
    .SAXIGP6WREADY(saxigp6_wready),
    .SAXIGP6BRESP(saxigp6_bresp),
    .SAXIGP6RRESP(saxigp6_rresp),
    .SAXIGP6RDATA(saxigp6_rdata),
    .SAXIGP6BID(saxigp6_bid),
    .SAXIGP6RID(saxigp6_rid),
    .SAXIGP6ARVALID(saxigp6_arvalid),
    .SAXIGP6AWVALID(saxigp6_awvalid),
    .SAXIGP6BREADY(saxigp6_bready),
    .SAXIGP6RREADY(saxigp6_rready),
    .SAXIGP6WLAST(saxigp6_wlast),
    .SAXIGP6WVALID(saxigp6_wvalid),
    .SAXIGP6ARBURST(saxigp6_arburst),
    .SAXIGP6ARLOCK(saxigp6_arlock),
    .SAXIGP6ARSIZE(saxigp6_arsize),
    .SAXIGP6AWBURST(saxigp6_awburst),
    .SAXIGP6AWLOCK(saxigp6_awlock),
    .SAXIGP6AWSIZE(saxigp6_awsize),
    .SAXIGP6ARPROT(saxigp6_arprot),
    .SAXIGP6AWPROT(saxigp6_awprot),
    .SAXIGP6ARADDR(saxigp6_araddr),
    .SAXIGP6AWADDR(saxigp6_awaddr),
    .SAXIGP6WDATA(saxigp6_wdata),
    .SAXIGP6ARCACHE(saxigp6_arcache),
    .SAXIGP6ARLEN(saxigp6_arlen),
    .SAXIGP6ARQOS(saxigp6_arqos),
    .SAXIGP6AWCACHE(saxigp6_awcache),
    .SAXIGP6AWLEN(saxigp6_awlen),
    .SAXIGP6AWQOS(saxigp6_awqos),
    .SAXIGP6WSTRB(saxigp6_wstrb),
    .SAXIGP6ARID(saxigp6_arid),
    .SAXIGP6AWID(saxigp6_awid),
    .SAXIACPARREADY(),
    .SAXIACPAWREADY(),
    .SAXIACPBVALID(),
    .SAXIACPRLAST(),
    .SAXIACPRVALID(),
    .SAXIACPWREADY(),
    .SAXIACPBRESP(),
    .SAXIACPRRESP(),
    .SAXIACPBID(),
    .SAXIACPRID(),
    .SAXIACPRDATA(),
    .SAXIACPACLK(1'B0),
    .SAXIACPARVALID(1'B0),
    .SAXIACPAWVALID(1'B0),
    .SAXIACPBREADY(1'B0),
    .SAXIACPRREADY(1'B0),
    .SAXIACPWLAST(1'B0),
    .SAXIACPWVALID(1'B0),
    .SAXIACPARID(3'B0),
    .SAXIACPARPROT(3'B0),
    .SAXIACPAWID(3'B0),
    .SAXIACPAWPROT(3'B0),
    .SAXIACPARADDR(32'B0),
    .SAXIACPAWADDR(32'B0),
    .SAXIACPARCACHE(4'B0),
    .SAXIACPARLEN(4'B0),
    .SAXIACPARQOS(4'B0),
    .SAXIACPAWCACHE(4'B0),
    .SAXIACPAWLEN(4'B0),
    .SAXIACPAWQOS(4'B0),
    .SAXIACPARBURST(2'B0),
    .SAXIACPARLOCK(2'B0),
    .SAXIACPARSIZE(3'B0),
    .SAXIACPAWBURST(2'B0),
    .SAXIACPAWLOCK(2'B0),
    .SAXIACPAWSIZE(3'B0),
    .SAXIACPARUSER(5'B0),
    .SAXIACPAWUSER(5'B0),
    .SAXIACPWDATA(64'B0),
    .SAXIACPWSTRB(8'B0),
.SACEFPDACREADY(),       
.SACEFPDARLOCK(),
.SACEFPDARVALID(),
.SACEFPDAWLOCK(),
.SACEFPDAWVALID(),
.SACEFPDBREADY(),
.SACEFPDCDLAST(),
.SACEFPDCDVALID(),
.SACEFPDCRVALID(),
.SACEFPDRACK(),
.SACEFPDRREADY(),
.SACEFPDWACK(),
.SACEFPDWLAST(),
.SACEFPDWUSER(),
.SACEFPDWVALID(),
.SACEFPDCDDATA(),
.SACEFPDWDATA(),
.SACEFPDARUSER(),
.SACEFPDAWUSER(),
.SACEFPDWSTRB(),
.SACEFPDARBAR(),
.SACEFPDARBURST(),
.SACEFPDARDOMAIN(),
.SACEFPDAWBAR(),
.SACEFPDAWBURST(),
.SACEFPDAWDOMAIN(),
.SACEFPDARPROT(),
.SACEFPDARSIZE(),
.SACEFPDAWPROT(),
.SACEFPDAWSIZE(),
.SACEFPDAWSNOOP(),
.SACEFPDARCACHE(),
.SACEFPDARQOS(),
.SACEFPDARREGION(),
.SACEFPDARSNOOP(),
.SACEFPDAWCACHE(),
.SACEFPDAWQOS(),
.SACEFPDAWREGION(),
.SACEFPDARADDR(),
.SACEFPDAWADDR(),
.SACEFPDCRRESP(),
.SACEFPDARID(),
.SACEFPDAWID(),
.SACEFPDARLEN(),
.SACEFPDAWLEN(),
.SACEFPDACVALID(),
.SACEFPDARREADY(),
.SACEFPDAWREADY(),
.SACEFPDBUSER(),
.SACEFPDBVALID(),
.SACEFPDCDREADY(),
.SACEFPDCRREADY(),
.SACEFPDRLAST(),
.SACEFPDRUSER(),
.SACEFPDRVALID(),
.SACEFPDWREADY(),
.SACEFPDRDATA(),
.SACEFPDBRESP(),
.SACEFPDACPROT(),
.SACEFPDACSNOOP(),
.SACEFPDRRESP(),
.SACEFPDACADDR(),
.SACEFPDBID(),
.SACEFPDRID(),

.PLPSIRQ0(pl_ps_irq0),
.PLPSIRQ1(pl_ps_irq1), 

.PL_RESETN0(pl_resetn0),
.PLCLK({pl_clk_t[3],pl_clk_t[2],pl_clk_t[1],pl_clk_t[0]})
  );

endmodule
