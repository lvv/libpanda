VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPDF"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

'******************************************************************************
' clsPDF
'
' Change Control:                                                      DDMMYYYY
'   Phil Rainey      File created                                      04072001
'
'Purpose:
'   clsPDF is a part of the project panda_com. panda_com is an active x
'   wrapper for the PDF generation library called Panda
'   (http://www.stillhq.com/).
'   Each instance of clsPDF represents a PDF document. clsPDF encaptulates all
'   of the functions in the panda library that affect a PDF document.
'******************************************************************************

' The text drawing modes (p 343 v 1.3)
Public Enum panda_textmode
    panda_textmode_normal = 0
    panda_textmode_outline
    panda_textmode_filledoutline
    panda_textmode_invisible
    panda_textmode_filledclipped
    panda_textmode_strokedclipped
    panda_textmode_filledstrokedclipped
    panda_textmode_clipped
End Enum


' Private variable to hold friend property
Private mlPDFRef As Long

' Private variables to hold public properties
Private msAuthor As String
Private msCreator As String
Private msTitle As String
Private msSubject As String
Private msKeywords As String
Private mlFont As Long
Private mlFontSize As Long
Private mlFontMode As Long
Private mdCharacterSpacing As Double
Private mdWordSpacing As Double
Private mdHorizontalScaling As Double
Private mdLeading As Double


'*************************************
' Friend Property - PDFRef
'*************************************
Friend Property Let PDFRef(ByVal lNewVal As Long)
    mlPDFRef = lNewVal
End Property


'*************************************
' Public Function - NewPage
'*************************************
Public Function NewPage(ByVal sSize As String) As clsPage
    
    Dim oPage As clsPage
    Dim lPageRef As Long
    
    If sSize = "" Then
        MsgBox "Invalid value pased to clsPDF.NewPage", vbExclamation
        Set NewPage = Nothing
    Else
        lPageRef = windows_panda_newpage(mlPDFRef, sSize)
        Set oPage = New clsPage
        oPage.PageRef = lPageRef
        oPage.PDFRef = mlPDFRef
        Set NewPage = oPage
    End If
    
End Function


'*************************************
' Public Function - NewTemplate
'*************************************
Public Function NewTemplate(ByVal sSize As String) As clsPage

    MsgBox "clsPDF.NewTemplate - This fuctionality is not yet fully supported.", vbExclamation, "Under Construction"
    
'    Dim oPage As clsPage
'    Dim lPageRef As Long
'
'    If sSize = "" Then
'        MsgBox "Invalid value pased to clsPDF.NewTemplate", vbExclamation
'        Set NewTemplate = Nothing
'    Else
'        lPageRef = windows_panda_newpage(mlPDFRef, sSize)
'        Set oPage = New clsPage
'        oPage.PageRef = lPageRef
'        oPage.PDFRef = mlPDFRef
'        Set NewTemplate = oPage
'    End If
    
End Function


'*************************************
' Public Function - CreateFont
'*************************************
Public Function CreateFont(ByVal sName As String, ByVal lFontType As Long, ByVal sEncoding As String) _
    As Long
    windows_panda_createfont mlPDFRef, sName, lFontType, sEncoding
End Function


'*************************************
' Public Sub - PDFClose
'*************************************
Public Sub PDFClose()
    windows_panda_close mlPDFRef
End Sub


'*************************************
' Public property - Author
'*************************************
Public Property Get Author() As String
    Author = msAuthor
End Property
Public Property Let Author(ByVal sNewValue As String)
    windows_panda_setauthor mlPDFRef, sNewValue
    msAuthor = sNewValue
End Property


'*************************************
' Public property - Creator
'*************************************
Public Property Get Creator() As String
    Creator = msCreator
End Property
Public Property Let Creator(ByVal sNewValue As String)
    windows_panda_setcreator mlPDFRef, sNewValue
    msCreator = sNewValue
End Property


'*************************************
' Public property - Title
'*************************************
Public Property Get Title() As String
    Title = msTitle
End Property
Public Property Let Title(ByVal sNewValue As String)
    windows_panda_settitle mlPDFRef, sNewValue
    msTitle = sNewValue
End Property


'*************************************
' Public property - Subject
'*************************************
Public Property Get Subject() As String
    Subject = msSubject
End Property
Public Property Let Subject(ByVal sNewValue As String)
    windows_panda_setsubject mlPDFRef, sNewValue
    msSubject = sNewValue
End Property


'*************************************
' Public property - Keywords
'*************************************
Public Property Get Keywords() As String
    Keywords = msKeywords
End Property


'*************************************
' Public property - Keywords
'*************************************
Public Property Let Keywords(ByVal sNewValue As String)
    windows_panda_setkeywords mlPDFRef, sNewValue
    msKeywords = sNewValue
End Property


'*************************************
' Public property - Font
'*************************************
Public Property Get Font() As Long
    Font = mlFont
End Property
Public Property Let Font(ByVal lNewValue As Long)
    windows_panda_setfont mlPDFRef, lNewValue
    mlFont = lNewValue
End Property


'*************************************
' Public property - FontSize
'*************************************
Public Property Get FontSize() As Long
    FontSize = mlFontSize
End Property
Public Property Let FontSize(ByVal lNewValue As Long)
    windows_panda_setfontsize mlPDFRef, lNewValue
    mlFontSize = lNewValue
End Property


'*************************************
' Public property - FontMode
'*************************************
Public Property Get FontMode() As Long
    FontMode = mlFontMode
End Property
Public Property Let FontMode(ByVal lNewValue As panda_textmode)
    windows_panda_setfontmode mlPDFRef, lNewValue
    mlFontMode = lNewValue
End Property


'*************************************
' Public property - CharacterSpacing
'*************************************
Public Property Get CharacterSpacing() As Double
    CharacterSpacing = mdCharacterSpacing
End Property
Public Property Let CharacterSpacing(ByVal dNewValue As Double)
    windows_panda_setcharacterspacing mlPDFRef, dNewValue
    mdCharacterSpacing = dNewValue
End Property


'*************************************
' Public property - WordSpacing
'*************************************
Public Property Get WordSpacing() As Double
    WordSpacing = mdWordSpacing
End Property
Public Property Let WordSpacing(ByVal dNewValue As Double)
    windows_panda_setwordspacing mlPDFRef, dNewValue
    mdWordSpacing = dNewValue
End Property


'*************************************
' Public property - HorizontalScaling
'*************************************
Public Property Get HorizontalScaling() As Double
    HorizontalScaling = mdHorizontalScaling
End Property
Public Property Let HorizontalScaling(ByVal dNewValue As Double)
    windows_panda_sethorizontalscaling mlPDFRef, dNewValue
    mdHorizontalScaling = dNewValue
End Property


'*************************************
' Public property - Leading
'*************************************
Public Property Get Leading() As Double
    Leading = mdLeading
End Property
Public Property Let Leading(ByVal dNewValue As Double)
    windows_panda_setleading mlPDFRef, dNewValue
    mdLeading = dNewValue
End Property
