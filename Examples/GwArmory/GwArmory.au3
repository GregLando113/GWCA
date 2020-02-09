#RequireAdmin

#AutoIt3Wrapper_Icon=..\resources\gwtoolbox.ico
#AutoIt3Wrapper_Res_Description=GwArmory Launcher
#AutoIt3Wrapper_Res_Fileversion=1.2.0.0
Global Const $laucher_version = 1.2 ; only shown in log. Increase after bugfixes to ease debugging of user issues.
Global Const $dll_name = 'GwArmory'
Global Const $skip_runtime_checks = false
#cs
Changelog:
1.0
- initial release

1.1
- included StringSize.au3
- added more log strings related to architecture and runtime checks
- added flag to optionally skip runtime checks
- added message box to optionally set the above flag

1.2
- fixed a bug with client selection
- added more debug strings related to client selection

#ce

#include <File.au3>
#include <ComboConstants.au3>
#include <FileConstants.au3>
#include <MsgBoxConstants.au3>
#include <GUIConstantsEx.au3>
#include <GuiComboBox.au3>
#include <WinAPIProc.au3>
#include <WinAPISys.au3>
#include <InetConstants.au3>
#include <GUIConstantsEx.au3>
#include <ProgressConstants.au3>
#include <StaticConstants.au3>
#include <WindowsConstants.au3>

; dx runtime: http://www.microsoft.com/en-us/download/details.aspx?id=8109

Opt("MustDeclareVars", True)
Opt("GuiResizeMode", BitOR($GUI_DOCKSIZE, $GUI_DOCKTOP, $GUI_DOCKLEFT))

; ==== Globals ====
Global $folder = @ScriptDir & '\'
Global $dllpath = $folder & $dll_name & '.dll'
Global $logstring = ""
Func FlushLog()
	Out("Flushing log, bye!")
	Local Const $file = FileOpen($folder & "launcher_log.txt", $FO_OVERWRITE)
	FileWrite($file, $logstring)
	FileClose($file)
EndFunc
Out("Launcher version: "&$laucher_version)
OnAutoItExitRegister("FlushLog")

; ==== Disclaimer ====
If Not FileExists($dllpath) Then
   Out("Showing disclaimer")
	If MsgBox($MB_OKCANCEL, $dll_name, 'DISCLAIMER:'&@CRLF& _
	'THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF ' & _
	'FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY ' & _
	'DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.' & @CRLF&@CRLF & _
	'By clicking the OK button you agree with all the above.') <> $IDOK Then Exit
EndIf

; ==== Install Visual Studio C++ 2015 Runtime ====
Func CheckVCRedist()
	Out("Checking visual studio redistributable")
	Local $Key
	If @CPUArch == "X64" Then
		Out("x64")
		$Key = RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x86", "Installed")
	Else
		Out("x86")
		$Key = RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86", "Installed")
	EndIf
	Out("key is " & $Key)
	If $Key == 1 Then Return False; Already installed

	If MsgBox($MB_YESNO, $dll_name & " Launcher", "You do not have The Visual C++ Runtime (vc_redist.x86) required for " & $dll_name & " to run. Would you like me to open where to download it?") == $IDYES Then
		Out("Launching vs redistr website")
		ShellExecute('https://www.microsoft.com/en-us/download/details.aspx?id=48145')
  EndIf
	Return True
EndFunc

; ==== Install DirectX 9.0c Runtime ====
Func CheckDirectX()
	Out("Checking visual studio redistributable")
	Local $Key
	If @CPUArch == "X64" Then
		Out("x64")
		$Key = RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\DirectX", "Version")
	Else
		Out("x86")
		$Key = RegRead("HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\DirectX", "Version")
	EndIf
	Out("key is " & $Key)
	If $Key == '4.09.00.0904' Then Return False; Already installed

	If MsgBox($MB_YESNO, $dll_name & " Launcher", "You do not have The DirectX 9.0c Runtime required for " & $dll_name & " to run. Would you like me to open where to download it?") == $IDYES Then
		Out("Launching directx website")
		ShellExecute('https://www.microsoft.com/en-us/download/details.aspx?id=35')
	EndIf

	Return True
EndFunc

If $skip_runtime_checks Then
	Out("Skipping runtime checks")
	Else
	If CheckVCRedist() Or CheckDirectX() Then
		MsgBox(0, $dll_name & " Launcher", "Please install the runtimes you were prompted to install, then run this launcher again.")
		Exit
	EndIf
EndIf

