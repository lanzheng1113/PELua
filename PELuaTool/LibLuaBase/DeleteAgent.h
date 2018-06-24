/**
 * \file DeleteAgent.h 
 * \brief 指针释放的代理类定义。
 * \author duocore
 * \date 2016/12/21
 */
#pragma once

namespace 
{

class NonCopyable 
{
protected:

	NonCopyable(){;}

	~NonCopyable(){;}

private:

	/**
	 * Copy constructor is private.
	 */
	NonCopyable(const NonCopyable &);

	/**
	 * Copy assignement is private.
	 */
	const NonCopyable & operator=(const NonCopyable &);
};

/**
 * \class DataArrayDeleteAgent
 * DataArrayDeleteAgent 用于在未使用boost::scope_array的地方代替它使用。
 *
 * 作用是用于释放一块内存。当这个类的对象离开作用域后自动释放它内部的指针指向的内存块。
 * 注意仅用于申请内存数组时使用。
 * 这里使用template，仅仅用于方便调试的时候知道是哪一种类型。
 */
template <class T>
class DataArrayDeleteAgent : public NonCopyable
{
public:
	DataArrayDeleteAgent(T* ptr)
	{
		m_ptr = ptr;
	}
	~DataArrayDeleteAgent()
	{
		if (m_ptr)
		{
			delete [] m_ptr;
		}
	}
protected:
private:
	T* m_ptr;
};

}




