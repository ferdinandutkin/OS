Dim WshShell,file,fso
set WshShell = WScript.CreateObject("WScript.Shell")
set fso = WScript.CreateObject("Scripting.FileSystemObject")
On Error Resume Next
Message =""
strComputer = "."
Set objWMIService = GetObject("winmgmts:" & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")

Set colItems = objWMIService.ExecQuery("Select * from Win32_Processor")

For Each objItem in colItems
    Message=Message & "Частота: " & objItem.CurrentClockSpeed & vbCrLf  
    Message=Message & "Разрядность: " & objItem.DataWidth & vbCrLf
    Message=Message & "Описание: " & objItem.Description & vbCrLf
    Message=Message & "Семейство: " & objItem.Family & vbCrLf
    Message=Message & "Размер кэш: " & objItem.L2CacheSize & vbCrLf
    Message=Message & "Текущая загрузка: " & objItem.LoadPercentage & vbCrLf
    Message=Message & "Производитель: " & objItem.Manufacturer & vbCrLf
    Message=Message & "Номер процессора: " & objItem.ProcessorId & vbCrLf
    Message=Message &"Версия: " & objItem.Version & vbCrLf
Next
set file = fso.CreateTextFile("t5.txt")
file.WriteLine(Message)
file.Close
