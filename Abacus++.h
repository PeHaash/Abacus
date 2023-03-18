#ifndef __ABACUSPP__
#define __ABACUSPP__

#include<vector>
#include<string>

#define MEM_BLOCK unsigned int
#define MEM_BLOCK_SIZE 4

namespace Abacus{



class Integer{
	private:
		std::vector<MEM_BLOCK> Number;
		bool Sign;
	public:
		Integer(int num);
		std::string Print();
		std::string InHex();

};

// #define SIZEOF(object) (char *)(&object+1) - (char *)(&object)

}
#endif
