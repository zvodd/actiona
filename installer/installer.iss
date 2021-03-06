#define MyAppName "Actionaz"
#define MyAppIsBeta 0
#define MyAppPublisher "Actionaz.org"
#define MyAppURL "http://www.actionaz.org/"
#define MyAppExeName "actionaz.exe"

#ifndef MyAppVersion
#error "You need to set the version number"
#endif

#ifndef MyAppBits
#define MyAppBits 32
#endif

[Setup]
#if int(MyAppBits) == 32
AppId={{968D7F40-0B23-457D-AD67-0F7C0012EF1E}
#else
AppId={{098CDAF9-5A9B-4731-9F3C-F3F1DF7490C2}
#endif
AppName={#MyAppName}
AppVersion={#MyAppVersion}
#if MyAppIsBeta == 1
AppVerName={#MyAppName} {#MyAppVersion} beta ({#MyAppBits} bits)
#else
AppVerName={#MyAppName} {#MyAppVersion} ({#MyAppBits} bits)
#endif
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=C:\act_base{#MyAppBits}\LICENSE.txt
OutputDir=C:\actionaz\installer
OutputBaseFilename=actionaz-{#MyAppVersion}-win{#MyAppBits}
SetupIconFile=C:\actionaz\gui\icons\actionaz.ico
Compression=lzma2/ultra
SolidCompression=yes
WizardImageFile=C:\actionaz\installer\start.bmp
WizardSmallImageFile=C:\actionaz\installer\icon.bmp
ChangesAssociations=yes
MinVersion=0,5.1
#if int(MyAppBits) == 32
ArchitecturesAllowed=x64 x86
#else
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
#endif 

[InstallDelete]
Type: files; Name: "{app}\locale\*"

[Registry]
; File extension
Root: HKCR; Subkey: ".ascr"; ValueType: string; ValueName: ""; ValueData: "ActionazScriptFile"; Flags: uninsdeletevalue
Root: HKCR; Subkey: ".acod"; ValueType: string; ValueName: ""; ValueData: "ActionazCodeFile"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "ActionazScriptFile"; ValueType: string; ValueName: ""; ValueData: "Actionaz Script File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "ActionazCodeFile"; ValueType: string; ValueName: ""; ValueData: "Actionaz Code File"; Flags: uninsdeletekey
Root: HKCR; Subkey: "ActionazScriptFile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\actionaz.exe,0"
Root: HKCR; Subkey: "ActionazCodeFile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\actionaz.exe,0"
Root: HKCR; Subkey: "ActionazScriptFile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\actionaz.exe"" ""%1"""
; Installer language
Root: HKCU; Subkey: "Software\Actionaz\Actionaz"; ValueType: string; ValueName: "installerLanguage"; ValueData: "{language}"; Flags: uninsdeletekey
; URL Protocol
Root: HKCR; Subkey: "actionaz"; ValueType: string; ValueName: "URL Protocol"; ValueData: ""; Flags: uninsdeletekey
Root: HKCR; Subkey: "actionaz"; ValueType: string; ValueData: "URL:Actionaz Protocol"; Flags: uninsdeletekey
Root: HKCR; Subkey: "actionaz\DefaultIcon"; ValueType: string; ValueData: "{app}\actexec.exe,0"; Flags: uninsdeletekey
Root: HKCR; Subkey: "actionaz\shell\open\command"; ValueType: string; ValueData: """{app}\actexec.exe"" ""%1"""; Flags: uninsdeletekey
; Path
Root: HKCU; SubKey: "Environment\"; ValueType: string; ValueName: "Path"; ValueData: "{reg:HKCU\Environment\,Path};{app}"

[Code]
// Path uninstall
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  Path, AppDir: string;
  Index: Integer;
begin
  if CurUninstallStep = usUninstall then
  begin
    if RegQueryStringValue(HKEY_CURRENT_USER,
      'Environment\',
      'Path', Path) then
    begin
      AppDir := ExpandConstant('{app}');
      Index := Pos(AppDir, Path);
      Delete(Path, Index-1, Length(AppDir)+1);
      RegWriteStringValue(HKEY_CURRENT_USER,
        'Environment\',
        'Path', Path);
    end;
  end;
end;

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\act_ref{#MyAppBits}\actionaz.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\*"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\actions\*"; DestDir: "{app}\actions"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\sfx\*"; DestDir: "{app}\sfx"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\locale\*"; DestDir: "{app}\locale"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\plugins\sqldrivers\*"; DestDir: "{app}\plugins\sqldrivers"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\plugins\mediaservice\*"; DestDir: "{app}\plugins\mediaservice"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\plugins\playlistformats\*"; DestDir: "{app}\plugins\playlistformats"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\plugins\imageformats\*"; DestDir: "{app}\plugins\imageformats"; Flags: ignoreversion
Source: "C:\act_ref{#MyAppBits}\code\script\*"; DestDir: "{app}\code\script"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, "&", "&&")}}"; Flags: nowait postinstall skipifsilent

