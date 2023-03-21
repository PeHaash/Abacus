// # define CODE_IS_IN_DEBUG_MODE
#ifdef CODE_IS_IN_DEBUG_MODE
	#ifndef _DEBUG_MACROS_
		#define _DEBUG_MACROS_
		#include<iostream>
		#define DebugFlag std::cerr <<">>>>>HERE:"<<__FILE__<<"\tLine: @"<<__LINE__<<std::endl;
		#define DebugLogg(x) std::cout<<"!  DATA:\t"<<#x<<"\t:"<<x<<std::endl;
	#endif
#else
	#define DebugFlag
	#define DebugLogg(x)
#endif