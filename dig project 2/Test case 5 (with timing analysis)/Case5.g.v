module Case2(A,B,Y);

input A;
input B;
input clk;

output Y;

wire wa;
wire wb;
wire w1;
wire w2;
wire w3;
wire w4;

AND2X1 and(
	.A(wa),
	.B(wb),
	.Y(w1)
);

DFFPOSX1 ff1(
	.CLK(clk),
	.D(w1),
	.Q(w2)
);


XOR2X1 xor(
	.A(wa),
	.B(w2),
	.Y(w3)
);

DFFPOSX1 ff2(
	.CLK(clk),
	.D(w3),
	.Q(w4)
);

assign wa = A;
assign wb = B;
assign Y = w4;

endmodule;
