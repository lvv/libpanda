VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPage"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

'******************************************************************************
' clsPage
'
' Change Control:                                                      DDMMYYYY
'   Phil Rainey      File created                                      04072001
'
'Purpose:
'   clsPage is a part of the project panda_com. panda_com is an active x
'   wrapper for the PDF generation library called Panda
'   (http://www.stillhq.com/).
'   Each instance of clsPage represents a page of a PDF document. clsPage
'   encaptulates all of the functions in the panda library that affect a page
'   of a PDF document.
'******************************************************************************

' The image types supported by panda
Public Enum panda_image_type
    panda_image_tiff = 0
    panda_image_jpeg
    panda_image_png
End Enum

' The line cap styles (p 139 v 1.3 II)
Public Enum panda_linecap_style
    panda_linecap_butt = 0
    panda_linecap_round
    panda_linecap_projectedsquare
    panda_linecap_max       ' Not in the spec!
End Enum
  
' The line join styles (p 140 v 1.3 II)
Public Enum panda_linejoin_style
    panda_linejoin_miter = 0
    panda_linejoin_round
    panda_linejoin_bevel
    panda_linejoin_max      ' Not in the spec!
End Enum


' Private variables to hold friend properties
Private mlPDFRef As Long
Private mlPageRef As Long

' Private variables to hold public properties
Private mlLineWidth As Long
Private mlLineCap As Long
Private mlLineJoin As Long


'*************************************
' Friend Property - PDFRef
'*************************************
Friend Property Let PDFRef(lNewValue As Long)
    mlPDFRef = lNewValue
End Property


'*************************************
' Friend Property - PageRef
'*************************************
Friend Property Get PageRef() As Long
    PageRef = mlPageRef
End Property
Friend Property Let PageRef(lNewValue As Long)
    mlPageRef = lNewValue
End Property


'*************************************
' Public Sub - SetLineStart
'*************************************
Public Sub SetLineStart(ByVal x As Long, ByVal y As Long)
    windows_panda_setlinestart mlPageRef, x, y
End Sub


'*************************************
' Public Sub - AddLineSegment
'*************************************
Public Sub AddLineSegment(ByVal x As Long, ByVal y As Long)
    windows_panda_addlinesegment mlPageRef, x, y
End Sub


'*************************************
' Public Sub - AddCubicCurveSegment
'*************************************
Public Sub AddCubicCurveSegment _
    (ByVal x As Long, ByVal y As Long, ByVal cx1 As Long, _
    ByVal cy1 As Long, ByVal cx2 As Long, ByVal cy2 As Long)
    windows_panda_addcubiccurvesegment mlPageRef, x, y, cx1, cy1, cx2, cy2
End Sub


'*************************************
' Public Sub - CloseLine
'*************************************
Public Sub CloseLine()
    windows_panda_closeline mlPageRef
End Sub


'*************************************
' Public Sub - AddQuadraticCurveSegmentOne
'*************************************
Public Sub AddQuadraticCurveSegmentOne _
    (ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long)
    windows_panda_addquadraticcurvesegmentone mlPageRef, x, y, cx, cy
End Sub


'*************************************
' Public Sub - AddQuadraticCurveSegmentTwo
'*************************************
Public Sub AddQuadraticCurveSegmentTwo _
    (ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long)
    windows_panda_addquadraticcurvesegmenttwo mlPageRef, x, y, cx, cy
End Sub


'*************************************
' Public Sub - Rectangle
'*************************************
Public Sub Rectangle _
    (ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long)
    windows_panda_rectangle mlPageRef, top, left, bottom, right
End Sub


'*************************************
' Public Sub - EndLine
'*************************************
Public Sub EndLine()
    windows_panda_endline mlPageRef
End Sub


'*************************************
' Public Sub - StrokeLine
'*************************************
Public Sub StrokeLine()
    windows_panda_strokeline mlPageRef
End Sub


'*************************************
' Public Sub - FillLine
'*************************************
Public Sub FillLine()
    windows_panda_fillline mlPageRef
End Sub


'*************************************
' Public Property - LineWidth
'*************************************
Public Property Get LineWidth() As Long
    LineWidth = mlLineWidth
End Property
Public Property Let LineWidth(ByVal lNewValue As Long)
    windows_panda_setlinewidth mlPageRef, lNewValue
    mlLineWidth = lNewValue
End Property


'*************************************
' Public Property - LineCap
'*************************************
Public Property Get LineCap() As Long
    LineCap = mlLineCap
End Property
Public Property Let LineCap(ByVal lNewValue As panda_linejoin_style)
    windows_panda_setlinejoin mlPageRef, lNewValue
    mlLineCap = lNewValue
End Property


'*************************************
' Public Property - LineJoin
'*************************************
Public Property Get LineJoin() As Long
     LineJoin = mlLineJoin
End Property
Public Property Let LineJoin(ByVal lNewValue As panda_linejoin_style)
    windows_panda_setlinejoin mlPageRef, lNewValue
    mlLineJoin = lNewValue
End Property


'*************************************
' Public Sub - SetLineDash
'*************************************
Public Sub SetLineDash _
    (ByVal ontime As Long, ByVal offtime As Long, ByVal period As Long)
    windows_panda_setlinedash mlPageRef, ontime, offtime, period
End Sub


'*************************************
' Public Sub - SetFillColor
'*************************************
Public Sub SetFillColor _
    (ByVal red As Long, ByVal green As Long, ByVal blue As Long)
    windows_panda_setfillcolor mlPageRef, red, green, blue
End Sub


'*************************************
' Public Sub - SetLineColor
'*************************************
Public Sub SetLineColor _
    (ByVal red As Long, ByVal green As Long, ByVal blue As Long)
    windows_panda_setlinecolor mlPageRef, red, green, blue
End Sub


'*************************************
' Public Sub - ImageBox
'*************************************
Public Sub ImageBox _
    (ByVal top As Long, ByVal left As Long, ByVal bottom As Long, _
    ByVal right As Long, ByVal FileName As String, ByVal imgtype As panda_image_type)
    
    If FileExists(FileName) Then
        windows_panda_imagebox mlPDFRef, mlPageRef, top, left, bottom, right, FileName, imgtype
    Else
        MsgBox "The specified file does not exist", vbCritical, "Error calling clsPage.ImageBox"
    End If
    
End Sub


'*************************************
' Public Sub - ImageBoxRot
'*************************************
Public Sub ImageBoxRot _
    (ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long, _
    ByVal angle As Double, ByVal FileName As String, ByVal imgtype As panda_image_type)
    
    If FileExists(FileName) Then
        windows_panda_imageboxrot mlPDFRef, mlPageRef, top, left, bottom, right, angle, FileName, imgtype
    Else
        MsgBox "The specified file does not exist", vbCritical, "Error calling clsPage.ImageBoxRot"
    End If
End Sub


'*************************************
' Public Sub - ApplyTemplate
'*************************************
Public Sub ApplyTemplate(ByVal oTemplate As clsPage)
    MsgBox "clsPage.ApplyTemplate - This fuctionality is not yet fully supported.", vbExclamation, "Under Construction"
    'windows_panda_applytemplate mlPDFRef, mlPageRef, oTemplate.PageRef
End Sub


'*************************************
' Public Sub - TextBox
'*************************************
Public Sub TextBox _
    (ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long, ByVal str As String)
    windows_panda_textbox mlPDFRef, mlPageRef, top, left, bottom, right, str
End Sub

