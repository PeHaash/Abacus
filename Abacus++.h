#ifndef __ABACUSPP__
#define __ABACUSPP__

#include<vector>
#include<string>

#define MEM_BLOCK unsigned int
#define MEM_BLOCK_SIZE 4
#define MEM_BLOCK_MAX 0xFFFFFFFF
#define MEM_BLOCK_MIN 0x0
// #define BYTE unsigned char

namespace Abacus{



class Integer{
	private:
		std::vector<MEM_BLOCK> Number;
		bool Sign;
		Integer& Crement(char change, MEM_BLOCK Threshold);
	public:
		Integer(int num);
		Integer& IncrementOne();
		Integer& DecrementOne();
		std::string InDec();
		std::string InHex();
		bool isSameSign(Integer b);

};


}
#endif
/*
	123 is treated as decimal.
	0123 is treated as octal.
	0x123 is treated as hexadecimal.
	0b101 is treated as binary (C++14).
*/