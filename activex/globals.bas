Attribute VB_Name = "basGlobals"
' This file imports all the functions from the Panda DLL

Declare Sub windows_panda_init Lib "panda.dll" _
    ()
Declare Function windows_panda_test Lib "panda.dll" _
    (ByVal FileName As String, ByVal mode As String) _
    As Long

' Curves.c
Declare Sub windows_panda_setlinestart Lib "panda.dll" _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long)
Declare Sub windows_panda_addlinesegment Lib "panda.dll" _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long)
Declare Sub windows_panda_addcubiccurvesegment Lib "panda.dll" _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long, ByVal cx1 As Long, ByVal cy1 As Long, ByVal cx2 As Long, ByVal cy2 As Long)
Declare Sub windows_panda_addquadraticcurvesegmentone Lib "panda.dll" _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long)
Declare Sub windows_panda_addquadraticcurvesegmenttwo Lib "panda.dll" _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long)
Declare Sub windows_panda_closeline Lib "panda.dll" _
    (ByVal page As Long)
Declare Sub windows_panda_rectangle Lib "panda.dll" _
    (ByVal page As Long, ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long)
Declare Sub windows_panda_endline Lib "panda.dll" _
    (ByVal page As Long)
Declare Sub windows_panda_strokeline Lib "panda.dll" _
    (ByVal page As Long)
Declare Sub windows_panda_fillline Lib "panda.dll" _
    (ByVal page As Long)
Declare Sub windows_panda_setlinewidth Lib "panda.dll" _
    (ByVal page As Long, ByVal width As Long)
Declare Sub windows_panda_setlinecap Lib "panda.dll" _
    (ByVal page As Long, ByVal cap As Long)
Declare Sub windows_panda_setlinejoin Lib "panda.dll" _
    (ByVal page As Long, ByVal join As Long)
Declare Sub windows_panda_setlinedash Lib "panda.dll" _
    (ByVal page As Long, ByVal ontime As Long, ByVal offtime As Long, ByVal period As Long)
Declare Sub windows_panda_setfillcolor Lib "panda.dll" _
    (ByVal page As Long, ByVal red As Long, ByVal green As Long, ByVal blue As Long)
Declare Sub windows_panda_setlinecolor Lib "panda.dll" _
    (ByVal page As Long, ByVal red As Long, ByVal green As Long, ByVal blue As Long)

' Images.c
Declare Sub windows_panda_imagebox Lib "panda.dll" _
    (ByVal pdf As Long, ByVal page As Long, ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long, ByVal FileName As String, ByVal imgtype As Long)
Declare Sub windows_panda_imageboxrot Lib "panda.dll" _
    (ByVal pdf As Long, ByVal page As Long, ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long, ByVal angle As Double, ByVal FileName As String, ByVal imgtype As Long)

' Info.c
Declare Sub windows_panda_setauthor Lib "panda.dll" _
    (ByVal pdf As Long, ByVal str As String)
Declare Sub windows_panda_setcreator Lib "panda.dll" _
    (ByVal pdf As Long, ByVal str As String)
Declare Sub windows_panda_settitle Lib "panda.dll" _
    (ByVal pdf As Long, ByVal str As String)
Declare Sub windows_panda_setsubject Lib "panda.dll" _
    (ByVal pdf As Long, ByVal str As String)
Declare Sub windows_panda_setkeywords Lib "panda.dll" _
    (ByVal pdf As Long, ByVal str As String)

' Font.c
Declare Function windows_panda_createfont Lib "panda.dll" _
    (ByVal pdf As Long, ByVal name As String, ByVal fonttype As Long, ByVal encoding As String) _
    As Long
Declare Sub windows_panda_setfont Lib "panda.dll" _
    (ByVal pdf As Long, ByVal ident As Long)
Declare Sub windows_panda_setfontsize Lib "panda.dll" _
    (ByVal pdf As Long, ByVal size As Long)
Declare Sub windows_panda_setfontmode Lib "panda.dll" _
    (ByVal pdf As Long, ByVal mode As Long)
Declare Sub windows_panda_setcharacterspacing Lib "panda.dll" _
    (ByVal pdf As Long, ByVal spacing As Double)
Declare Sub windows_panda_setwordspacing Lib "panda.dll" _
    (ByVal pdf As Long, ByVal spacing As Double)
Declare Sub windows_panda_sethorizontalscaling Lib "panda.dll" _
    (ByVal pdf As Long, ByVal scaling As Double)
Declare Sub windows_panda_setleading Lib "panda.dll" _
    (ByVal pdf As Long, ByVal Leading As Double)

' Panda.c
Declare Function windows_panda_open Lib "panda.dll" _
    (ByVal FileName As String, ByVal mode As String) _
    As Long
    
Declare Function windows_panda_newpage Lib "panda.dll" _
    (ByVal pdf As Long, ByVal size As String) _
    As Long
    
Declare Sub windows_panda_close Lib "panda.dll" _
    (ByVal pdf As Long)

' Template.c
Declare Function windows_panda_newtemplate Lib "panda.dll" _
    (ByVal pdf As Long, ByVal pagesize As String) As Long
Declare Sub windows_panda_applytemplate Lib "panda.dll" _
    (ByVal pdf As Long, ByVal page As Long, ByVal template As Long)

' Text.c
Declare Sub windows_panda_textbox Lib "panda.dll" _
    (ByVal pdf As Long, ByVal page As Long, ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long, ByVal str As String)



'*************************************
' Public Function - FileExists
'*************************************
Public Function FileExists(sFilePath As String) As Boolean
' Returns true if "sFilePath" is the path of an existing file
    
    On Error GoTo FileNotFound

    ' This function raises an error if the file does not exist
    GetAttr sFilePath
    
    ' No error. The file must be there
    FileExists = True

    Exit Function

FileNotFound:
    
    ' There was an error reading the file attributes. Assume the file is not there.
    FileExists = False

End Function
