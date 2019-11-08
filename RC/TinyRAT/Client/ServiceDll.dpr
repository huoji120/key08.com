{
 �ļ���: ServiceDll.dpr
 ����: �滻��svchost.exe������ĳ��ϵͳ����,���������ȫ�ֱ��� ServiceName ����.

 ������,���ɵ�DLL�ļ�������ȫ����.
 ���Ի���: Windows 2003 Server + Delphi 7.0

}

library ServiceDll;

uses
  Windows, WinSvc, SysUtils, Classes;

{ ����ȫ�ֱ��� }
var
  // ���������Ϣ���
  SvcStatsHandle : SERVICE_STATUS_HANDLE;
  // �洢����״̬
  dwCurrState : DWORD;
  // ��������
  ServiceName : PChar = 'BITS';

{ ���Ժ���,������������ı� }
procedure OutPutText(CH:PChar);
var
  FileHandle: TextFile;
  F : Integer;
Begin
  if not FileExists('C:\zztestdll.txt') then  F := FileCreate('C:\zztestdll.txt');
  if F > 0 Then FileClose(F);
  
  AssignFile(FileHandle,'C:\zztestdll.txt');
  Append(FileHandle);
  Writeln(FileHandle,CH);
  Flush(FileHandle);
  CloseFile(FileHandle);
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

{ ��SCM������ͨ�� }
function TellSCM(dwState: DWORD; dwExitCode: DWORD; dwProgress: DWORD ): Boolean;
var
  srvStatus : service_status;
begin
  srvStatus.dwServiceType := SERVICE_WIN32_SHARE_PROCESS;
  dwCurrState := dwState;
  srvStatus.dwCurrentState := dwState;
  srvStatus.dwControlsAccepted := SERVICE_ACCEPT_STOP or SERVICE_ACCEPT_PAUSE_CONTINUE or SERVICE_ACCEPT_SHUTDOWN;
  srvStatus.dwWin32ExitCode := dwExitCode;
  srvStatus.dwServiceSpecificExitCode := 0;
  srvStatus.dwCheckPoint := dwProgress;
  srvStatus.dwWaitHint := 1000;
  Result := SetServiceStatus( SvcStatsHandle, srvStatus );
end;

{ Service ���ƺ��� }
procedure servicehandler(fdwcontrol: integer); stdcall;
begin
  case fdwcontrol of
    SERVICE_CONTROL_STOP:
    begin
      TellSCM( SERVICE_STOP_PENDING, 0, 1 );
      Sleep(10);
      TellSCM( SERVICE_STOPPED, 0, 0 );
    end;

    SERVICE_CONTROL_PAUSE:
    begin
      TellSCM( SERVICE_PAUSE_PENDING, 0, 1 );
      TellSCM( SERVICE_PAUSED, 0, 0 );
    end;

    SERVICE_CONTROL_CONTINUE:
    begin
      TellSCM( SERVICE_CONTINUE_PENDING, 0, 1 );
      TellSCM( SERVICE_RUNNING, 0, 0 );
    end;
    
    SERVICE_CONTROL_INTERROGATE: TellSCM(dwCurrState, 0, 0);
    SERVICE_CONTROL_SHUTDOWN: TellSCM(SERVICE_STOPPED, 0, 0);
  end;
end;

//  �ı��������ģʽ
function ConfigServiceEx(ServiceName: Pchar): Boolean;
var
  SCManager: SC_Handle;
  Service: SC_Handle;
begin
  Result := False;
  SCManager := OpenSCManager(nil, nil, SC_MANAGER_ALL_ACCESS);
  if SCManager = 0 then Exit;
  try
    Service := OpenService(SCManager, ServiceName, SERVICE_ALL_ACCESS);
    Result := ChangeServiceConfig(Service, SERVICE_WIN32_OWN_PROCESS or SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START,
      SERVICE_NO_CHANGE, nil, nil, nil, nil, nil, nil, nil);
    CloseServiceHandle(Service);
  finally
    CloseServiceHandle(SCManager);
  end;
end;

//  service main����
procedure ServiceMain(argc : Integer; var argv : pchar ); stdcall;
begin
{ try
  begin
  if ParamStr(1) <> '' then
  svcname := strNew(PChar(ParamStr(1)))
  else  begin
    svcname := strAlloc(10 * Sizeof(Char));
    svcname := 'none';
  end;
  OutPutText(svcname);
  end
  finally
   strdispose(svcname);
  end;
}

 // ע����ƺ���
  SvcStatsHandle := RegisterServiceCtrlHandler(ServiceName, @servicehandler);
  if (SvcStatsHandle = 0) then
  begin
    OutPutText('Error in RegisterServiceCtrlHandler');
    exit;
  end else
  begin
    if ConfigServiceEx(ServiceName) then OutPutText('Setting Service AutoRun Done!')
    else OutPutText('Setting Service AutoRun Error!');
  end;

  // ��������
  TellSCM(SERVICE_START_PENDING, 0, 1 );
  TellSCM(SERVICE_RUNNING, 0, 0 );
  OutPutText('Service is Running');
  // �������ִ����������Ҫ���Ĵ���
  while ((dwCurrState <> SERVICE_STOP_PENDING) and (dwCurrState <> SERVICE_STOPPED)) do
  begin
    sleep(1000);
  end;
  OutPutText('Service Exit');

end;


// ���������б�
exports
  ServiceMain;

{ dll��ڵ� }
begin
  DllProc := @DLLEntryPoint;
end. 



