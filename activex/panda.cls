VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPanda"
Attribute VB_GlobalNameSpace = True
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

'******************************************************************************
' clsPanda
'
' Change Control:                                                      DDMMYYYY
'   Phil Rainey      File created                                      04072001
'
'Purpose:
'   clsPanda is a part of the project panda_com. panda_com is an active x
'   wrapper for the PDF generation library called Panda
'   (http://www.stillhq.com/).
'   clsPanda is a global object used to define functions and constants required
'   by the other panda_com objects.
'******************************************************************************
    
' Private declarations of constants that are made public as readonly properties
Private Const const_panda_pagesize_a4 = "[0 0 594 841]"
Private Const const_panda_pagesize_usletter = "[0 0 612 792]"


'*************************************
' Private Sub - Class_Initialize
'*************************************
Private Sub Class_Initialize()
    windows_panda_init
End Sub


'*************************************
' Public Function - PDFOpen
'*************************************
Public Function PDFOpen _
    (ByVal FileName As String, ByVal mode As String) As clsPDF
    
    Dim oPDF As clsPDF
    Dim lPDFRef As Long
    
    If FileName = "" Or mode = "" Then
        MsgBox "Invalid value pased to PDFOpen.", vbExclamation
        Set PDFOpen = Nothing
    Else
        lPDFRef = windows_panda_open(FileName, mode)
        Set oPDF = New clsPDF
        oPDF.PDFRef = lPDFRef
        Set PDFOpen = oPDF
    End If
        
End Function


'*************************************
' Public Property - PANDA_PAGESIZE_A4
'*************************************
Public Property Get PANDA_PAGESIZE_A4() As String
    PANDA_PAGESIZE_A4 = const_panda_pagesize_a4
End Property


'*************************************
' Public Property - PANDA_PAGESIZE_USLETTER
'*************************************
Public Property Get PANDA_PAGESIZE_USLETTER() As String
    PANDA_PAGESIZE_USLETTER = const_panda_pagesize_usletter
End Property


'*************************************
' Public Function - PandaSuperScript
'*************************************
Public Function PandaSuperScript(ByVal sText As String, ByVal lOffset As Long) As String
    PandaSuperScript = Chr(4) & lOffset & sText & Chr(6)
End Function


'*************************************
' Public Function - PandaSubScript
'*************************************
Public Function PandaSubScript(ByVal sText As String, ByVal lOffset As Long) As String
    PandaSubScript = Chr(5) & lOffset & sText & Chr(6)
End Function
