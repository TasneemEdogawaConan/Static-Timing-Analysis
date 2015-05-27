module Case2(A,Y);

input A;
input clk;

output Y;

wire wa;
wire w2;
wire w3;
wire w4;
wire w5;
wire w6;
wire w8;


AND2X1 U4(
	.A(w2),
	.B(wa),
	.Y(w5)
);

INVX1 inverter2(
	.A(wa),
	.Y(w4)
);

INVX1 inverter2(
	.A(w8),
	.Y(w6)
);

DFFPOSX1 U1(
	.CLK(clk),
	.D(w4),
	.Q(w2)
);

DFFPOSX1 U2(
	.CLK(clk),
	.D(w5),
	.Q(w8)
);

assign wa = A;
assign Y = w6;

endmodule;
