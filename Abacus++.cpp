#include<vector>
#include<string>

#include"Abacus++.h"

#include<iostream>


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

	// bool Integer::isSameSign(Integer b){
	// 	return (b.Sign==Sign)?true:false;
	// }

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
		std::cout <<Number[0]<<'\n';
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

	Integer& Integer::HandleZeroStatus(){
		if (Number.size()==1 && Number[0]==0)
			Sign = 0;
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