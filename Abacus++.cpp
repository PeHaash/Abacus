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

	bool Integer::isSameSign(Integer b){
		return (b.Sign==Sign)?true:false;
	}

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

	Integer& Integer::Crement(char change, MEM_BLOCK Threshold){
		MEM_BLOCK we_will_have_carry = (change==1)?MEM_BLOCK_MIN:MEM_BLOCK_MAX;

		for(unsigned int i=0;i<Number.size();i++){
			Number[i]+=change;
			if (Number[i]!=we_will_have_carry)
				return *this;
		}
		if(change == -1)
			Number.pop_back();
		else
			Number.push_back(1);
		return *this;

	}

	Integer& Integer::IncrementOne(){
		if(Sign)
			return this->Crement(+1, MEM_BLOCK_MAX);
		else
			return this->Crement(-1, MEM_BLOCK_MIN);
	}
	Integer& Integer::DecrementOne(){
		if(Sign)
			return this->Crement(-1, MEM_BLOCK_MAX);
		else
			return this->Crement(+1, MEM_BLOCK_MIN);

	}

}