#define RoboVersion "1.0.4"

#define ReleaseType "Release"
#define ReleaseName "Roboverb"
#define RoboPkgBase "Roboverb-Windows"

; Uncomment these to make the demo installer. don't forget to uncomment the
; previous defines for the full version

;#define ReleaseType "Demo"
;#define ReleaseName "Roboverb Demo"
;#define RoboPkgBase "RoboverbDemo-Windows"

[Setup]
AppName={#ReleaseName}
AppVersion={#RoboVersion}
DefaultDirName={pf}\Kushview\Roboverb
DisableDirPage=yes
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\MyProg.exe
OutputDir=C:\Users\mfisher\workspace\kushview\kv-plugins\build
AppCopyright=Copyright 2015 Kushview, LLC
AppId={{FE624D60-74F2-414A-868F-5FB4C99936D5}
AppPublisher=Kushview, LLC
AppPublisherURL=https://kushview.net
AppSupportURL=https://kushview.net/support
AppUpdatesURL=https://kushview.net/support/updates
ShowLanguageDialog=no
VersionInfoVersion={#RoboVersion}
VersionInfoCompany=Kushview
VersionInfoTextVersion=v{#RoboVersion}
VersionInfoProductName=Roboverb
VersionInfoProductVersion={#RoboVersion}
VersionInfoProductTextVersion=Roboverb v{#RoboVersion}
OutputBaseFilename={#RoboPkgBase}-{#RoboVersion}
SetupIconFile=KushviewIcon.ico

[Files]
Source: "..\..\build\x64\{#ReleaseType}\Roboverb.dll";  Flags: ignoreversion; DestDir: "{code:GetVst64Path}\Kushview"; Check: IsWin64;
Source: "..\..\build\x86\{#ReleaseType}\Roboverb.dll";  Flags: ignoreversion; DestDir: "{code:GetVst32Path}\Kushview"
Source: "..\..\build\x64\{#ReleaseType}\Roboverb.vst3"; Flags: ignoreversion; DestDir: "{code:GetVst364Path}\Kushview"; Check: IsWin64;
Source: "..\..\build\x86\{#ReleaseType}\Roboverb.vst3"; Flags: ignoreversion; DestDir: "{code:GetVst332Path}\Kushview"
Source: "..\..\build\x86\{#ReleaseType}\Roboverb.exe";  Flags: ignoreversion; DestDir: "{code:GetStandalonePath}"

[Registry]
;Root: HKCU; Subkey: "Software\My Company"; Flags: uninsdeletekeyifempty
;Root: HKCU; Subkey: "Software\My Company\My Program"; Flags: uninsdeletekey
;Root: HKCU; Subkey: "Software\My Company\My Program\Settings"; ValueType: string; ValueName: "Name"; ValueData: "{code:GetUser|Name}"
;Root: HKCU; Subkey: "Software\My Company\My Program\Settings"; ValueType: string; ValueName: "Company"; ValueData: "{code:GetUser|Company}"
;Root: HKCU; Subkey: "Software\My Company\My Program\Settings"; ValueType: string; ValueName: "DataDir"; ValueData: "{code:GetDataDir}"
; etc.

[Dirs]

[Code]
var
  DataDirPage: TInputDirWizardPage;
  StandaloneDirPage: TInputDirWizardPage;

procedure InitializeWizard;
begin
  { Create the pages }
  DataDirPage := CreateInputDirPage(wpSelectDir,
    'Select Plugin Directories', 'Where should the plugins be installed?',
    'Select the folder in which Setup should install plugin files, then click Next.',
    False, '');

  if IsWin64 then begin
    DataDirPage.Add('64bit VST Destination Path');
    DataDirPage.Add('32bit VST Destination Path');
    DataDirPage.Add('64bit VST3 Destination Path');
    DataDirPage.Add('32bit VST3 Destination Path');
    DataDirPage.Values[0] := ExpandConstant('{pf64}') + '\VstPlugins';
    DataDirPage.Values[1] := ExpandConstant('{pf32}') + '\VstPlugins';
    DataDirPage.Values[2] := ExpandConstant('{cf64}') + '\VST3';
    DataDirPage.Values[3] := ExpandConstant('{cf32}') + '\VST3';
  end
  else begin
    DataDirPage.Add('VST Destination Path');
    DataDirPage.Add('VST3 Destination Path');
    DataDirPage.Values[0] := ExpandConstant('{pf}') + '\VstPlugins';
    DataDirPage.Values[1] := ExpandConstant('{cf}') + '\VST3';
  end

  StandaloneDirPage := CreateInputDirPage(DataDirPage.ID,
    'Select Standalone Directory', 'Where should the standalone app be installed?',
    'Select the folder in which Setup should install standalone application, then click Next.',
    False, '');
  StandaloneDirPage.Add('Standalone Application Path');
  StandaloneDirPage.Values[0] := ExpandConstant('{pf}') + '\Kushview\Roboverb';
end;

function GetVst64Path(Param: String): String;
begin
  Result := DataDirPage.Values[0];
end;

function GetVst32Path(Param: String): String;
begin
  if IsWin64 then begin
    Result := DataDirPage.Values[1];
  end
  else begin
    Result := DataDirPage.Values[0];
  end
end;

function GetVst364Path(Param: String): String;
begin
  Result := DataDirPage.Values[2];
end;

function GetVst332Path(Param: String): String;
begin
  if IsWin64 then begin
    Result := DataDirPage.Values[3];
  end
  else begin
    Result := DataDirPage.Values[1];
  end
end;

function GetStandalonePath(Param: String): String;
begin
  Result := StandaloneDirPage.Values[0];
end;