; ==== Create directories ====
Out("Creating directories and reading .ini... ")
If Not FileExists($folder) Then DirCreate($folder)
If Not FileExists($folder) Then Exit MsgBox($MB_ICONERROR, $dll_name & " Launcher Error", $dll_name & " Launcher was unable to create the folder '" & $folder & "'." & @CRLF & _
	"Make sure you have admin rights and your antivirus is not blocking " & $dll_name &". Quitting.")

Out("dll path: "&$dllpath)

Global $gui = 0, $label = 0, $progressbar = 0, $progresslabel = 0, $changelabel = 0, $height = 0

; === Last checks before using the dll ===
; this is in case the user moved the gwtoolbox.dll to another location, for example, while playing around with gwlauncher plugins, or simply renaming gwtoolbox.dll to whydopeopledothis.dll
If Not FileExists($dllpath) Then
	Local Const $msgbutton = MsgBox(BitOR($MB_ICONWARNING, $MB_OKCANCEL), $dll_name, "Error: Cannot find "& $dllpath & ".dll."&@CRLF&"Please help me find "& $dll_name& ".dll")
	If ($msgbutton <> $IDOK) Then
		Out("User did not want to help me find a dll :(")
		Exit
	EndIf
	$dllpath = FileOpenDialog("Please locate " & $dll_name& ".dll", $folder, "Dll (*.dll)", $FD_FILEMUSTEXIST + $FD_PATHMUSTEXIST)
	If @error Then
		Out("User did not help me find the dll :(")
		Exit
	EndIf
EndIf

; again?!?
If Not FileExists($dllpath) Then Error("Cannot find " & $dll_name& " dll in "&$dllpath)

If FileGetSize($dllpath) == 0 Then Error($dll_name& " dll in "&$dllpath&" is an empty file!")

; === Client selection ===
Global $mKernelHandle, $mGWProcHandle, $mGWProcId
Global $lWinList = WinList("[CLASS:ArenaNet_Dx_Window_Class]")
Global $gwPID
Switch $lWinList[0][0]
	Case 0
		Error("Guild Wars not running")
	Case 1
		$gwPID = WinGetProcess($lWinList[1][1])
		Out("One copy of GW detected. PID: " & $gwPID)
	Case Else
		Out("Multiple copies of GW detected")
		Local $lCharArray[$lWinList[0][0]]
		Local $lFirstChar
		Local $lComboStr
		Local $lCharNameRva
		For $i = 1 To $lWinList[0][0]
			Local $lPID = WinGetProcess($lWinList[$i][1])
			MemoryOpen($lPID)
			Local $lModuleBase, $lModuleSize
			GetModuleInfo($lModuleBase, $lModuleSize)
			If $i = 1 Then
				$lCharNameRva = ScanForCharname($lModuleBase, $lModuleSize)
				$lFirstChar = MemoryRead($lModuleBase + $lCharNameRva, 'wchar[30]')
				$lCharArray[$i - 1] = $lFirstChar
			Else
				$lCharArray[$i - 1] = MemoryRead($lModuleBase + $lCharNameRva, 'wchar[30]')
			EndIf
			MemoryClose()

			$lComboStr &= $lCharArray[$i - 1]
			If $i <> $lWinList[0][0] Then $lComboStr &= '|'
		Next
		Local Const $selection_gui = GUICreate("GWToolbox++", 150, 60)
		Local Const $comboCharname = GUICtrlCreateCombo("", 6, 6, 138, 25, $CBS_DROPDOWNLIST)
		GUICtrlSetData(-1, $lComboStr, $lFirstChar)
		Local Const $launchbutton = GUICtrlCreateButton("Launch", 6, 31, 138, 24)
		GUISetState(@SW_SHOW, $selection_gui)
		Local $index
		While True
			Switch GUIGetMsg()
				Case $launchbutton
					$index = _GUICtrlComboBox_GetCurSel($comboCharname) ; this needs to be before GUIDelete
					GUIDelete($selection_gui)
					ExitLoop
				Case $GUI_EVENT_CLOSE
					Out("Exiting from client selection gui")
					Exit
			EndSwitch
			Sleep(10)
		WEnd

		$gwPID = WinGetProcess($lWinList[$index + 1][1])
		Out("Client (" & $index & ") selected. PID: " & $gwPID)
EndSwitch

If Not ProcessExists($gwPID) Then Error("Cannot find Guild Wars process")

