unit UnitManager;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls, Menus, VarUnit, Sockets, ImgList;

type
  TFormManager = class(TForm)
    SBarEx: TStatusBar;
    PageEx: TPageControl;
    Tab_Process: TTabSheet;
    Tab_File: TTabSheet;
    ListView_Process: TListView;
    PM_Process: TPopupMenu;
    N1: TMenuItem;
    N2: TMenuItem;
    ListView_File: TListView;
    PM_File: TPopupMenu;
    N3: TMenuItem;
    N4: TMenuItem;
    N5: TMenuItem;
    N6: TMenuItem;
    N7: TMenuItem;
    N8: TMenuItem;
    N9: TMenuItem;
    N10: TMenuItem;
    N11: TMenuItem;
    N12: TMenuItem;
    ImgList_File: TImageList;
    SD_File: TSaveDialog;
    OD_File: TOpenDialog;
    procedure PM_ProcessPopup(Sender: TObject);
    procedure N1Click(Sender: TObject);
    procedure N2Click(Sender: TObject);
    procedure PM_FilePopup(Sender: TObject);
    procedure N12Click(Sender: TObject);
    procedure ListView_FileDblClick(Sender: TObject);
    procedure N11Click(Sender: TObject);
    procedure N3Click(Sender: TObject);
    procedure N4Click(Sender: TObject);
    procedure N8Click(Sender: TObject);
    procedure N9Click(Sender: TObject);
    procedure N6Click(Sender: TObject);
    procedure N5Click(Sender: TObject);
  private
    { Private declarations }
    function GetFileIcon(FileName: String): Integer;
  public
    IconList: TStringList;
    ClientSocket: TCustomWinSocket;
    isDownloadFile: Boolean;
    isUploadFile: Boolean;
    BinaryFile: THandle;
    dwBytesDone, dwFileSize: DWORD;
    procedure ClientWork(ClientSocket: TCustomWinSocket; Data: Pointer);
    { Public declarations }
  end;

var
  FormManager: TFormManager;

implementation

{$R *.dfm}

uses
  UnitFileCallback, ShellAPI, CommCtrl;

var
  StrCustomPath, StrFilePath: String;


function TFormManager.GetFileIcon(FileName: String): Integer;
var
  FileInfo: TSHFileInfo;
  Ext    : String;
begin
  Ext := LowerCase(ExtractFileExt(FileName));
  Result := IconList.IndexOf(Ext) + 1;
  if Result = 0 then
  begin
    SHGetFileInfo(PChar(Ext), 0, FileInfo, SizeOf(FileInfo), SHGFI_ICON or SHGFI_USEFILEATTRIBUTES or SHGFI_SMALLICON);
    IconList.Add(Ext);
    Result := ImageList_ReplaceIcon(ImgList_File.Handle, -1, FileInfo.hIcon);
    DestroyIcon(FileInfo.hIcon);
  end else Result := Result - 1;
end;

//  ����ȡ�����߳�
procedure TFormManager.ClientWork(ClientSocket: TCustomWinSocket; Data: Pointer);
var
  dwSocketCmd, I, dwTemp: DWORD;
  StrBuffer, StrTemp: String;
  CStrList: TStringList;
  MinBuffer: TMinBufferHeader;
  MinExBuffer: TMinExBufferHeader;
  dwBytesRead, dwBytesWritten, dwLen: DWORD;
  lpChar: Pointer;
