#ifndef __ABACUSPP__
#define __ABACUSPP__

#include<vector>
#include<string>

#define MEM_BLOCK unsigned int

namespace Abacus{



class Integer{
	private:
		std::vector<MEM_BLOCK> Number;
		bool Sign;
	public:
		Integer(int num);
		std::string Print();

};

// #define SIZEOF(object) (char *)(&object+1) - (char *)(&object)

}
#endif
