#define IN_NOC

unsigned short int ddr_cfg_2_rbc[] = {
/*
 * [8:7]  bank(n:n bit bank)
 * [6:4]  row(12+n)
 * [3:2]  bank(n:n bit bank)
 * [1:0]  col(9+n),when channel bus width=32bit
 *
 * all config have (13col,3bank,16row,1cs)
 * bank,  row,   bank, col          col bank row(32bit)
 */
	((3<<7)|(3<<4)|(0<<2)|2),
	((0<<7)|(1<<4)|(3<<2)|1),
	((0<<7)|(2<<4)|(3<<2)|1),
	((0<<7)|(3<<4)|(3<<2)|1),
	((0<<7)|(4<<4)|(3<<2)|1),
	((0<<7)|(1<<4)|(3<<2)|2),
	((0<<7)|(2<<4)|(3<<2)|2),
	((0<<7)|(3<<4)|(3<<2)|2),
	((0<<7)|(1<<4)|(3<<2)|0),
	((0<<7)|(2<<4)|(3<<2)|0),
	((0<<7)|(3<<4)|(3<<2)|0),
	((0<<7)|(2<<4)|(2<<2)|0),
	((0<<7)|(1<<4)|(2<<2)|1),
	((0<<7)|(0<<4)|(2<<2)|2),
	((3<<7)|(4<<4)|(0<<2)|1),
	((0<<7)|(4<<4)|(3<<2)|2),
};
