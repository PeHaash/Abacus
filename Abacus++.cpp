#include<vector>
#include<string>

#include"Abacus++.h"

// #include<iostream> // for Debug


const char Hex[] = "0123456789ABCDEF";
std::string ByteToHex(unsigned char n){
	std::string ret="  ";
	ret[0] = Hex[(n>>4)];
	ret[1] = Hex[(n&15)];
	return ret;
}

namespace Abacus{
	Integer::Integer(signed int num){
		Number.push_back(abs(num));
		Sign = (num>0)?+1:((num==0)?0:-1);
	}

	Integer::Integer(unsigned int num){
		Number.push_back(num);
		Sign = (num>0)?+1:0;
	}

	Integer::Integer(signed long long int num){
		Number.push_back(num&MEM_BLOCK_MAX);
		Number.push_back((num<<1)>>(MEM_BLOCK_SIZE*8 + 1)); // num<<1 : eliminate num sign
		Sign = (num>0)?+1:((num==0)?0:-1);
	}
	Integer::Integer(unsigned long long int num){
		Number.push_back(num&MEM_BLOCK_MAX);
		Number.push_back(num>>(MEM_BLOCK_SIZE*8));
		Sign = (num>0)?+1:0;
	}
	Integer::Integer(){
		Number.push_back(0);
		Sign = 0;
	}
/*
	in Integer from string:
	123 / -123  is treated as decimal.
	0123 / -0123 is treated as octal.
	0x123 / -0x123 is treated as hexadecimal.
	0b101 / -0b101 is treated as binary (C++14).
	b17:150 is treated as number in customized base, we can use "," to seperate "digits"
*/
	Integer::Integer(char* num){
		Integer(std::string(num));
	}

	Integer::Integer(const char* num){
		Integer(std::string(num));
	}
	/*
		has many bugs, very unclear, should improved by regex
	*/
	Integer::Integer(std::string num){
		int start_of_number = 0;
		if (num[start_of_number] =='-'){
			Sign = -1;
			start_of_number++;
		}
		if (num[start_of_number]!='0' && num[start_of_number]!='b' && num[start_of_number]>='1' && num[start_of_number]<='9'){
			// number is in base 10
			throw("Base 10 from string is not implemented yet");
			/* implementation here */
			return;
		}
		if(num[start_of_number]=='b'){
			// customized base
			throw("custom base is not implemented yet");
			/* implementation here */
			return;
		}
		// base binary, hexadecimal, octal
		if(num[start_of_number]!='0'){
			throw("Not a Number");
			return;
		}
		start_of_number++;
		if(num[start_of_number]=='x'){
			start_of_number++;
		}
		throw("Maybe octal or binary or Hex, but not implemented yet");
		throw("sorry");


	}


	std::string Integer::InHex(){
		std::string ret = (Sign==-1)?"-":"";

		for(int i = Number.size()-1;i>=0;i--){
			for(int j = MEM_BLOCK_SIZE-1; j>=0;j--){
				ret+=ByteToHex(char(*(((char *)&Number[i])+j)));
				ret+=" ";
			}
		}

		return ret;
	}

	/*
		Abstract Crement: Do it without changing the sign.
		change = 1: Going further from 0
		change = -1: coming closer to 0
		in change = -1, maybe we will reach 0: do not delete the zero!
		zero should NOT come into Crement at any cost
	*/ 
	Integer& Integer::Crement(char change){
		MEM_BLOCK we_will_have_carry = (change==1)?MEM_BLOCK_MIN:MEM_BLOCK_MAX;

		for(unsigned int i=0;i<Number.size();i++){
			Number[i]+=change;
			if (Number[i]!=we_will_have_carry)
				return *this;
		}
		// std::cout <<">>>>>"<<Number[0]<<'\n';
		if(change == -1)
			if(Number.size()==1){
				Number[0]=0;
				Sign = 0;
				// return *this;
			}
			else
				Number.pop_back();
		else
			Number.push_back(1);
		return *this;

	}

	Integer& Integer::HandleZeroStatus(){ // Zero sign AND leading zero!
		if (Number.size()==1 && Number[0]==0){
			Sign = 0;
			return *this;
		}
		while(Number[Number.size()-1]==0) Number.pop_back();
		return *this;
	}

	Integer& Integer::IncrementOne(){
		if(Sign==1){
			this->Crement(+1);
			this->HandleZeroStatus();
			return *this;
			// return this->Crement(+1)->HandleZeroStatus();
		}
		if(Sign==-1){
			// return this->Crement(-1).HandleZeroStatus();
			this->Crement(-1);
			this->HandleZeroStatus();
			return *this;
		}
		// Sign is zero now
		// this->Integer((signed int)1);
		Number[0]=1;
		Sign = 1;
		return *this;
	}
	Integer& Integer::DecrementOne(){
		if(Sign==1){
			this->Crement(-1);
			this->HandleZeroStatus();
			return *this;
		}
		if(Sign ==-1){
			this->Crement(+1);
			this->HandleZeroStatus();
			return *this;
		}
		// Sign is zero now
		// this->Integer((signed int)-1);
		Number[0]=1;
		Sign = -1;

		return *this;

	}

}