/**
 * \file DeleteAgent.h 
 * \brief ָ���ͷŵĴ����ඨ�塣
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
 * DataArrayDeleteAgent ������δʹ��boost::scope_array�ĵط�������ʹ�á�
 *
 * �����������ͷ�һ���ڴ档�������Ķ����뿪��������Զ��ͷ����ڲ���ָ��ָ����ڴ�顣
 * ע������������ڴ�����ʱʹ�á�
 * ����ʹ��template���������ڷ�����Ե�ʱ��֪������һ�����͡�
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




