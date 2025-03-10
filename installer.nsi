; Engine Installer Script
!include "MUI2.nsh"
!include "FileFunc.nsh"

; General
Name "3D Engine"
OutFile "EngineSetup.exe"
InstallDir "$PROGRAMFILES\3DEngine"
InstallDirRegKey HKCU "Software\3DEngine" ""

; Interface Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "LICENSE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

Section "MainSection" SEC01
    SetOutPath "$INSTDIR"
    SetOverwrite ifnewer
    
    ; Create directories
    CreateDirectory "$INSTDIR\bin"
    CreateDirectory "$INSTDIR\SHADERS"
    CreateDirectory "$INSTDIR\resources"
    CreateDirectory "$INSTDIR\config"
    
    ; Copy executable and DLLs
    SetOutPath "$INSTDIR\bin"
    File "dist\bin\OpenGLRenderer.exe"
    File "dist\bin\*.dll"
    
    ; Copy shader files
    SetOutPath "$INSTDIR\SHADERS"
    File "dist\SHADERS\*.vert"
    File "dist\SHADERS\*.frag"
    
    ; Create symbolic link for SHADERS in bin directory
    ExecWait 'cmd /c mklink /D "$INSTDIR\bin\SHADERS" "..\SHADERS"'
    
    ; Copy config files
    SetOutPath "$INSTDIR\config"
    File /nonfatal "dist\config\*.ini"
    
    ; Copy documentation
    SetOutPath "$INSTDIR"
    File "dist\README.md"
    File "dist\LICENSE"
    
    ; Create shortcuts with working directory set to bin
    CreateDirectory "$SMPROGRAMS\3DEngine"
    SetOutPath "$INSTDIR\bin"
    CreateShortCut "$SMPROGRAMS\3DEngine\3DEngine.lnk" "$INSTDIR\bin\OpenGLRenderer.exe"
    CreateShortCut "$DESKTOP\3DEngine.lnk" "$INSTDIR\bin\OpenGLRenderer.exe"
    
    ; Write uninstaller
    SetOutPath "$INSTDIR"
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    ; Write registry keys
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "DisplayName" "3D Engine"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "Publisher" "3D Engine"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "DisplayIcon" "$INSTDIR\bin\OpenGLRenderer.exe"
SectionEnd

Section "Uninstall"
    ; Remove symbolic link first
    RMDir "$INSTDIR\bin\SHADERS"
    
    ; Remove directories and files
    RMDir /r "$INSTDIR\bin"
    RMDir /r "$INSTDIR\SHADERS"
    RMDir /r "$INSTDIR\resources"
    RMDir /r "$INSTDIR\config"
    Delete "$INSTDIR\README.md"
    Delete "$INSTDIR\LICENSE"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir "$INSTDIR"
    
    ; Remove shortcuts
    Delete "$SMPROGRAMS\3DEngine\3DEngine.lnk"
    RMDir "$SMPROGRAMS\3DEngine"
    Delete "$DESKTOP\3DEngine.lnk"
    
    ; Remove registry keys
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine"
    DeleteRegKey HKCU "Software\3DEngine"
SectionEnd 