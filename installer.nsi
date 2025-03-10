; Engine Installer Script
!include "MUI2.nsh"
!include "FileFunc.nsh"

; General
Name "3D Engine - Prototype 0.0.1"
OutFile "3DEngine-0.0.1-Setup.exe"
InstallDir "$PROGRAMFILES\3DEngine"
InstallDirRegKey HKCU "Software\3DEngine" ""

; Interface Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\win.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\win.bmp"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "LICENSE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

; Version Information
VIProductVersion "0.0.1.0"
VIAddVersionKey "ProductName" "3D Engine"
VIAddVersionKey "Comments" "Modern OpenGL-based 3D Engine"
VIAddVersionKey "CompanyName" "3D Engine"
VIAddVersionKey "FileVersion" "0.0.1"
VIAddVersionKey "ProductVersion" "0.0.1"
VIAddVersionKey "LegalCopyright" "© 2024 3D Engine"
VIAddVersionKey "FileDescription" "3D Engine Installer"

Section "Core Files (required)" SEC01
    SectionIn RO
    SetOutPath "$INSTDIR"
    SetOverwrite ifnewer
    
    ; Create directories
    CreateDirectory "$INSTDIR\bin"
    CreateDirectory "$INSTDIR\SHADERS"
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
    
    ; Copy version info
    SetOutPath "$INSTDIR"
    File "dist\version.txt"
    
    ; Create shortcuts with working directory set to bin
    CreateDirectory "$SMPROGRAMS\3DEngine"
    SetOutPath "$INSTDIR\bin"
    CreateShortCut "$SMPROGRAMS\3DEngine\3D Engine.lnk" "$INSTDIR\bin\OpenGLRenderer.exe"
    CreateShortCut "$DESKTOP\3D Engine.lnk" "$INSTDIR\bin\OpenGLRenderer.exe"
SectionEnd

Section "Documentation" SEC02
    SetOutPath "$INSTDIR\docs"
    File "dist\docs\README.md"
    File "dist\docs\LICENSE"
    File "dist\docs\RELEASE_NOTES.md"
    
    CreateShortCut "$SMPROGRAMS\3DEngine\Documentation.lnk" "$INSTDIR\docs\RELEASE_NOTES.md"
SectionEnd

Section "Screenshots" SEC03
    SetOutPath "$INSTDIR\screenshots"
    File "dist\screenshots\*.png"
SectionEnd

Section -Post
    SetOutPath "$INSTDIR"
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    ; Write registry keys
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "DisplayName" "3D Engine"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "Publisher" "3D Engine"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "DisplayIcon" "$INSTDIR\bin\OpenGLRenderer.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "DisplayVersion" "0.0.1"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "VersionMajor" "0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine" "VersionMinor" "0"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Core engine files, including executable, shaders, and required DLLs."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Documentation files including README, LICENSE, and Release Notes."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} "Screenshot examples of the engine in action."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"
    ; Remove symbolic link first
    RMDir "$INSTDIR\bin\SHADERS"
    
    ; Remove directories and files
    RMDir /r "$INSTDIR\bin"
    RMDir /r "$INSTDIR\SHADERS"
    RMDir /r "$INSTDIR\config"
    RMDir /r "$INSTDIR\docs"
    RMDir /r "$INSTDIR\screenshots"
    Delete "$INSTDIR\version.txt"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir "$INSTDIR"
    
    ; Remove shortcuts
    Delete "$SMPROGRAMS\3DEngine\3D Engine.lnk"
    Delete "$SMPROGRAMS\3DEngine\Documentation.lnk"
    RMDir "$SMPROGRAMS\3DEngine"
    Delete "$DESKTOP\3D Engine.lnk"
    
    ; Remove registry keys
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DEngine"
    DeleteRegKey HKCU "Software\3DEngine"
SectionEnd 