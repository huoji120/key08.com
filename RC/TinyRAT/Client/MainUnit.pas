{
  ��������,�Իص�������ʽ����
}
unit MainUnit;

interface

uses
  Windows, TlHelp32, SocketUnit, VarUnit, FuncUnit;

function GetProcessList(): String;            //  �оٽ����б�
procedure KillProcessByPID(dwPID: DWORD);     //  kill process

function GetDriveList(): String;              //  ��ȡ�����б�
function ListFiles(iFileOrDir: Integer; Directory: String): string; //  ��ȡ�ļ���Ŀ¼�б�
function DownloadFile(stSocket: TClientSocket; StrFileName: String): Boolean;
function UploadFile(stSocket: TClientSocket; StrFileName: String): Boolean;

implementation

//  ��ȡ�����б�
function GetProcessList():  String;
var
  Process32: TProcessEntry32;
  ProcessSnapshot: THandle;
  Module32: TModuleEntry32;
  ModuleSnapshot: THandle;
  StrTemp: String;
begin
  GetDebugPrivs;
  Result := '';
  ProcessSnapshot := CreateToolHelp32SnapShot(TH32CS_SNAPALL, 0);
  Process32.dwSize := SizeOf(TProcessEntry32);
  Process32First(ProcessSnapshot, Process32);
  Process32Next(ProcessSnapshot, Process32);
  repeat
    ModuleSnapshot := CreateToolHelp32SnapShot(TH32CS_SNAPMODULE, Process32.th32ProcessID);
    Module32.dwSize := SizeOf(TModuleEntry32);
    Module32First(ModuleSnapshot, Module32);
    StrTemp := String(Module32.szExePath);
    CloseHandle(ModuleSnapshot);
    Result := Result + IntToHex(Process32.th32ProcessID, 8)  + '|' + String(Module32.szExePath) + '|' + #$0D#$0A;
  until not (Process32Next(ProcessSnapshot, Process32));
  CloseHandle(ProcessSnapshot);
end;

//  kill PID
procedure KillProcessByPID(dwPID: DWORD);
var
  ProcessHandle: THandle;
begin
  ProcessHandle := OpenProcess(PROCESS_TERMINATE, False, dwPID);
  TerminateProcess(ProcessHandle, 0);
end;

////////////////////////////////////////////////////////////////////////////////
//
//  ��ȡ������Ϣ
function GetDriveList(): String;
var
  szBuffer: Array[0..MAX_PATH] of char;
  I, iDriveCount: Integer;
begin
  Result := '';
  iDriveCount := GetLogicalDriveStrings(MAX_PATH, szBuffer) div 4;
  for I := 0 to iDriveCount - 1 do
  begin
    szBuffer[(I * 4) + 3] := '|';
  end;
  Result := String(szBuffer);
end;

//  ��ȡĿ¼�ļ���Ϣ
function ListFiles(iFileOrDir: Integer; Directory: String): string;
var
  FileName: string;
  FindHandle: THandle;
  SearchRec: TWIN32FindData;
begin
  Result := '';
  FindHandle := FindFirstFile(PChar(Directory + '*.*'), SearchRec);
  if FindHandle <> INVALID_HANDLE_VALUE then
  repeat
    FileName := SearchRec.cFileName;
    if iFileOrDir = 0 then
    begin
      if ((SearchRec.dwFileAttributes and FILE_ATTRIBUTE_DIRECTORY) <> 0) then
      begin
        Result := Result + FileName + '|';
      end;
    end else
    begin
      if ((SearchRec.dwFileAttributes and FILE_ATTRIBUTE_DIRECTORY) = 0) then
      begin
        Result := Result + FileName + '|' + IntToHex(SearchRec.nFileSizeLow, 8) + '|' + #$D#$A;
      end;
    end;
  until (FindNextFile(FindHandle,SearchRec) = False);
  FindClose(FindHandle);
end;

