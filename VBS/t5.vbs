Dim WshShell,file,fso
set WshShell = WScript.CreateObject("WScript.Shell")
set fso = WScript.CreateObject("Scripting.FileSystemObject")
On Error Resume Next
Message =""
strComputer = "."
Set objWMIService = GetObject("winmgmts:" & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")

Set colItems = objWMIService.ExecQuery("Select * from Win32_Processor")

For Each objItem in colItems
    Message=Message & "�������: " & objItem.CurrentClockSpeed & vbCrLf  
    Message=Message & "�����������: " & objItem.DataWidth & vbCrLf
    Message=Message & "��������: " & objItem.Description & vbCrLf
    Message=Message & "���������: " & objItem.Family & vbCrLf
    Message=Message & "������ ���: " & objItem.L2CacheSize & vbCrLf
    Message=Message & "������� ��������: " & objItem.LoadPercentage & vbCrLf
    Message=Message & "�������������: " & objItem.Manufacturer & vbCrLf
    Message=Message & "����� ����������: " & objItem.ProcessorId & vbCrLf
    Message=Message &"������: " & objItem.Version & vbCrLf
Next
set file = fso.CreateTextFile("t5.txt")
file.WriteLine(Message)
file.Close
