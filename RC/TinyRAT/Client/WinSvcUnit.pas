{
  ������Ԫ
}
unit WinSvcUnit;

interface

uses
  Windows, WinSvc, WinSvcEx;

procedure InstallServiceEx(lpServiceName, lpDisplayName, lpValue, lpFileName: Pchar);
procedure StartServiceEx(Machine, ServiceName: Pchar);
procedure UninstallServiceEx(lpServiceName: Pchar; stStatus: TServiceStatus);
function ConfigServiceEx(ServiceName: Pchar; dwStatus: DWORD): Boolean;

implementation

{
  ��������: ��װһ������
  �������:
    1.��������
    2.��ʾ����
    3.����˵��
    4.��������
}
procedure InstallServiceEx(lpServiceName, lpDisplayName, lpValue, lpFileName: Pchar);
var
  SCManager: SC_HANDLE;
  srvdesc: PServiceDescription;
  Service: SC_HANDLE;
  lpArgs, lpDesc: Pchar;
begin
  SCManager := OpenSCManager(nil, nil, SC_MANAGER_ALL_ACCESS);
  if SCManager = 0 then Exit;
  try
    Service := CreateService(SCManager, lpServiceName, lpDisplayName, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS or SERVICE_INTERACTIVE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, lpFileName, nil, nil, nil, nil, nil);
    lpArgs := nil;
    if Assigned(ChangeServiceConfig2) then//�ı����˵��
    begin
      lpDesc := lstrcpy(lpDesc, lpValue);
      GetMem(srvdesc, SizeOf(TServiceDescription));
      GetMem(srvdesc^.lpDescription, lstrlen(lpDesc) + 1);
      try
        lpDesc := lstrcpy(srvdesc^.lpDescription, lpDesc);
        ChangeServiceConfig2(Service, SERVICE_CONFIG_DESCRIPTION, srvdesc);
      finally
        FreeMem(srvdesc^.lpDescription);
        FreeMem(srvdesc);
      end;
    end;
    StartService(Service, 0, lpArgs);
    CloseServiceHandle(Service);
  finally
    CloseServiceHandle(SCManager);
  end;
end;

{
  ��������: ����һ���Ѵ��ڵķ���
  �������:
    1.��������
    2.��������
}
procedure StartServiceEx(Machine, ServiceName: Pchar);
var
  SCManager: SC_Handle;
  Service: SC_Handle;
  Args: pchar;
begin
  SCManager := OpenSCManager(Machine, nil, SC_MANAGER_ALL_ACCESS);
  if SCManager = 0 then Exit;
  try
    Service := OpenService(SCManager, ServiceName, SERVICE_ALL_ACCESS);
    Args := nil;
    StartService(Service, 0, Args);
    CloseServiceHandle(Service);
  finally
    CloseServiceHandle(SCManager);
  end;
end;

{
  ��������: ֹͣ��ɾ��һ������
  �������:
    1.��������
    2.����״̬
}
procedure UninstallServiceEx(lpServiceName: Pchar; stStatus: TServiceStatus);
var
  SCManager: SC_HANDLE;
  Service: SC_HANDLE;
begin
  SCManager := OpenSCManager(nil, nil, SC_MANAGER_ALL_ACCESS);
  if SCManager = 0 then Exit;
  try
    Service := OpenService(SCManager, lpServiceName, SERVICE_ALL_ACCESS);
    ControlService(Service, SERVICE_CONTROL_STOP, stStatus);
    DeleteService(Service);
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

end.
