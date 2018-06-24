#pragma once

/**
 * 定义数据头标志，但凡带版本信息的.dat文件都有此内容
 */
#define file_data_head_flag ('KJVE')

/**
 * 定义配置数据类型
 */
#define	data_file_unknow_data			(0)			///< 未知类型。视程序如何使用
#define data_file_content_type_ini		(1)			///< 实际数据内容为INI
#define data_file_content_type_xml		(2)			///< 实际数据内容为XML
#define data_file_content_type_json		(3)			///< 实际数据内容为JSON
#define data_file_content_sqlite		(4)			///< sqlite

/**
 * KJVERSION定义版本号格式
 */
typedef struct tagDATA_FILE_VERSION
{
	unsigned short major;		///< 主版本号
	unsigned short minor;		///< 次版本号
	unsigned short third;		///< 第三版本号
	unsigned short forth;		///< 第四版本号
}DATA_FILE_VERSION;

/**
 * DAT文件头的定义
 */
typedef struct tag_DATA_FILE_HEADER
{
	UINT32 flag;				///< ='KJVE'
	unsigned short head_length;	///< 头长度
	DATA_FILE_VERSION head_version;		///< 文件头的版本号（注：保留备用）
	DATA_FILE_VERSION data_version;		///< 此数据文件的版本
	BYTE data_type;				///< 此数据文件的文件头之后的内容的格式
	BYTE crypt_type;			///< 此数据文件的加密方式
	BYTE crypt_key[32];			///< 此数据文件的加密KEY
	USHORT key_len;				///< 加密秘钥的长度
	UINT32 dwCheckSum;			///< 此数据文件的校验和（注：目前被设定为文件的总大小）
	USHORT data_offset;			///< 此数据文件的"实际数据（加密后）"内容开始位置在文件中的偏移
	UINT32 data_size;			///< 此数据文件的"实际数据(加密后)"内容的大小。注意：这个值允许为0，表示没有数据的空文件！
}DATA_FILE_HEADER,*PDATA_FILE_HEADER;
