object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'HardMod/Autodifficulty'
  ClientHeight = 279
  ClientWidth = 383
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Memo: TMemo
    Left = 8
    Top = 8
    Width = 249
    Height = 263
    Lines.Strings = (
      'Memo')
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Button1: TButton
    Left = 271
    Top = 248
    Width = 104
    Height = 23
    Caption = 'GO!'
    TabOrder = 1
    OnClick = Button1Click
  end
  object MinEdit: TLabeledEdit
    Left = 271
    Top = 24
    Width = 75
    Height = 21
    EditLabel.Width = 16
    EditLabel.Height = 13
    EditLabel.Caption = 'Min'
    TabOrder = 2
    Text = '1'
  end
  object MaxEdit: TLabeledEdit
    Left = 271
    Top = 64
    Width = 75
    Height = 21
    EditLabel.Width = 20
    EditLabel.Height = 13
    EditLabel.Caption = 'Max'
    TabOrder = 3
    Text = '5'
  end
  object TotalEdit: TLabeledEdit
    Left = 271
    Top = 112
    Width = 75
    Height = 21
    EditLabel.Width = 72
    EditLabel.Height = 13
    EditLabel.Caption = 'sv_maxplayers'
    TabOrder = 4
    Text = '16'
  end
  object ModEdit: TLabeledEdit
    Left = 271
    Top = 160
    Width = 75
    Height = 21
    EditLabel.Width = 20
    EditLabel.Height = 13
    EditLabel.Caption = 'Mod'
    TabOrder = 5
    Text = '1,0'
  end
end