begin
  if ClientSocket.Connected then
  begin
    //  ����ָ��
    dwSocketCmd := PDWORD(Data)^;
    case dwSocketCmd of

      //  ��ȡ�����б�
      Client_GetProcessList:
      begin
        StrBuffer := String(Pchar(@(Pchar(Data)[4])));
        if Length(StrBuffer) < 4 then Exit;

        CStrList := TStringList.Create;
        ListView_Process.Items.Clear;
        CStrList.Text := StrBuffer;

        ListView_Process.Items.BeginUpdate;
        For I := 0 to CStrList.Count - 1 do
        begin
          with ListView_Process.Items.Add do
          begin
            StrTemp := CStrList.Strings[I];
            Caption := SplitStr(StrTemp, '|');
            SubItems.Add(SplitStr(StrTemp, '|'));
          end;
        end;
        ListView_Process.Items.EndUpdate;
        CStrList.Free;
      end;

      //  ��ȡ�����б�
      Get_DiskList:
      begin
        ListView_File.Items.Clear;
        StrBuffer := String(Pchar(@(Pchar(Data)[4])));
        if Length(StrBuffer) = 0 then Exit;
        dwTemp := Length(StrBuffer);
        if dwTemp < 4 then Exit;
        dwTemp := dwTemp div 4;
        ListView_File.Items.BeginUpdate;
        For I := 1 to dwTemp do
        begin
          with ListView_File.Items.Add do
          begin
            ImageIndex := 1;
            Caption := SplitStr(StrBuffer, '|');
          end;
        end;
        ListView_File.Items.EndUpdate;
        SBarEx.Panels.Items[0].Text := StrCustomPath;
      end;

      //  ��ȡĿ¼
      Get_DirList:
      begin
        ListView_File.Items.Clear;
        StrBuffer := String(Pchar(@(Pchar(Data)[4])));
        if Length(StrBuffer) = 0 then Exit;
        ListView_File.Items.BeginUpdate;
        while True do
        begin
          StrTemp := SplitStr(StrBuffer, '|');
          if Length(StrTemp) = 0 then Break;
          with ListView_File.Items.Add do
          begin
            ImageIndex := 0;
            Caption := StrTemp;
          end;
        end;
        ListView_File.Items.EndUpdate;
        if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
        StrTemp := MakeSocketCmd(Get_FileList) + StrCustomPath + '\';
        ClientSocket.SendText(StrTemp);
        SBarEx.Panels.Items[0].Text := StrCustomPath;
      end;

      //  ��ȡ�ļ��б�
      Get_FileList:
      begin
        StrBuffer := String(Pchar(@(Pchar(Data)[4])));
        if Length(StrBuffer) = 0 then Exit;
        CStrList := TStringList.Create;
        CStrList.Text := StrBuffer;
        ListView_File.Items.BeginUpdate;
        for I := 0 to CStrList.Count - 1 do
        begin
          StrBuffer := CStrList.Strings[I];
          if Length(StrBuffer) = 0 then Break;
          with ListView_File.Items.Add do
          begin
            Caption := SplitStr(StrBuffer, '|');
            ImageIndex := GetFileIcon(Caption);
            SubItems.Add(ExtractFileExt(Caption));
            SubItems.Add(FileSize(HexToInt(SplitStr(StrBuffer, '|'))));
          end;
        end;
        CStrList.Free;
        ListView_File.Items.EndUpdate;
      end;

      //  I/O Error
      File_IO_Error:
      begin
        CloseHandle(BinaryFile);

        if isDownloadFile then
        begin
          isDownloadFile := False;
          MessageBox(Application.Handle, '�����ļ�ʧ��!.�ļ��������ڱ���������ռ��!', nil, MB_ICONERROR);
        end;

        if isUploadFile then
        begin
          isUploadFile := False;
          MessageBox(Application.Handle, '�ϴ��ļ�ʧ��!.�ļ������Ѿ�����!', nil, MB_ICONERROR);
        end;
      end;

      //  �ļ����ؿ�ʼ
      File_DownLoadBegin:
      begin
        dwFileSize := PDWORD(@(Pchar(Data)[4]))^;
        if dwFileSize = 0 then
        begin
          MessageBox(Application.Handle, '�ļ�����Ϊ0.�����㲻Ҫ������!', nil, MB_ICONERROR);
          Exit;
        end else
        begin
          BinaryFile := CreateFile(Pchar(StrFilePath), GENERIC_WRITE, FILE_SHARE_WRITE, nil, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
          if BinaryFile = INVALID_HANDLE_VALUE then
          begin
            MessageBox(Application.Handle, '�����ļ�ʧ��!.�ļ��������ڱ���������ռ��!', nil, MB_ICONERROR);
            isDownloadFile := False;
            Exit;
          end;
          dwBytesDone := 0;
          MinBuffer.dwSocketCmd := File_DownLoadBegin;
          ClientSocket.SendBuf(MinBuffer, MIN_BUFFER_SIZE);
          isDownloadFile := True;
          UnitFileCallback.FormCallback.GaugeEx.MinValue := 0;
          UnitFileCallback.FormCallback.GaugeEx.MaxValue := dwFileSize;
          UnitFileCallback.FormCallback.ShowModal;
        end;
      end;

      //  �ļ��������
      File_DownloadEnd:
      begin
        CloseHandle(BinaryFile);
        isDownloadFile := False;
        UnitFileCallback.FormCallback.Close;
        ShowMessage('��ϲ!�ļ��������!');
      end;

      //  �ļ��ϴ���ʼ
      File_UploadBegin:
      begin
        if isUploadFile then
        begin
          BinaryFile := CreateFile(Pchar(StrFilePath), GENERIC_READ, FILE_SHARE_READ, nil, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
          if BinaryFile = INVALID_HANDLE_VALUE then
          begin
            MessageBox(Application.Handle, '�ϴ��ļ�ʧ��!.�ļ��������ڱ���������ռ��!', nil, MB_ICONERROR);
            Exit;
          end;

          dwBytesDone := 0;
          UnitFileCallback.FormCallback.GaugeEx.MinValue := 0;
          UnitFileCallback.FormCallback.GaugeEx.MaxValue := dwFileSize;

          GetMem(lpChar, 4096);
          try
            while True do
            begin
              if dwFileSize > dwBytesDone then
              begin
                ReadFile(BinaryFile, lpChar^, 4096, dwBytesRead, nil);
                ClientSocket.SendBuf(lpChar^, dwBytesRead);
                Inc(dwBytesDone, dwBytesRead);
                UnitFileCallback.FormCallback.GaugeEx.Progress := dwBytesDone;
              end else if dwFileSize = dwBytesDone then
              begin
                isUploadFile := False;
                UnitFileCallback.FormCallback.Close;
                ShowMessage('��ϲ!�ļ��������!');
                Break;
              end;
            end;

          finally
            FreeMem(lpChar);
            CloseHandle(BinaryFile);
          end;
        end else
        begin
          dwFileSize := GetFileSizeEx(Pchar(StrFilePath));
          if dwFileSize = 0 then Exit;
          MinExBuffer.dwSocketCmd := File_UploadBegin;
          MinExBuffer.dwBufferSize := dwFileSize;
          isUploadFile := True;
          Sleep(10);
          ClientSocket.SendBuf(MinExBuffer, MinEx_BUFFER_SIZE);
        end;
      end;

    else
      //  �ж��Ƿ����ļ�������
      if isDownloadFile then
      begin
        dwLen := GetPointerSize(Data);
        if dwLen = 0 then
        begin
          isDownloadFile := False;
          Exit;
        end;
        if dwFileSize > dwBytesDone then WriteFile(BinaryFile, Data^, dwLen, dwBytesWritten, nil);
        Inc(dwBytesDone, dwLen);
        UnitFileCallback.FormCallback.GaugeEx.Progress := dwBytesDone;
      end;

    end;
  end;
end;

//  �����Ҽ�����
procedure TFormManager.PM_ProcessPopup(Sender: TObject);
var
  CListItem: TListItem;
begin
  CListItem := ListView_Process.Selected;
  if Assigned(CListItem) then
  begin
    N1.Enabled := True;
    N2.Enabled := True;
  end else
  begin
    N1.Enabled := True;
    N2.Enabled := False;
  end;
end;

//  ˢ�½���
procedure TFormManager.N1Click(Sender: TObject);
var
  MinBuffer: TMinBufferHeader;
begin
  MinBuffer.dwSocketCmd := Client_GetProcessList;
  ClientSocket.SendBuf(MinBuffer, MIN_BUFFER_SIZE);
end;

//  �ɵ�����
procedure TFormManager.N2Click(Sender: TObject);
var
  MinExBuffer: TMinExBufferHeader;
  CListItem: TListItem;
begin
  CListItem := ListView_Process.Selected;
  if Assigned(CListItem) then
  begin
    MinExBuffer.dwSocketCmd := Client_KillProcess;
    MinExBuffer.dwBufferSize := HexToInt(CListItem.Caption);
    ClientSocket.SendBuf(MinExBuffer, MINEX_BUFFER_SIZE);
    Sleep(10);
    N1Click(Self);
  end;
end;

//  �Ҽ�����
procedure TFormManager.PM_FilePopup(Sender: TObject);
var
  CListItem: TListItem;
begin
  CListItem := ListView_File.Selected;
  if Assigned(CListItem) then
  begin
    Case CListItem.ImageIndex of
      2:
      begin
        N3.Enabled := False;
        N4.Enabled := False;
        N5.Enabled := False;
        N6.Enabled := False;
        N8.Enabled := False;
        N9.Enabled := False;
        N11.Enabled := True;
        N12.Enabled := True;
      end;

      1:
      begin
        N3.Enabled := True;
        N4.Enabled := True;
        N5.Enabled := True;
        N6.Enabled := True;
        N8.Enabled := True;
        N9.Enabled := False;
        N11.Enabled := True;
        N12.Enabled := True;
      end;

      0:
      begin
        N3.Enabled := False;
        N4.Enabled := False;
        N5.Enabled := False;
        N6.Enabled := False;
        N8.Enabled := True;
        N9.Enabled := True;
        N11.Enabled := True;
        N12.Enabled := True;
      end;

    end;
    N1.Enabled := True;
  end else
  begin
    N3.Enabled := False;
    N4.Enabled := False;
    N5.Enabled := False;
    N6.Enabled := False;
    N8.Enabled := True;
    N9.Enabled := False;
    N11.Enabled := True;
    N12.Enabled := True;
  end;
end;

//  ��ȡ�����б�
procedure TFormManager.N12Click(Sender: TObject);
var
  MinBuffer: TMinBufferHeader;
begin
  MinBuffer.dwSocketCmd := Get_DiskList;
  ClientSocket.SendBuf(MinBuffer, MIN_BUFFER_SIZE);
  StrCustomPath := '';
end;

//  ����Ŀ¼                         
procedure TFormManager.ListView_FileDblClick(Sender: TObject);
var
  StrDir: String;
  CListItem: TListItem;
begin
  CListItem := ListView_File.Selected;
  if Assigned(CListItem) then
  begin
    //  �ж��Ƿ��Ǵ���
    case CListItem.ImageIndex of
      1:
      begin
        StrDir := CListItem.Caption;
        if Length(StrDir) = 0 then Exit;
        StrCustomPath := StrDir;
      end;

      0:
      begin
        if Length(StrCustomPath) = 0 then Exit;
        StrDir := CListItem.Caption;
        if Length(StrDir) = 0 then Exit;
        if StrDir = '.' then Exit;

        if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
        if StrDir = '..' then
        begin
          if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
          StrDir := ExtractFilePath(StrCustomPath);
          StrCustomPath := StrDir;
        end else
        begin
          StrCustomPath := StrCustomPath + '\' + StrDir;
          StrDir := StrCustomPath + '\';
        end;
      end;
    end;
    StrDir := MakeSocketCmd(Get_DirList) + StrDir;
    ClientSocket.SendText(StrDir);
  end;
end;

//  ˢ�µ�ǰĿ¼
procedure TFormManager.N11Click(Sender: TObject);
var
  StrDir: String;
begin
  if Length(StrCustomPath) < 2 then
  begin
    StrDir := MakeSocketCmd(Get_DiskList);
  end else
  begin
    if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
    StrDir := StrCustomPath + '\';
    StrDir := MakeSocketCmd(Get_DirList) + StrDir;
  end;
  ClientSocket.SendText(StrDir);
end;

//  Զ��ִ���ļ�
procedure TFormManager.N3Click(Sender: TObject);
var
  StrDir: String;
  CListItem: TListItem;
begin
  CListItem := ListView_File.Selected;
  if Assigned(CListItem) then
  begin
    StrDir := CListItem.Caption;
    if Length(StrDir) = 0 then Exit;

    if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
    StrDir := StrCustomPath + '\' + StrDir;
    StrDir := MakeSocketCmd(File_Execute) + StrDir;
    ClientSocket.SendText(StrDir);
  end;
end;

//  ɾ���ļ�
procedure TFormManager.N4Click(Sender: TObject);
var
  StrDir: String;
  CListItem: TListItem;
begin
  CListItem := ListView_File.Selected;
  if Assigned(CListItem) then
  begin
    StrDir := CListItem.Caption;
    if Length(StrDir) = 0 then Exit;

    if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
    StrDir := StrCustomPath + '\' + StrDir;
    StrDir := MakeSocketCmd(File_Delete) + StrDir;
    ClientSocket.SendText(StrDir);
    Sleep(10);
    N11Click(Self);
  end;
end;

//  �½��ļ���
procedure TFormManager.N8Click(Sender: TObject);
var
  StrDir: String;
begin
  if Length(StrCustomPath) < 2 then Exit;
  StrDir := InputBox('New Directory', 'Input Directory Name:', '');
  if Length(StrDir) = 0 then Exit;

  if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
  StrDir := StrCustomPath + '\' + StrDir;
  StrDir := MakeSocketCmd(Dir_New) + StrDir;
  ClientSocket.SendText(StrDir);
  Sleep(10);
  N11Click(Self);
end;

//  ɾ���ļ���
procedure TFormManager.N9Click(Sender: TObject);
var
  StrDir: String;
  CListItem: TListItem;
begin
  CListItem := ListView_File.Selected;
  if Assigned(CListItem) then
  begin
    StrDir := CListItem.Caption;
    if Length(StrDir) = 0 then Exit;

    if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
    StrDir := StrCustomPath + '\' + StrDir;
    StrDir := MakeSocketCmd(Dir_Delete) + StrDir;
    ClientSocket.SendText(StrDir);
    Sleep(10);
    N11Click(Self);
  end;
end;

//  �����ļ�
procedure TFormManager.N6Click(Sender: TObject);
var
  StrDir, StrFile: String;
  CListItem: TListItem;
begin
  CListItem := ListView_File.Selected;
  if Assigned(CListItem) then
  begin
    StrDir := CListItem.Caption;
    if Length(StrDir) = 0 then Exit;

    SD_File.FileName := StrDir;
    if SD_File.Execute then
    begin
      StrFile := SD_File.FileName;
      if Length(StrFile) < 4 then
      begin
        MessageBox(Application.Handle, '�ļ�·����Ч!,��ѡ����ȷ�ı���λ��!', nil, MB_ICONERROR);
        Exit;
      end;
      if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
      StrDir := StrCustomPath + '\' + StrDir;
      StrDir := MakeSocketCmd(File_DownLoadBegin) + StrDir;
      isDownloadFile := False;
      StrFilePath := StrFile;
      ClientSocket.SendText(StrDir);
    end;
  end;
end;

//  �ϴ��ļ�
procedure TFormManager.N5Click(Sender: TObject);
var
  StrDir, StrFile: String;
  CListItem: TListItem;
begin
  CListItem := ListView_File.Selected;
  if Assigned(CListItem) then
  begin
    StrDir := CListItem.Caption;
    if Length(StrDir) = 0 then Exit;

    if OD_File.Execute then
    begin
      StrFile := OD_File.FileName;
      if Not (FileExists(StrFile)) then
      begin
        MessageBox(Application.Handle, '�ļ�·����Ч!,��ѡ����ȷ���ļ�!', nil, MB_ICONERROR);
        Exit;
      end;

      dwFileSize := GetFileSizeEx(Pchar(StrFile));
      if dwFileSize = 0 then
      begin
        MessageBox(Application.Handle, '�ϴ�0�ֽڵ��ļ��㲻�о�������?�Ͳ������ϴ�!', nil, MB_ICONERROR);
        Exit;
      end;

      if StrCustomPath[Length(StrCustomPath)] = '\' then Delete(StrCustomPath, Length(StrCustomPath), 1);
      StrDir := StrCustomPath + '\' + ExtractFileName(StrFile);
      StrDir := MakeSocketCmd(File_UploadBegin) + StrDir;
      isUpLoadFile := False;
      StrFilePath := StrFile;
      ClientSocket.SendText(StrDir);
      UnitFileCallback.FormCallback.ShowModal;
    end;
  end;
end;

end.
