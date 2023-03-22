#include<vector>
#include<string>
#include<algorithm>

#include"Abacus++.h"

#include"Debug.h" // for Debug


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


	std::string Integer::InHex(bool leading_zero, char seperator){
		std::string ret = (Sign==-1)?"-":"";

		for(int i = Number.size()-1;i>=0;i--){
			for(int j = MEM_BLOCK_SIZE-1; j>=0;j--){
				ret+=ByteToHex(char(*(((char *)&Number[i])+j)));
				if(seperator !='X')
					ret+=seperator;
			}
		}

		if(!leading_zero){
			int start = (Sign ==-1)?1:0;
			for(unsigned int i = start; i < ret.size();i++)
				if (ret[i]!='0'){
					ret.erase(start,i-start);
					return ret;
				}
			return "0";
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
		while(Number[Number.size()-1]==0)
			Number.pop_back();
		if (Number.size()==0)
			Number.push_back(0);
		if (Number.size()==1 && Number[0]==0){
			Sign = 0;
			return *this;
		}
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

	void Integer::HexFromString(std::string num){
		int number_start = 0;
		Sign = +1;
		if(num[0]=='+'){
			Sign = +1;
			number_start++;
		}
		else if(num[0]=='-'){
			Sign = -1;
			number_start++;
		}
		Number.clear();
		for(int i = num.size()-1,digit = 0;i>=number_start;i--,digit++){
			if(digit%8==0)
				Number.push_back(0);
			char n = (num[i]<='9' && num[i]>='0')?num[i]-'0':
						(num[i]>='a' && num[i]<='f')?(num[i]-'a'+10):
						(num[i]>='A' && num[i]<='f')?(num[i]-'A'+10):throw("Not a valid digit");
			// std::cout<<num[i]<<' '<<int(n)<<std::endl;
			Number[digit/8]+=n<<((digit%8)*4);

		}
		this->HandleZeroStatus();
	}

	Integer& Integer::Negate(){
		Sign = -Sign;
		return *this;
	}


	// to many Ifs here, can be optimized drastically #optimization
	Integer Integer::Add(const Integer &a, const Integer &b){
		if (a.Sign == 0)
			return b;
		if (b.Sign == 0)
			return a;
		if (a.Sign == +1 && b.Sign == +1)
			return SignFreeAddition(a,b);
		if (a.Sign == -1 && b.Sign == -1)
			return SignFreeAddition(a,b).Negate();
		if (a.Sign == +1 && b.Sign == -1){
			int s = SignFreeComparison(a, b);
			if (s==0)
				return Integer(0);
			if (s==+1) // |b| > a
				return SignFreeSubtraction(b, a).Negate();
			if (s==-1) // a > |b|
				return SignFreeSubtraction(a, b);
		}
		if (a.Sign ==-1 && b.Sign ==+1){ // last situation
			int s = SignFreeComparison(a, b);
			if (s==0)
				return Integer(0);
			if (s==+1) // b > |a|
				return SignFreeSubtraction(b, a);
			if (s==-1) // b < |a|
				return SignFreeSubtraction(a, b).Negate();
		}
		throw("????");
		
	}

	// a - b
	// we can subtract ifs #optimization
	Integer Integer::Subtract(const Integer &a, const Integer &b){
		Integer ret;
		if (b.Sign == 0)
			return a;
		if (a.Sign == 0){
			ret = b;
			return ret.Negate();
		}
		if (a.Sign == +1 && b.Sign == -1)
			return SignFreeAddition(a, b);
		if (a.Sign == -1 && b.Sign == +1)
			return SignFreeAddition(a, b).Negate();
		if (a.Sign == +1 && b.Sign == +1){
			int s = SignFreeComparison(a, b);
			if (s == 0)
				return 0;
			if (s == +1) // a < b
				return SignFreeSubtraction(b, a).Negate();
			if (s == -1) // a > b
				return SignFreeSubtraction(a, b);
		}
		if (a.Sign == -1 && b.Sign == -1){
			int s = SignFreeComparison(a, b);
			if (s == 0)
				return 0;
			// ret = SignFreeSubtraction(b, a);
			if (s == +1) // b > a
				return SignFreeSubtraction(b, a);
			if (s == -1) // a > b
				return SignFreeSubtraction(a, b).Negate();
		}

		throw("??");

	}


	Integer Integer::SignFreeSubtraction(const Integer &a, const Integer &b){ // a > b, a!=b (important!!), we need a - b
		Integer ret; ret.Number.pop_back();ret.Sign = +1;
		int carry_range = b.Number.size();
		int end_range = a.Number.size();
		MEM_BLOCK has_carried_away = 0;
		int pointer = 0;
		for(; pointer < carry_range; pointer++){
			if((b.Number[pointer] > a.Number[pointer])||(has_carried_away && b.Number[pointer]==a.Number[pointer])){
				// can't handle the pressure, should borrow from next house
				ret.Number.push_back(
					(MEM_BLOCK)
					((BIG_BLOCK)a.Number[pointer]
					+(BIG_BLOCK)MEM_BLOCK_MAX)+(BIG_BLOCK)1
					-(BIG_BLOCK)has_carried_away
					-(BIG_BLOCK)b.Number[pointer]);
				has_carried_away = 1; 
			}else{
				ret.Number.push_back(
					a.Number[pointer]
					-has_carried_away
					-b.Number[pointer]);
				has_carried_away = 0;
			}
		}

		for(; pointer < end_range; pointer++){
			if(has_carried_away && a.Number[pointer]==0){
				// can't handle the pressure, should borrow from next house, number = 0, carry= 1
				ret.Number.push_back(MEM_BLOCK_MAX);
				has_carried_away = 1;
			}else{
				ret.Number.push_back(a.Number[pointer] - has_carried_away);
				has_carried_away = 0;
			}
		}
		if(has_carried_away)
			throw("shit?");
		if(ret.Number[ret.Number.size()-1]==0)
			ret.Number.pop_back();

		return ret;
	}

	// +1: b is bigger, -1: a is bigger, 0: equal
	int Integer::SignFreeComparison(const Integer &a, const Integer &b){
		if(a.Number.size()!=b.Number.size())
			return a.Number.size() > b.Number.size()?-1:+1;
		for(int i = a.Number.size()-1;i>=0;i--)
			if(a.Number[i]!=b.Number[i])
				return (a.Number[i] > b.Number[i])?-1:+1;
		return 0;

	}

	Integer Integer::SignFreeAddition(const Integer &a, const Integer &b){
		const Integer& longerr_one = a.Number.size()>b.Number.size()?a:b; // new refrences to work effectiverly
		const Integer& shorter_one = a.Number.size()>b.Number.size()?b:a;
		// Integer ret = new Integer();
		Integer ret; ret.Number.pop_back();ret.Sign = +1;
		BIG_BLOCK value, carry = 0;
		int pointer;
		int carry_range = shorter_one.Number.size(), end_range = longerr_one.Number.size();
		for(pointer = 0; pointer<carry_range; pointer++){
			value = (BIG_BLOCK) shorter_one.Number[pointer] + (BIG_BLOCK)longerr_one.Number[pointer] + carry;
			carry = value >>(MEM_BLOCK_SIZE*8);
			ret.Number.push_back((MEM_BLOCK)(value & MEM_BLOCK_MAX));
		}

		for(; pointer < end_range; pointer++){
			value = (BIG_BLOCK)longerr_one.Number[pointer] + carry;
			carry = value >>(MEM_BLOCK_SIZE*8);
			ret.Number.push_back((MEM_BLOCK)(value & MEM_BLOCK_MAX));
		}
		if(carry)
			ret.Number.push_back(carry);
		return ret;

	}



}

/*
			void jam(int a[],int b[],int c[])
			{
			     if ((al(a)==1) && (al(b)==1))   { mjam(a,b,c);return;}
			     if ((al(a)==-1) && (al(b)==-1)) { mjam(a,b,c);man1(c);return;}
			     if ((al(a)==1) && (al(b)==-1))  { if (absmo(a,b)==-1){ mmenha(a,b,c);return;}
			                                       if (absmo(a,b)== 0){ clear(c);return;} 
			                                       if (absmo(a,b)== 1){ mos(b);mmenha(b,a,c);man(b);man1(c);return;}   }
			     if ((al(a)==-1) && (al(b)==1))  { if (absmo(b,a)==-1){ mmenha(b,a,c);return;}
			                                       if (absmo(b,a)== 0){ clear(c);return;} 
			                                       if (absmo(b,a)== 1){ mmenha(a,b,c);man1(c);return;}   }
			     
			} 

*/