; === Check for DEP (Data Execution prevention) ===
Local $dep_status = _WinAPI_GetSystemDEPPolicy()
if ($dep_status == 1) Then Error("You have DEP (Data Execution Prevention) enabled for all applications, GWToolbox will not work unless you disable it or whitelist gw.exe")

; === Injection ===
Local $Kernel32 = DllOpen("kernel32.dll")
If $Kernel32 == -1 Then Error("Failed to open kernel32.dll")

Local $DLL_Path = DllStructCreate("char[255]")
Local $getfullpath_ret = DllCall($Kernel32, "DWORD", "GetFullPathNameA", "str", $dllpath, "DWORD", 255, "ptr", DllStructGetPtr($DLL_Path), "int", 0)
If @error Then Error1("Failed to DllCall GetFullPathNameA", @error)
If $getfullpath_ret == 0 Then Error2("Failed to call 'GetFullPathNameA'", _WinAPI_GetLastError(), $getfullpath_ret)
Local $sDLLFullPath = DllStructGetData($DLL_Path,1)

Local $modules = _WinAPI_EnumProcessModules($gwPID)
If @error Then Error2("Failed to call 'EnumProcessModules' (1)", @error, $modules)
For $i = 1 To $modules[0][0]
	If $modules[$i][1] == $dllpath Then Error("Already in specified process")
Next

Local $hProcess = DllCall($Kernel32, "DWORD", "OpenProcess", "DWORD", 0x1F0FFF, "int", 0, "DWORD", $gwPID)[0]
If @error Then Error1("Failed to DllCall OpenProcess", @error)
If $hProcess == 0 Then Error2("OpenProcess failed", _WinAPI_GetLastError(), $hProcess)

Local $hModule = DllCall($Kernel32, "DWORD", "GetModuleHandleA", "str", "kernel32.dll")[0]
If @error Then Error1("Failed to DllCall GetModuleHandleA", @error)
If $hModule == 0 Then Error2("GetModuleHandle failed", _WinAPI_GetLastError(), $hModule)

Local $lpStartAddress = DllCall($Kernel32, "DWORD", "GetProcAddress", "DWORD", $hModule, "str", "LoadLibraryA")[0]
If @error Then Error1("Failed to DllCall GetProcAddress", @error)
If $lpStartAddress == 0 Then Error2("GetProcAddress failed", _WinAPI_GetLastError(), $lpStartAddress)

Local $lpParameter = DllCall($Kernel32, "DWORD", "VirtualAllocEx", "int", $hProcess, "int", 0, "ULONG_PTR", DllStructGetSize($DLL_Path), "DWORD", 0x3000, "int", 4)[0]
If @error Then Error1("Failed to DllCall VirtualAllocEx", @error)
If $lpParameter == 0 Then Error2("VirtualAllocEx failed", _WinAPI_GetLastError(), $lpParameter)

Local $writeProcMem_ret = DllCall($Kernel32, "BOOL", "WriteProcessMemory", "int", $hProcess, "DWORD", $lpParameter, "str", $sDLLFullPath, "ULONG_PTR", DllStructGetSize($DLL_Path), "int", 0)[0]
If @error Then Error1("Failed to DllCall WriteProcessMemory", @error)
If $writeProcMem_ret == 0 Then Error2("WriteProcessMemory failed", _WinAPI_GetLastError(), $writeProcMem_ret)

Local $hThread = DllCall($Kernel32, "int", "CreateRemoteThread", "DWORD", $hProcess, "int", 0, "int", 0, "DWORD", $lpStartAddress, "DWORD", $lpParameter, "int", 0, "int", 0)[0]
If @error Then Error1("Failed to DllCall CreateRemoteThread", @error)
If $hThread == 0 Then Error2("CreateRemoteThread failed", _WinAPI_GetLastError(), $hThread)

Local $nWaitEvent = DllCall($Kernel32, "DWORD", "WaitForSingleObject", "HANDLE", $hThread, "DWORD", 1000)[0]
If @error Then Error1("Failed to DllCall WaitForSingleObject", @error)
If $nWaitEvent <> 0 Then Error2("WaitForSingleObject timed out", _WinAPI_GetLastError(), Hex($nWaitEvent))

Local $nExitCode = 0
Local $hTBModule = DllCall($Kernel32, "HANDLE", "GetExitCodeThread", "HANDLE", $hThread, "DWORD*", $nExitCode)[0]
If @error Then Error1("Failed to DllCall GetExitCodeThread", @error)
If $hTBModule == 0 Then Error2("GetExitCodeThread failed", _WinAPI_GetLastError(), $nExitCode)

