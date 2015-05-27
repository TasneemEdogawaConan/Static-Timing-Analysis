module Case1(A,B,clk,Y);

input A;
input B;
input clk;
output Y;
wire w1;
wire w2;
wire w2;
wire w4;
wire w5;
wire w6;

AND2X1 and1(
	.A(A),
	.B(B),
	.Y(w1)
);

OR2X1 or1(
	.A(A),
	.B(B),
	.Y(w2)
);

XOR2X1 xor1(
	.A(w1),
	.B(w2),
	.Y(w3)
);

DFFPOSX1 ff1(
	.CLK(clk),
	.D(w3),
	.Q(w4)
);

NAND2X1 nand1(
	.A(w4),
	.B(w4),
	.Y(w5)
);

DFFPOSX1 ff2(
	.CLK(clk),
	.D(w5),
	.Q(w6)
);

assign Y = w6;

endmodule;
