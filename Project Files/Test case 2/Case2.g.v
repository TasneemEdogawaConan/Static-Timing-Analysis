module Case2(A,B,C,clk,Y);

input A;
input B;
input C;
input clk;
output Y;
wire wa;
wire wb;
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

DFFPOSX1 ff1(
	.CLK(clk),
	.D(w1),
	.Q(w3)
);

DFFPOSX1 ff2(
	.CLK(clk),
	.D(w2),
	.Q(w4)
);

XOR2X1 xor1(
	.A(w3),
	.B(w4),
	.Y(w5)
);

NAND2X1 nand1(
	.A(w3),
	.B(w4),
	.Y(w6)
);

DFFPOSX1 ff3(
	.CLK(clk),
	.D(w5),
	.Q(w9)
);

AND2X1 and2(
	.A(w5),
	.B(w6),
	.Y(w8)
);

MUX2X1 mux1(
	.A(w7),
	.B(w8),
	.S(w6),
	.Y(w9)
);

DFFPOSX1 ff4(
	.CLK(clk),
	.D(w9),
	.Q(w10)
);

assign wa = A;
assign wb = B;
assign wc = C;
assign Y = w11;

endmodule;
