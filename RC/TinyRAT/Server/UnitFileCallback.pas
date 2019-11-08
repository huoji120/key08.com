unit UnitFileCallback;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Gauges, ComCtrls;

type
  TFormCallback = class(TForm)
    SBarEx: TStatusBar;
    GaugeEx: TGauge;
    Button_Cancel: TButton;
    procedure Button_CancelClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  FormCallback: TFormCallback;

implementation

{$R *.dfm}

procedure TFormCallback.Button_CancelClick(Sender: TObject);
begin
  MessageBox(Application.Handle, '��ѽ~�㶼��ˮ!С׳ѽ~��������!', nil, 0);
end;

end.
