#ifndef __ABACUSPP__
#define __ABACUSPP__

#include<vector>
#include<string>
#include"Debug.h"
#define MEM_BLOCK unsigned int
#define BIG_BLOCK unsigned long long int
#define MEM_BLOCK_SIZE 4
#define MEM_BLOCK_MAX 0xFFFFFFFF
#define MEM_BLOCK_MIN 0x0
// #define BYTE unsigned char

namespace Abacus{



class Integer{
	private:
		std::vector<MEM_BLOCK> Number;
		signed char Sign; // +1/0/-1 : to save sanity of the code, we need three types for Sign, a simple true false is bad bc of +/-0
		Integer& Crement(char change);
		Integer& HandleZeroStatus();
		static Integer SignFreeAddition(const Integer &a, const Integer &b); 

	public:
		Integer(signed int num);
		Integer(unsigned int num);
		Integer(signed long long int num);
		Integer(unsigned long long int num);
		Integer();
		Integer(char* num);
		Integer(const char* num);		
		Integer(std::string num);
		Integer& IncrementOne();
		Integer& DecrementOne();
		Integer& Negate();
		std::string InDec();
		std::string InHex();
		bool isSameSign(Integer b);
		void HexFromString(std::string num);
		static Integer Add(const Integer &a, const Integer &b);
};


}
#endif
/*
	123 is treated as decimal.
	0123 is treated as octal.
	0x123 is treated as hexadecimal.
	0b101 is treated as binary (C++14).
*/

/*
	Integer Add(const &Integer a, const &Integer b);
	Integer Subtract(const &Integer a, const &Integer b);
	Integer UnsignedAdd(const &Integer a, const &Integer b); // add a & b, but without paying attention to signs
	Integer GuaranteedSubtract(const &Integer a, const &Integer b); // a subtract when we know a > b
	Integer Absolute(const &Integer a); //input/output can change
	Integer Minus(const &Integer b);    // input/output can change
*/