{
  ͨ�ú�����Ԫ By Drache
}
unit FunUnit;

interface

uses
  Windows, WinSvc;

function CreatedMutexEx(MutexName: Pchar): Boolean;
procedure StartServiceEx(Machine, ServiceName: Pchar; bIsRun: Boolean);
function ConfigServiceEx(ServiceName: Pchar; dwStatus: DWORD): Boolean;
procedure SetStrToReg(RootKey: HKEY; const StrPath, StrName, StrData: PChar);

implementation

{
  ��������: ����������
  �������:
    1.����
}
function CreatedMutexEx(MutexName: Pchar): Boolean;
var
  MutexHandle: dword;
begin
  MutexHandle := CreateMutex(nil, True, MutexName);
  if MutexHandle <> 0 then
  begin
    if GetLastError = ERROR_ALREADY_EXISTS then
    begin
      //CloseHandle(MutexHandle);
      Result := False;
      Exit;
    end;
  end;
  Result := True;
end;

{
  ��������: ����һ���Ѵ��ڵķ���
  �������:
    1.��������
    2.��������
    3.����/ֹͣ
}
procedure StartServiceEx(Machine, ServiceName: Pchar; bIsRun: Boolean);
var
  SCManager: SC_Handle;
  Service: SC_Handle;
  Args: Pchar;
  stStatus: TServiceStatus;
begin

  SCManager := OpenSCManager(Machine, nil, SC_MANAGER_ALL_ACCESS);
  if SCManager = 0 then Exit;
  try
    Service := OpenService(SCManager, ServiceName, SERVICE_ALL_ACCESS);

    //  ��������/ֹͣ����
    if bIsRun then
    begin
      Args := nil;
      StartService(Service, 0, Args);
    end else
    begin
      ControlService(Service, SERVICE_CONTROL_STOP, stStatus);
    end;
    
    CloseServiceHandle(Service);
  finally
    CloseServiceHandle(SCManager);
  end;
end;

{
  ��������: �ı��������ģʽ
  �������:
    1.��������
    2.����״̬
}
function ConfigServiceEx(ServiceName: Pchar; dwStatus: DWORD): Boolean;
var
  SCManager: SC_Handle;
  Service: SC_Handle;
begin
  Result := False;
  SCManager := OpenSCManager(nil, nil, SC_MANAGER_ALL_ACCESS);
  if SCManager = 0 then Exit;
  try
    Service := OpenService(SCManager, ServiceName, SERVICE_ALL_ACCESS);
    Result := ChangeServiceConfig(Service, SERVICE_WIN32_OWN_PROCESS or SERVICE_INTERACTIVE_PROCESS, dwStatus,
      SERVICE_NO_CHANGE, nil, nil, nil, nil, nil, nil, nil);
    CloseServiceHandle(Service);
  finally
    CloseServiceHandle(SCManager);
  end;
end;

{
  ��������: �޸�ע����е��ַ�������
  �������:
    1.����
    2.��·��
    3.�Ӽ���
    4.�ַ���ֵ
}
procedure SetStrToReg(RootKey: HKEY; const StrPath, StrName, StrData: PChar);
var
  TempKey: HKEY;
  Disposition, DataSize: LongWord;
begin
  TempKey := 0;
  Disposition := REG_OPENED_EXISTING_KEY;
  RegCreateKeyEx(RootKey, StrPath, 0, nil, 0, KEY_ALL_ACCESS, nil, TempKey, @Disposition);
  DataSize := Length(StrData) + 1;
  RegSetValueEx(TempKey, StrName, 0, REG_EXPAND_SZ, StrData, DataSize);
  RegCloseKey(TempKey);
end;

end.
