unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls;

type
  TMainForm = class(TForm)
    Memo: TMemo;
    Button1: TButton;
    MinEdit: TLabeledEdit;
    MaxEdit: TLabeledEdit;
    TotalEdit: TLabeledEdit;
    ModEdit: TLabeledEdit;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.dfm}

function GetLineFunction(GLF_Min, GLF_Max, i: Integer): Integer;
var
  K, B: Real;
begin
	K := (GLF_Max - GLF_Min) / (StrToInt(MainForm.TotalEdit.Text) - 4);
	B := GLF_Max - k * StrToInt(MainForm.TotalEdit.Text);
	result := Round(k * i + b);
end;

procedure TMainForm.Button1Click(Sender: TObject);
var
  I, J: Integer;
begin
  MainForm.Memo.Clear;
  For I := 4 to 32
   do
    begin
      J := GetLineFunction(StrToInt(MainForm.MinEdit.Text), StrToInt(MainForm.MaxEdit.Text), I);
      MainForm.Memo.Lines.Add(IntToStr(I) + ': ' + IntToStr(J) + ' ---mod---> ' + IntToStr(Round(J * StrToFloat(MainForm.ModEdit.Text))));
    end; 
end;

procedure TMainForm.FormCreate(Sender: TObject);
begin
  MainForm.Memo.Clear;
end;

end.
