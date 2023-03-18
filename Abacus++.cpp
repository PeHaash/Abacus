#include<vector>
#include<string>

#include"Abacus++.h"



const char Hex[] = "0123456789ABCDEF";
std::string ByteToHex(unsigned char n){
	std::string ret="  ";
	ret[0] = Hex[(n>>4)];
	ret[1] = Hex[(n&15)];
	return ret;
}

namespace Abacus{
	Integer::Integer(int num){
		Number.push_back(abs(num));
		Sign = (num>=0)?true:false;
	}

	// std::string Integer::Print(){
	// 	return "Not Implemented Yet!";
	// }

	std::string Integer::InHex(){
		std::string ret = Sign?"":"-";

		for(int i = Number.size()-1;i>=0;i--){
			for(int j = MEM_BLOCK_SIZE-1; j>=0;j--){
				ret+=ByteToHex(char(*(((char *)&Number[i])+j)));
				ret+=" ";
			}
		}

		return ret;
	}
}