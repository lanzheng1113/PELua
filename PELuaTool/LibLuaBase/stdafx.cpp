// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// LibLuaExt.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

/**
 * \mainpage ��ҳ
 * ��һ���ĵ�������LibLuaExtģ��Ĺ��ܺ�ʵ�ַ�ʽ��
 * LibLuaExt�������ΪLUA�ű��ĵ����ӿڡ�
 * �ṩ������luaģ��ӿڣ�
 * - Firewall:���/ɾ������ǽ�������
 * - KaiJiaExt����������ص�һЩ�ӿ�
 * - lfs���ļ�ϵͳ�Ĳ�����
 * - OsExt�����õ�һЩϵͳAPI�ķ�װ��
 * - RegIni��ע����INI�����ļ��Ĳ�����װ��
 * ���ĵ�������20170810��
 *
 * \section intro_sec  ����
 * ģ��Ĺ���Ϊ��ͨLUA��չ���ܡ�
 *   
 * \section main_files ��Ҫ�ļ�
 * ģ�鹦�ܵ�ʵ����Ҫ�������¼����ļ���
 * - LibLuaExt.cpp �ر�ע������ļ��е�LuaOpenExtendLib�������ǵ���LUA�ӿڵ�����ں�����
 * - Firewall.cpp ����ļ�ʵ�������ɾ������ǽ������Ľӿڡ�
 * - KaiJiaExt.cppp ����ļ�ʵ�������ײ�����LUA�ӿڡ�
 * - lfs.cpp ��Դ�����ʵ�����ļ���Ŀ¼������
 * - luacurl.c ��Դ�����ʵ���˶�curl�ķ�װ������libcurl����
 * - OsCommon.cpp ʵ����ϵͳ���õĽӿڡ�
 * - RegIni.cpp ʵ����ע����ini�Ľӿڡ�
 *
 * \section notes_sec ע���
 * - Firewallģ��ʹ��COM�ӿ�ʵ�ַ���ǽ������Ĳ���������������Ŀ¼�µ�netfw.tlb��
 * - lfs��luacurl�ǿ�Դģ�飬����ϸ�����������йȸ衣
 * - ��������ʹ��LuaOpenExtendLib�������ɵ���libluaext����luaģ�顣
 * 
 * \section dependcy ������ϵ
 * - ��ģ�����������¹��� �� libcurl
 * - ������ģ��Ĺ��� �� ��
 *
 * \section CONNTEXT ��ϵ��ʽ
 * ����Դ��ĵ������ʿ��Է����ʼ���������:lanzheng_1123@163.com 
 * Duo C. Fan
 * 2016/8/23 
 */