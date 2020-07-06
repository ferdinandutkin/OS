Option Explicit
Dim WshShell, filename
Set WshShell = WScript.CreateObject("WScript.Shell")
filename = inputbox("Name of the file to open via notepad",  "Enter file name", "ex1.vbs")
WshShell.Run "notepad " & filename
 