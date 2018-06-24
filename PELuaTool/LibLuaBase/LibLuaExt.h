/**
 * \file LibLuaExt.h
 * \brief ���ļ������˵���LUAģ��Ľӿ�LuaOpenExtendLib��ͨ����������������LUAģ��:
 * - lfs �ļ�������չ�ӿ�
 * - OsExt ͨ����չ�ӿ�
 * - RegIni ע����INI�ļ�����
 * - curl ����
 * - Firewall ����ǽ
 * - �������׳����˳���������ȫ�ֿ�
 */

#pragma once

/**
 * \defgroup LUA_RUNTIME LUA��֧��
 * \{
 */

/**
 * \brief ����lua �ű���
 * \param lua_State ���͵�ָ��
 * \note ����Ҫʹ��lua_open()�Լ���ص�lua C API����һ��LUA������Ȼ����ĳ����ڵ���LuaOpenExtendLib��
 * �Ϳ���ʹ�ýӿ��ṩ��LUAģ����ִ��LUA�ű��ˡ��������Ŀǰ����ִ��KJSelfFix�����޸��ű���
 */
void LuaOpenExtendLib(PVOID pLuaState);


/**
 * \}
 */