Local $closehandle_ret = DllCall($Kernel32, "BOOL", "CloseHandle", "DWORD", $hProcess)[0]
If @error Then Error1("Failed to DllCall CloseHandle", @error)
If $closehandle_ret == 0 Then Error2("CloseHandle failed", _WinAPI_GetLastError(), $closehandle_ret)

Local $found = False
Local $deadlock = TimerInit()
While Not $found And TimerDiff($deadlock) < 4000

	$modules = _WinAPI_EnumProcessModules($gwPID)
	If @error Then Error2("Failed to call 'EnumProcessModules' (2)", @error, $modules)

	For $i = 1 To $modules[0][0]
		If $modules[$i][1] == $dllpath Then
			$found = True
			ExitLoop
		EndIf
	Next
	Sleep(200)
WEnd

If Not $found Then
	If ($dep_status == 3) Then
		Error("dll was not loaded, unknown error" & @CRLF & "Note: you have DEP (Data Execution Prevention) enabled, make sure to whitelist gw.exe")
	Else
		Error("dll was not loaded, unknown error "& $dllpath)
	EndIf
EndIf

If $gui > 0 Then
	GUICtrlSetData($progresslabel, GUICtrlRead($progresslabel) & ". You can close this window.")

	While GUIGetMsg() <> $GUI_EVENT_CLOSE
		Sleep(0)
	WEnd
EndIf

#Region error message boxes
Func Out($msg)
	ConsoleWrite($msg & @CRLF)
	$logstring &= ($msg & @CRLF)
EndFunc
Func Warning($msg)
	Out("Warning: "&$msg)
	MsgBox($MB_ICONWARNING, $dll_name, "Warning: " & $msg)
EndFunc
Func Error($msg)
	Out("Error: "&$msg)
	MsgBox($MB_ICONERROR, $dll_name, "Error: " & $msg)
	Exit
EndFunc
Func Error1($msg, $err)
	Out("Error: "&$msg&" code: "&$err)
	MsgBox($MB_ICONERROR, $dll_name, "Error: " & $msg & @CRLF & "Error code: " & $err)
	Exit
EndFunc
Func Error2($msg, $err, $ret)
	Out("Error: "&$msg&" code: "&$err&" return: "&$ret)
	MsgBox($MB_ICONERROR, $dll_name,"Error: " & $msg & @CRLF & "Error code: " & $err & @CRLF & "Return value: " & $ret)
	Exit
EndFunc
#EndRegion

#Region memory managment and char name read
Func MemoryOpen($aPID)
	$mKernelHandle = DllOpen('kernel32.dll')
	Local $lOpenProcess = DllCall($mKernelHandle, 'int', 'OpenProcess', 'int', 0x1F0FFF, 'int', 1, 'int', $aPID)
	$mGWProcHandle = $lOpenProcess[0]
	$mGWProcId = $aPID
EndFunc   ;==>MemoryOpen

;~ Description: Internal use only.
Func MemoryClose()
	DllCall($mKernelHandle, 'int', 'CloseHandle', 'int', $mGWProcHandle)
EndFunc   ;==>MemoryClose

;~ Description: Internal use only.
Func MemoryRead($aAddress, $aType = 'dword')
	Local $lBuffer = DllStructCreate($aType)
	DllCall($mKernelHandle, 'int', 'ReadProcessMemory', 'int', $mGWProcHandle, 'int', $aAddress, 'ptr', DllStructGetPtr($lBuffer), 'int', DllStructGetSize($lBuffer), 'int', '')
	Return DllStructGetData($lBuffer, 1)
EndFunc   ;==>MemoryRead

Func GetModuleInfo(ByRef $aModuleBase, ByRef $aModuleSize)
	Local $lProcName = _WinAPI_GetProcessName($mGWProcId)
	Local $lProcNameLength = StringLen($lProcName)

	Local $lModules = _WinAPI_EnumProcessModules($mGWProcId)
	For $i = 1 To $lModules[0][0]
		Local $lModuleName = StringRight($lModules[$i][1], $lProcNameLength)
		If StringCompare($lModuleName, $lProcName, $STR_NOCASESENSE) = 0 Then
			Local $lModuleInfo = _WinAPI_GetModuleInformation($mGWProcHandle, $lModules[$i][0])
			$aModuleBase = DllStructGetData($lModuleInfo, 'BaseOfDll')
			$aModuleSize = DllStructGetData($lModuleInfo, 'SizeOfImage')
			Return True
		EndIf
	Next
	Return False
