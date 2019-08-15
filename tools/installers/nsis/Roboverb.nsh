;--------------------------------
; Includes
  !include 'LogicLib.nsh'
  !include "MUI2.nsh"
  !include x64.nsh

;--------------------------------
; Plugin Files
  !define STANDALONE_FILE "..\..\build\${ARCH}\Release\Standalone Plugin\Roboverb.exe"
  !define VST2_FILE "..\..\..\build\${ARCH}\Release\VST\Roboverb.dll"
  !define VST3_FILE "..\..\..\build\${ARCH}\Release\VST3\Roboverb.vst3"

; Init
Function .onInit
  ${If} ${ARCH} == "x64" ; Don't override setup.exe /D=c:\custom\dir
    StrCpy $InstDir "$ProgramFiles64\Kushview\Roboverb"
  ${Else}
    StrCpy $InstDir "$ProgramFiles\Kushview\Roboverb"
  ${EndIf}
FunctionEnd

;--------------------------------
; Branding
  !define MUI_PRODUCT "Roboverb"
  !define MUI_FILE "savefile"
  !define MUI_VERSION "1.0.6"
  !define MUI_BRANDINGTEXT "${MUI_PRODUCT} v${MUI_VERSION}"
  CRCCheck On
  BrandingText "${MUI_BRANDINGTEXT}"

;--------------------------------
;General
  ;Name and file
  Name "Roboverb"
  OutFile "..\..\..\build\${MUI_PRODUCT}-${MUI_VERSION}-${ARCH_HUMAN}.exe"

  ;Default installation folder
  # InstallDir "$PROGRAMFILES\Kushview\Roboverb"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Kushviw\Roboverb" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings
  !define MUI_ABORTWARNING
  !define MUI_ICON "..\inno\KushviewIcon.ico"
  ;!define MUI_HEADERIMAGE
  ;!define MUI_HEADERIMAGE_BITMAP "path\to\InstallerLogo.bmp"
  ;!define MUI_HEADERIMAGE_RIGHT

;--------------------------------
;Pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "../../../LICENSE.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections
Section "Roboverb" SecDummy
  SetOutPath "$INSTDIR"
  ; File "${STANDALONE_FILE}"

  var /GLOBAL BasePath
  ${If} ${RunningX64}
    ${If} ${ARCH} == "x64"
      StrCpy $BasePath $ProgramFiles64
    ${Else}
      StrCpy $BasePath $ProgramFiles32
    ${Endif}
  ${Else}
    StrCpy $BasePath $ProgramFiles
  ${Endif}

  SetOutPath "$BasePath\VstPlugins"
  File "${VST2_FILE}"

  SetOutPath "$BasePath\Common Files\VST3"
  File "${VST3_FILE}"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

;--------------------------------
;Uninstaller
Section "Uninstall"
  SetShellVarContext current
  ; Delete "$INSTDIR\Roboverb.exe"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir  "$INSTDIR"

  ${If} ${RunningX64}
    ${If} ${ARCH} == "x64"
      Delete "$ProgramFiles64\Common Files\VST3\Roboverb.*"
      Delete "$ProgramFiles64\VstPlugins\Roboverb.*"
    ${Else}
      Delete "$ProgramFiles32\Common Files\VST3\Roboverb.*"
      Delete "$ProgramFiles32\VstPlugins\Roboverb.*"
    ${Endif}
  ${Else}
      Delete "$ProgramFiles\Common Files\VST3\Roboverb.*"
      Delete "$ProgramFiles\VstPlugins\Roboverb.*"
  ${Endif}
SectionEnd

;Language strings
LangString DESC_SecDummy ${LANG_ENGLISH} "Roboverb standalone application"

;Assign language strings to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
