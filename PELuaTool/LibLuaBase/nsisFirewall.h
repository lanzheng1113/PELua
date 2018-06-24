/**
 * \file nsisFirewall.h
 * \brief ����ǽ�����б�����ɾ������
 */
#pragma once

/**
 * \defgroup FIREWALL_OPERATION ��������ǽ�����б�
 * \{
 */

/**
 * \brief ��ӳ��򵽷���ǽ�����С�
 * \param ExceptionName ������
 * \param ProcessPath �������ȫ·��
 * \return ʹ��SUCCEEDED(hr)���жϺ��������Ƿ�ɹ���
 */
HRESULT AddAuthorizedApplication(LPCTSTR ExceptionName, LPCTSTR ProcessPath);

/**
 * \brief ��ӱ����򵽷���ǽ�����С�
 * \param ExceptionName ������
 * \return ʹ��SUCCEEDED(hr)���жϺ��������Ƿ�ɹ���
 */
HRESULT AddAuthorizedApplication(LPCTSTR ExceptionName); // (overload) add current process

/**
 * \brief �ӷ���ǽ�������Ƴ�����
 * \param �����ȫ·����
 * \return ʹ��SUCCEEDED(hr)���жϺ��������Ƿ�ɹ���
 */
HRESULT RemoveAuthorizedApplication(LPCTSTR ProcessPath);

/**
 * \}
 */