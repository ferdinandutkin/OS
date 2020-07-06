Option Explicit
Dim WshShell, fso, file1, file2
set WshShell = WScript.CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")

set file1 = fso.OpenTextFile("file1.txt", 1, 0)
set file2 = fso.OpenTextFile("file2.txt", 8, 0)

Do Until file1.AtEndOfStream
  file2.Writeline(file1.ReadLine)
Loop

'file2.WriteLine(file1.ReadAll)

file1.close
file2.close

