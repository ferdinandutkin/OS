Option Explicit
Dim WshShell,fso,file
Set WshShell = WScript.CreateObject ("WScript.Shell")
Set fso = WScript.CreateObject("Scripting.FileSystemObject")
Set file = fso.CreateTextFile ("t4.bat")
file.WriteLine("Start scalc ")
file.close
WshShell.Run "t4.bat",0,false



