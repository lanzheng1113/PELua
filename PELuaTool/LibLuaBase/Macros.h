/**
 * \file Macros.h
 * \brief ���ļ�������һ���Զ��������ķ��� ReplacePathMacro 
 * ����ӿڽ�һ���Զ����·������${TEMP}ת��Ϊȫ·��"C:\Users\Duo\Temp"��
 *
 * Ŀǰ֧�ֵ�·������
 * <ul>
 *    <li>INSTDIR ���������Ŀ¼��ע��<b>���ǰ�װĿ¼</b></li>
 *    <li>SYSTEM32 X:\\windows\\system32</li>
 *    <li>SYSWOW64 X:\\windows\\SysWOW64</li>
 *    <li>UPDATE ��������Ŀ¼\\UPDATE </li>
 *    <li>����֧�ֵĺ��<a href="https://msdn.microsoft.com/en-us/library/windows/desktop/bb762494(v=vs.85).aspx">CSIDL</a>�Ķ���</li>
 * </ul>
 * \author fanduoduo 
 * \date 2016/11/10
 */

#pragma once
#include <string>
namespace os
{
	/**
	 * \brief ��һ���Զ����·������${TEMP}ת��Ϊȫ·��"C:\Users\Duo\Temp"��
	 * \param KPCmd ������Զ���·����
	 * \return ����ȫ·����
	 */
	std::string ReplacePathMacro( const std::string& KPCmd );
}