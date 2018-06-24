#pragma once

/**
 * ��������ͷ��־���������汾��Ϣ��.dat�ļ����д�����
 */
#define file_data_head_flag ('KJVE')

/**
 * ����������������
 */
#define	data_file_unknow_data			(0)			///< δ֪���͡��ӳ������ʹ��
#define data_file_content_type_ini		(1)			///< ʵ����������ΪINI
#define data_file_content_type_xml		(2)			///< ʵ����������ΪXML
#define data_file_content_type_json		(3)			///< ʵ����������ΪJSON
#define data_file_content_sqlite		(4)			///< sqlite

/**
 * KJVERSION����汾�Ÿ�ʽ
 */
typedef struct tagDATA_FILE_VERSION
{
	unsigned short major;		///< ���汾��
	unsigned short minor;		///< �ΰ汾��
	unsigned short third;		///< �����汾��
	unsigned short forth;		///< ���İ汾��
}DATA_FILE_VERSION;

/**
 * DAT�ļ�ͷ�Ķ���
 */
typedef struct tag_DATA_FILE_HEADER
{
	UINT32 flag;				///< ='KJVE'
	unsigned short head_length;	///< ͷ����
	DATA_FILE_VERSION head_version;		///< �ļ�ͷ�İ汾�ţ�ע���������ã�
	DATA_FILE_VERSION data_version;		///< �������ļ��İ汾
	BYTE data_type;				///< �������ļ����ļ�ͷ֮������ݵĸ�ʽ
	BYTE crypt_type;			///< �������ļ��ļ��ܷ�ʽ
	BYTE crypt_key[32];			///< �������ļ��ļ���KEY
	USHORT key_len;				///< ������Կ�ĳ���
	UINT32 dwCheckSum;			///< �������ļ���У��ͣ�ע��Ŀǰ���趨Ϊ�ļ����ܴ�С��
	USHORT data_offset;			///< �������ļ���"ʵ�����ݣ����ܺ�"���ݿ�ʼλ�����ļ��е�ƫ��
	UINT32 data_size;			///< �������ļ���"ʵ������(���ܺ�)"���ݵĴ�С��ע�⣺���ֵ����Ϊ0����ʾû�����ݵĿ��ļ���
}DATA_FILE_HEADER,*PDATA_FILE_HEADER;