EndFunc   ;==>GetModuleInfo

Func ScanForCharname($aModuleBase, $aModuleSize)
	Local $lCharNameCode = BinaryToString('0x8BF86A03680F0000C08BCFE8')
	Local $lCurrentSearchAddress = $aModuleBase
	Local $lMBI[7], $lMBIBuffer = DllStructCreate('dword;dword;dword;dword;dword;dword;dword')
	Local $lSearch, $lTmpMemData, $lTmpAddress, $lTmpBuffer = DllStructCreate('ptr'), $i

	While $lCurrentSearchAddress < $aModuleBase + $aModuleSize
		Local $lMBI[7]
		DllCall($mKernelHandle, 'int', 'VirtualQueryEx', 'int', $mGWProcHandle, 'int', $lCurrentSearchAddress, 'ptr', DllStructGetPtr($lMBIBuffer), 'int', DllStructGetSize($lMBIBuffer))
		For $i = 0 To 6
			$lMBI[$i] = StringStripWS(DllStructGetData($lMBIBuffer, ($i + 1)), 3)
		Next

		If $lMBI[4] = 4096 Then
			Local $lBuffer = DllStructCreate('byte[' & $lMBI[3] & ']')
			DllCall($mKernelHandle, 'int', 'ReadProcessMemory', 'int', $mGWProcHandle, 'int', $lCurrentSearchAddress, 'ptr', DllStructGetPtr($lBuffer), 'int', DllStructGetSize($lBuffer), 'int', '')

			$lTmpMemData = DllStructGetData($lBuffer, 1)
			$lTmpMemData = BinaryToString($lTmpMemData)

			$lSearch = StringInStr($lTmpMemData, $lCharNameCode, 2)
			If $lSearch > 0 Then
				$lTmpAddress = $lCurrentSearchAddress + $lSearch - 1
				DllCall($mKernelHandle, 'int', 'ReadProcessMemory', 'int', $mGWProcHandle, 'int', $lTmpAddress - 0x42, 'ptr', DllStructGetPtr($lTmpBuffer), 'int', DllStructGetSize($lTmpBuffer), 'int', '')
				Return DllStructGetData($lTmpBuffer, 1) - $aModuleBase
			EndIf

			$lCurrentSearchAddress += $lMBI[3]
		EndIf
	WEnd

	Return False
EndFunc   ;==>ScanForCharname
#EndRegion

; #INDEX# ============================================================================================================
; Title .........: _StringSize
; AutoIt Version : v3.2.12.1 or higher
; Language ......: English
; Description ...: Returns size of rectangle required to display string - maximum width can be chosen
; Remarks .......:
; Note ..........:
; Author(s) .....:  Melba23 - thanks to trancexx for the default DC code
; ====================================================================================================================

;#AutoIt3Wrapper_Au3Check_Parameters=-d -w 1 -w 2 -w 3 -w 4 -w 5 -w 6

; #CURRENT# ==========================================================================================================
; _StringSize: Returns size of rectangle required to display string - maximum width can be chosen
; ====================================================================================================================

; #INTERNAL_USE_ONLY#=================================================================================================
; _StringSize_Error_Close: Releases DC and deletes font object after error
; _StringSize_DefaultFontName: Determines Windows default font
; ====================================================================================================================

