{
  ͨ�õ�Ԫ
}
unit VarUnit;

interface

uses
  Windows, SysUtils;

const
  Sock_Error    = $0FFFFFFF;
  Sock_Close    = $00FFFFFF;
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

function HexToInt(S: String): DWORD;
function FileSize(SizeInBytes: dword): string;
function MakeSocketCmd(dwSockCmd: DWORD): String;
function Split(Input: string; Deliminator: string; Index: integer): string;
function SplitStr(var StrBuffer: String; StrSub: String): String;

function GetPointerSize(lpBuffer: Pointer): DWORD;
function GetFileSizeEx(lpszFileName: Pchar): DWORD;

implementation

function HexToInt(S: String): DWORD;
begin
  Val('$' + S, Result, Result);
end;

function FileSize(SizeInBytes: dword): string;
const
  Formats: array[0..3] of string =  (' Bytes', ' KB', ' MB', ' GB');
  FormatSpecifier: array[Boolean] of string = ('%n', '%.2n');
var
  iLoop: integer;
  TempSize: Real;
begin
  iLoop := -1;
  TempSize := SizeInBytes;
  while (iLoop <= 3) do
  begin
    TempSize := TempSize / 1024;
    inc(iLoop);
    if Trunc(TempSize) = 0 then
    begin
      TempSize := TempSize * 1024;
      Break;
    end;
  end;
  Result := Format(FormatSpecifier[((Frac(TempSize)*10) > 1)], [TempSize]);
  if Copy(Result, Length(Result) - 2, 3) = '.00' then
    Result := Copy(Result, 1, Length(Result) - 3);
  Result := Result + Formats[iLoop];
end;

function MakeSocketCmd(dwSockCmd: DWORD): String;
var
  lpChar: Pchar;
begin
  lpChar := @dwSockCmd;
  Result := lpChar[0] + lpChar[1] + lpChar[2] + lpChar[3];
end;

function GetPointerSize(lpBuffer: Pointer): DWORD;
begin
  if lpBuffer = nil then
    Result := DWORD(-1)
  else
    Result := DWORD(Pointer(Cardinal(lpBuffer) - 4)^) and $7FFFFFFC - 8;
end;

function Split(Input: string; Deliminator: string; Index: integer): string;
var
  StringLoop, StringCount: integer;
  Buffer: string;
begin
  Buffer := '';
  if Index < 1 then Exit;
  StringCount := 0;
  StringLoop := 1;
  while (StringLoop <= Length(Input)) do
  begin
    if (Copy(Input, StringLoop, Length(Deliminator)) = Deliminator) then
    begin
      Inc(StringLoop, Length(Deliminator) - 1);
      Inc(StringCount);
      if StringCount = Index then
      begin
        Result := Buffer;
        Exit;
      end
      else
      begin
        Buffer := '';
      end;
    end
    else
    begin
      Buffer := Buffer + Copy(Input, StringLoop, 1);
    end;
    Inc(StringLoop, 1);
  end;
  Inc(StringCount);
  if StringCount < Index then Buffer := '';
  Result := Buffer;
end;

function SplitStr(var StrBuffer: String; StrSub: String): String;
var
  iPos: Integer;
begin
  Result := '';
  if Length(StrBuffer) = 0 then Exit;
  if Length(StrSub) = 0 then Exit;

  iPos := Pos(StrSub, StrBuffer);
  if iPos = 0 then Exit;
  Result := Copy(StrBuffer, 1, iPos - 1);
  Delete(StrBuffer, 1, iPos);
end;

function GetFileSizeEx(lpszFileName: Pchar): DWORD;
var
  hFile: THandle;
begin
  Result := 0;
  hFile := CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, nil, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if hFile = INVALID_HANDLE_VALUE then Exit;
  Result := GetFileSize(hFile, nil);
  CloseHandle(hFile);
end;

end.