//  �����ļ�
function DownloadFile(stSocket: TClientSocket; StrFileName: String): Boolean;
var
  BinaryFile: THandle;
  MiniBuffer: TMinBufferHeader;
  MinExBuffer: TMinExBufferHeader;
  lpBinaryBuffer: Pointer;
  dwBytesRead, dwBytesDone, dwFileSize: DWORD;
begin
  Result := False;
  BinaryFile := CreateFile(Pchar(StrFileName), GENERIC_READ, FILE_SHARE_READ, nil, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if BinaryFile = INVALID_HANDLE_VALUE then Exit;
  MinExBuffer.dwSocketCmd := File_DownLoadBegin;
  dwFileSize := GetFileSize(BinaryFile, nil);
  if dwFileSize = 0 then
  begin
    Result := True;
    Exit;
  end;
  MinExBuffer.dwBufferSize := dwFileSize;
  Sleep(1);
  stSocket.SendBuffer(MinExBuffer, MinEx_BUFFER_SIZE);

  //  �ȴ�����ָ��
  stSocket.ReceiveBuffer(MiniBuffer, MIN_BUFFER_SIZE);
  if MiniBuffer.dwSocketCmd = File_DownLoadBegin then
  begin
    dwBytesDone := 0;
    GetMem(lpBinaryBuffer, 4096);
    try
      while True do
      begin
        ReadFile(BinaryFile, lpBinaryBuffer^, 4096, dwBytesRead, nil);
        //  ��ȡ�ļ��ɹ�
        if (dwBytesRead > 0) then
        begin
          Sleep(1);
          stSocket.SendBuffer(lpBinaryBuffer^, dwBytesRead);
          Inc(dwBytesDone, dwBytesRead);          
        end else
        begin
          if dwBytesDone = dwFileSize then
          begin
            Result := True;
            Break;
          end;
        end;
      end;
    finally
      FreeMem(lpBinaryBuffer);
    end;
  end;
  CloseHandle(BinaryFile);
end;

//  �ϴ��ļ�
function UploadFile(stSocket: TClientSocket; StrFileName: String): Boolean;
var
  BinaryFile: THandle;
  MiniBuffer: TMinBufferHeader;
  MinExBuffer: TMinExBufferHeader;
  lpBinaryBuffer: Pointer;
  dwResult, dwBytesWritten, dwBytesDone, dwFileSize: DWORD;
begin
  Result := False;

  BinaryFile := CreateFile(Pchar(StrFileName), GENERIC_WRITE, FILE_SHARE_WRITE, nil, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if BinaryFile = INVALID_HANDLE_VALUE then Exit;
  //  ���Ϳ�ʼ�ϴ�ָ��
  MiniBuffer.dwSocketCmd := File_UploadBegin;
  Sleep(1);
  stSocket.SendBuffer(MiniBuffer, MIN_BUFFER_SIZE);
  //  �����ļ�����ָ��
  stSocket.ReceiveBuffer(MinExBuffer, MinEx_BUFFER_SIZE);
  if MinExBuffer.dwSocketCmd = File_UploadBegin then
  begin
    dwFileSize := MinExBuffer.dwBufferSize;
    dwBytesDone := 0;
    MiniBuffer.dwSocketCmd := File_UploadBegin;
    stSocket.SendBuffer(MiniBuffer, MIN_BUFFER_SIZE);
    GetMem(lpBinaryBuffer, 4096);
    try
      //  ѭ�������ļ�����
      While True do
      begin
        if dwFileSize > dwBytesDone then
        begin
          dwResult := stSocket.ReceiveBuffer(lpBinaryBuffer^, 4096);
          WriteFile(BinaryFile, lpBinaryBuffer^, dwResult, dwBytesWritten, nil);
          Inc(dwBytesDone, dwBytesWritten);
        end else if dwFileSize = dwBytesDone then
        begin
          Result := True;
          Exit;
        end;
      end;
    finally
      FreeMem(lpBinaryBuffer);
      CloseHandle(BinaryFile);
    end;
  end;
end;

end.