; #FUNCTION# =========================================================================================================
; Name...........: _StringSize
; Description ...: Returns size of rectangle required to display string - maximum permitted width can be chosen
; Syntax ........: _StringSize($sText[, $iSize[, $iWeight[, $iAttrib[, $sName[, $iWidth[, $hWnd]]]]]])
; Parameters ....: $sText   - String to display
;                  $iSize   - [optional] Font size in points - (default = 8.5)
;                  $iWeight - [optional] Font weight - (default = 400 = normal)
;                  $iAttrib - [optional] Font attribute (0-Normal (default), 2-Italic, 4-Underline, 8 Strike)
;                             + 1 if tabs are to be expanded before sizing
;                  $sName   - [optional] Font name - (default = Tahoma)
;                  $iWidth  - [optional] Max width for rectangle - (default = 0 => width of original string)
;                  $hWnd    - [optional] GUI in which string will be displayed - (default 0 => normally not required)
; Requirement(s) : v3.2.12.1 or higher
; Return values .: Success - Returns 4-element array: ($iWidth set // $iWidth not set)
;                  |$array[0] = String reformatted with additonal @CRLF // Original string
;                  |$array[1] = Height of single line in selected font // idem
;                  |$array[2] = Width of rectangle required for reformatted // original string
;                  |$array[3] = Height of rectangle required for reformatted // original string
;                  Failure - Returns 0 and sets @error:
;                  |1 - Incorrect parameter type (@extended = parameter index)
;                  |2 - DLL call error - extended set as follows:
;                       |1 - GetDC failure
;                       |2 - SendMessage failure
;                       |3 - GetDeviceCaps failure
;                       |4 - CreateFont failure
;                       |5 - SelectObject failure
;                       |6 - GetTextExtentPoint32 failure
;                  |3 - Font too large for chosen max width - a word will not fit
; Author ........: Melba23 - thanks to trancexx for the default DC code
; Modified ......:
; Remarks .......: The use of the $hWnd parameter is not normally necessary - it is only required if the UDF does not
;                   return correct dimensions without it.
; Related .......:
; Link ..........:
; Example .......: Yes
;=====================================================================================================================
Func _StringSize($sText, $iSize = 8.5, $iWeight = 400, $iAttrib = 0, $sName = "", $iMaxWidth = 0, $hWnd = 0)

	; Set parameters passed as Default
	If $iSize = Default Then $iSize = 8.5
	If $iWeight = Default Then $iWeight = 400
	If $iAttrib = Default Then $iAttrib = 0
	If $sName = "" Or $sName = Default Then $sName = _StringSize_DefaultFontName()

	; Check parameters are correct type
	If Not IsString($sText) Then Return SetError(1, 1, 0)
	If Not IsNumber($iSize) Then Return SetError(1, 2, 0)
	If Not IsInt($iWeight) Then Return SetError(1, 3, 0)
	If Not IsInt($iAttrib) Then Return SetError(1, 4, 0)
	If Not IsString($sName) Then Return SetError(1, 5, 0)
	If Not IsNumber($iMaxWidth) Then Return SetError(1, 6, 0)
	If Not IsHwnd($hWnd) And $hWnd <> 0 Then Return SetError(1, 7, 0)

	Local $aRet, $hDC, $hFont, $hLabel = 0, $hLabel_Handle

	; Check for tab expansion flag
	Local $iExpTab = BitAnd($iAttrib, 1)
	; Remove possible tab expansion flag from font attribute value
	$iAttrib = BitAnd($iAttrib, BitNot(1))

	; If GUI handle was passed
	If IsHWnd($hWnd) Then
		; Create label outside GUI borders
		$hLabel = GUICtrlCreateLabel("", -10, -10, 10, 10)
		$hLabel_Handle = GUICtrlGetHandle(-1)
		GUICtrlSetFont(-1, $iSize, $iWeight, $iAttrib, $sName)
		; Create DC
		$aRet = DllCall("user32.dll", "handle", "GetDC", "hwnd", $hLabel_Handle)
		If @error Or $aRet[0] = 0 Then
			GUICtrlDelete($hLabel)
			Return SetError(2, 1, 0)
		EndIf
		$hDC = $aRet[0]
		$aRet = DllCall("user32.dll", "lparam", "SendMessage", "hwnd", $hLabel_Handle, "int", 0x0031, "wparam", 0, "lparam", 0) ; $WM_GetFont
		If @error Or $aRet[0] = 0 Then
			GUICtrlDelete($hLabel)
			Return SetError(2, _StringSize_Error_Close(2, $hDC), 0)
		EndIf
		$hFont = $aRet[0]
	Else
		; Get default DC
		$aRet = DllCall("user32.dll", "handle", "GetDC", "hwnd", $hWnd)
		If @error Or $aRet[0] = 0 Then Return SetError(2, 1, 0)
		$hDC = $aRet[0]
		; Create required font
		$aRet = DllCall("gdi32.dll", "int", "GetDeviceCaps", "handle", $hDC, "int", 90) ; $LOGPIXELSY
		If @error Or $aRet[0] = 0 Then Return SetError(2, _StringSize_Error_Close(3, $hDC), 0)
		Local $iInfo = $aRet[0]
		$aRet = DllCall("gdi32.dll", "handle", "CreateFontW", "int", -$iInfo * $iSize / 72, "int", 0, "int", 0, "int", 0, _
			"int", $iWeight, "dword", BitAND($iAttrib, 2), "dword", BitAND($iAttrib, 4), "dword", BitAND($iAttrib, 8), "dword", 0, "dword", 0, _
			"dword", 0, "dword", 5, "dword", 0, "wstr", $sName)
		If @error Or $aRet[0] = 0 Then Return SetError(2, _StringSize_Error_Close(4, $hDC), 0)
		$hFont = $aRet[0]
	EndIf

	; Select font and store previous font
	$aRet = DllCall("gdi32.dll", "handle", "SelectObject", "handle", $hDC, "handle", $hFont)
	If @error Or $aRet[0] = 0 Then Return SetError(2, _StringSize_Error_Close(5, $hDC, $hFont, $hLabel), 0)
	Local $hPrevFont = $aRet[0]

	; Declare variables
	Local $avSize_Info[4], $iLine_Length, $iLine_Height = 0, $iLine_Count = 0, $iLine_Width = 0, $iWrap_Count, $iLast_Word, $sTest_Line
	; Declare and fill Size structure
	Local $tSize = DllStructCreate("int X;int Y")
	DllStructSetData($tSize, "X", 0)
	DllStructSetData($tSize, "Y", 0)

	; Ensure EoL is @CRLF and break text into lines
	$sText = StringRegExpReplace($sText, "((?<!\x0d)\x0a|\x0d(?!\x0a))", @CRLF)
	Local $asLines = StringSplit($sText, @CRLF, 1)

	; For each line
	For $i = 1 To $asLines[0]
		; Expand tabs if required
		If $iExpTab Then
			$asLines[$i] = StringReplace($asLines[$i], @TAB, " XXXXXXXX")
		EndIf
		; Size line
		$iLine_Length = StringLen($asLines[$i])
		DllCall("gdi32.dll", "bool", "GetTextExtentPoint32W", "handle", $hDC, "wstr", $asLines[$i], "int", $iLine_Length, "ptr", DllStructGetPtr($tSize))
		If @error Then Return SetError(2, _StringSize_Error_Close(6, $hDC, $hFont, $hLabel), 0)
		If DllStructGetData($tSize, "X") > $iLine_Width Then $iLine_Width = DllStructGetData($tSize, "X")
		If DllStructGetData($tSize, "Y") > $iLine_Height Then $iLine_Height = DllStructGetData($tSize, "Y")
	Next

	; Check if $iMaxWidth has been both set and exceeded
	If $iMaxWidth <> 0 And $iLine_Width > $iMaxWidth Then ; Wrapping required
		; For each Line
		For $j = 1 To $asLines[0]
			; Size line unwrapped
			$iLine_Length = StringLen($asLines[$j])
			DllCall("gdi32.dll", "bool", "GetTextExtentPoint32W", "handle", $hDC, "wstr", $asLines[$j], "int", $iLine_Length, "ptr", DllStructGetPtr($tSize))
			If @error Then Return SetError(2, _StringSize_Error_Close(6, $hDC, $hFont, $hLabel), 0)
			; Check wrap status
			If DllStructGetData($tSize, "X") < $iMaxWidth - 4 Then
				; No wrap needed so count line and store
				$iLine_Count += 1
				$avSize_Info[0] &= $asLines[$j] & @CRLF
			Else
				; Wrap needed so zero counter for wrapped lines
				$iWrap_Count = 0
				; Build line to max width
				While 1
					; Zero line width
					$iLine_Width = 0
					; Initialise pointer for end of word
					$iLast_Word = 0
					; Add characters until EOL or maximum width reached
					For $i = 1 To StringLen($asLines[$j])
						; Is this just past a word ending?
						If StringMid($asLines[$j], $i, 1) = " " Then $iLast_Word = $i - 1
						; Increase line by one character
						$sTest_Line = StringMid($asLines[$j], 1, $i)
						; Get line length
						$iLine_Length = StringLen($sTest_Line)
						DllCall("gdi32.dll", "bool", "GetTextExtentPoint32W", "handle", $hDC, "wstr", $sTest_Line, "int", $iLine_Length, "ptr", DllStructGetPtr($tSize))
						If @error Then Return SetError(2, _StringSize_Error_Close(6, $hDC, $hFont, $hLabel), 0)
						$iLine_Width = DllStructGetData($tSize, "X")
						; If too long exit the loop
						If $iLine_Width >= $iMaxWidth - 4 Then ExitLoop
					Next
					; End of the line of text?
					If $i > StringLen($asLines[$j]) Then
						; Yes, so add final line to count
						$iWrap_Count += 1
						; Store line
						$avSize_Info[0] &= $sTest_Line & @CRLF
						ExitLoop
					Else
						; No, but add line just completed to count
						$iWrap_Count += 1
						; Check at least 1 word completed or return error
						If $iLast_Word = 0 Then Return SetError(3, _StringSize_Error_Close(0, $hDC, $hFont, $hLabel), 0)
						; Store line up to end of last word
						$avSize_Info[0] &= StringLeft($sTest_Line, $iLast_Word) & @CRLF
						; Strip string to point reached
						$asLines[$j] = StringTrimLeft($asLines[$j], $iLast_Word)
						; Trim leading whitespace
						$asLines[$j] = StringStripWS($asLines[$j], 1)
						; Repeat with remaining characters in line
					EndIf
				WEnd
				; Add the number of wrapped lines to the count
				$iLine_Count += $iWrap_Count
			EndIf
		Next
		; Reset any tab expansions
		If $iExpTab Then
			$avSize_Info[0] = StringRegExpReplace($avSize_Info[0], "\x20?XXXXXXXX", @TAB)
		EndIf
		; Complete return array
		$avSize_Info[1] = $iLine_Height
		$avSize_Info[2] = $iMaxWidth
		; Convert lines to pixels and add drop margin
		$avSize_Info[3] = ($iLine_Count * $iLine_Height) + 4
	Else ; No wrapping required
		; Create return array (add drop margin to height)
		Local $avSize_Info[4] = [$sText, $iLine_Height, $iLine_Width, ($asLines[0] * $iLine_Height) + 4]
	EndIf

	; Clear up
	DllCall("gdi32.dll", "handle", "SelectObject", "handle", $hDC, "handle", $hPrevFont)
	DllCall("gdi32.dll", "bool", "DeleteObject", "handle", $hFont)
	DllCall("user32.dll", "int", "ReleaseDC", "hwnd", 0, "handle", $hDC)
	If $hLabel Then GUICtrlDelete($hLabel)

	Return $avSize_Info

EndFunc ;==>_StringSize

; #INTERNAL_USE_ONLY#============================================================================================================
; Name...........: _StringSize_Error_Close
; Description ...: Releases DC and deleted font object if required after error
; Syntax ........: _StringSize_Error_Close ($iExtCode, $hDC, $hGUI)
; Parameters ....: $iExtCode   - code to return
;                  $hDC, $hGUI - handles as set in _StringSize function
; Return value ..: $iExtCode as passed
; Author ........: Melba23
; Modified.......:
; Remarks .......: This function is used internally by _StringSize
; ===============================================================================================================================
Func _StringSize_Error_Close($iExtCode, $hDC = 0, $hFont = 0, $hLabel = 0)

	If $hFont <> 0 Then DllCall("gdi32.dll", "bool", "DeleteObject", "handle", $hFont)
	If $hDC <> 0 Then DllCall("user32.dll", "int", "ReleaseDC", "hwnd", 0, "handle", $hDC)
	If $hLabel Then GUICtrlDelete($hLabel)

	Return $iExtCode

EndFunc ;=>_StringSize_Error_Close

; #INTERNAL_USE_ONLY#============================================================================================================
; Name...........: _StringSize_DefaultFontName
; Description ...: Determines Windows default font
; Syntax ........: _StringSize_DefaultFontName()
; Parameters ....: None
; Return values .: Success - Returns name of system default font
;                  Failure - Returns "Tahoma"
; Author ........: Melba23, based on some original code by Larrydalooza
; Modified.......:
; Remarks .......: This function is used internally by _StringSize
; ===============================================================================================================================
Func _StringSize_DefaultFontName()

	; Get default system font data
	Local $tNONCLIENTMETRICS = DllStructCreate("uint;int;int;int;int;int;byte[60];int;int;byte[60];int;int;byte[60];byte[60];byte[60]")
	DLLStructSetData($tNONCLIENTMETRICS, 1, DllStructGetSize($tNONCLIENTMETRICS))
	DLLCall("user32.dll", "int", "SystemParametersInfo", "int", 41, "int", DllStructGetSize($tNONCLIENTMETRICS), "ptr", DllStructGetPtr($tNONCLIENTMETRICS), "int", 0)
	Local $tLOGFONT = DllStructCreate("long;long;long;long;long;byte;byte;byte;byte;byte;byte;byte;byte;char[32]", DLLStructGetPtr($tNONCLIENTMETRICS, 13))
	If IsString(DllStructGetData($tLOGFONT, 14)) Then
		Return DllStructGetData($tLOGFONT, 14)
	Else
		Return "Tahoma"
	EndIf

EndFunc
