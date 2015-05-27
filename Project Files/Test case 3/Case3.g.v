module Case3(A,B,C,clk,Y);

input A;
input B;
input C;
input D;
input clk;

output Y;

wire wa;
wire wb;
wire wc;
wire wd;
wire w1;
wire w2;
wire w2;
wire w4;
wire w5;
wire w6;
wire w7;
wire w8;
wire w9;
wire w10;

AND2X1 and1(
	.A(wa),
	.B(wb),
	.Y(w1)
);

AND2X1 and2(
	.A(wb),
	.B(wc),
	.Y(w2)
);

AND2X1 and3(
	.A(wc),
	.B(wd),
	.Y(w3)
);

INVX1 inverter1(
	.A(w1),
	.Y(w4)
);

NAND2X1 nand1(
	.A(w1),
	.B(w2),
	.Y(w5)
);

OR2X1 or1(
	.A(w2),
	.B(w3),
	.Y(w6)
);

NAND3X1 nand3in(
	.A(w4),
	.B(w5),
	.C(w6),
	.Y(w7)
);

DFFPOSX1 ff(
	.CLK(clk),
	.D(w7),
	.Q(w8)
);

INVX4 inverter2(
	.A(w8),
	.Y(w9)
);

AND2X1 and4(
	.A(w9),
	.B(w3),
	.Y(w10)
);

assign wa = A;
assign wb = B;
assign wc = C;
assign wd = D;
assign Y = w11;

endmodule;
