//created by xiaoshi 6/26/2016
//------------------------------------------------
//Singleton:
//生成二进制数据的类
//-----------------------------------------------

#include <cassert>

template <typename T> 
class Singleton
{
	static T* ms_Singleton;

public:
	Singleton( void )
	{
		assert( !ms_Singleton );
		int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
		ms_Singleton = (T*)((int)this + offset);
	}
	~Singleton( void )
	{  assert( ms_Singleton );  ms_Singleton = 0;  }
	static T& GetSingleton( void )
	{  assert( ms_Singleton );  return ( *ms_Singleton );  }
	static T* getInstance( void )
	{  return ( ms_Singleton );  }
};

template <typename T> T* Singleton <T>::ms_Singleton = 0;
