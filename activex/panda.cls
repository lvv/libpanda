VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "activex"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
' There are a series of constants that we need to make available from
' the DLL

Private Sub Class_Initialize()
    ' Setup the world
    panda.setup
End Sub

Private Sub Class_Terminate()
    ' There is no cleanup currently required
End Sub

Public Property Get panda_pagesize_a4() As Long
   panda_pagesize_a4 = panda.panda_pagesize_a4
End Property

Public Property Get panda_pagesize_usletter() As Long
   panda_pagesize_usletter = panda.panda_pagesize_usletter
End Property

Public Property Get panda_image_tiff() As Long
   panda_image_tiff = panda.panda_image_tiff
End Property

Public Property Get panda_image_jpeg() As Long
   panda_image_jpeg = panda.panda_image_jpeg
End Property

Public Property Get panda_image_png() As Long
   panda_image_png = panda.panda_image_png
End Property

Public Property Get panda_linecap_butt() As Long
   panda_linecap_butt = panda.panda_linecap_butt
End Property

Public Property Get panda_linecap_round() As Long
   panda_linecap_round = panda.panda_linecap_round
End Property

Public Property Get panda_linecap_projectedsquare() As Long
   panda_linecap_projectedsquare = panda.panda_linecap_projectedsquare
End Property

Public Property Get panda_linejoin_miter() As Long
   panda_linejoin_miter = panda.panda_linejoin_miter
End Property

Public Property Get panda_linejoin_round() As Long
   panda_linejoin_round = panda.panda_linejoin_round
End Property

Public Property Get panda_linejoin_bevel() As Long
   panda_linejoin_bevel = panda.panda_linejoin_bevel
End Property

Public Sub panda_setlinestart _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long)
    panda.windows_panda_setlinestart page, x, y
End Sub

Public Sub panda_addlinesegment _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long)
    panda.windows_panda_addlinesegment page, x, y
End Sub
    
Public Sub windows_panda_addcubiccurvesegment _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long, ByVal cx1 As Long, ByVal cy1 As Long, ByVal cx2 As Long, ByVal cy2 As Long)
    panda.windows_panda_addcubiccurvesegment page, x, y, cx1, cy1, cx2, cy2
End Sub

Public Sub windows_panda_addquadraticcurvesegmentone _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long)
    panda.windows_panda_addquadraticcurvesegmentone page, x, y, cx, cy
End Sub

Public Sub windows_panda_addquadraticcurvesegmenttwo _
    (ByVal page As Long, ByVal x As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long)
    panda.windows_panda_addquadraticcurvesegmenttwo page, x, y, cx, cy
End Sub

Public Sub windows_panda_closeline _
    (ByVal page As Long)
    panda.windows_panda_closeline page
End Sub

Public Sub windows_panda_rectangle _
    (ByVal page As Long, ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long)
    panda.windows_panda_rectangle page, top, left, bottom, right
End Sub

Public Sub windows_panda_endline _
    (ByVal page As Long)
    panda.windows_panda_endline page
End Sub

Public Sub windows_panda_strokeline _
    (ByVal page As Long)
    panda.windows_panda_strokeline page
End Sub

Public Sub windows_panda_fillline _
    (ByVal page As Long)
    panda.windows_panda_fillline page
End Sub

Public Sub windows_panda_setlinewidth _
    (ByVal page As Long, ByVal width As Long)
    panda.windows_panda_setlinewidth page, width
End Sub

Public Sub windows_panda_setlinecap _
    (ByVal page As Long, ByVal cap As Long)
    panda.windows_panda_setlinecap page, cap
End Sub

Public Sub windows_panda_setlinejoin _
    (ByVal page As Long, ByVal join As Long)
    panda.windows_panda_setlinejoin page, join
End Sub

Public Sub windows_panda_setlinedash _
    (ByVal page As Long, ByVal ontime As Long, ByVal offtime As Long, ByVal period As Long)
    panda.windows_panda_setlinedash page, ontime, offtime, period
End Sub

Public Sub windows_panda_setfillcolor _
    (ByVal page As Long, ByVal red As Long, ByVal green As Long, ByVal blue As Long)
    panda.windows_panda_setfillcolor page, red, green, blue
End Sub

Public Sub windows_panda_setlinecolor _
    (ByVal page As Long, ByVal red As Long, ByVal green As Long, ByVal blue As Long)
    panda.windows_panda_setlinecolor page, red, green, blue
End Sub

Public Sub windows_panda_imagebox _
    (ByVal pdf As Long, ByVal page As Long, ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long, ByVal filename As String, ByVal imgtype As Long)
    panda.windows_panda_imagebox pdf, page, top, left, bottom, gith, filename, imgtype
End Sub

Public Sub windows_panda_imageboxrot _
    (ByVal pdf As Long, ByVal page As Long, ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long, ByVal angle As Double, ByVal filename As String, ByVal imgtype As Long)
    panda.windows_panda_imageboxrot pdf, page, top, left, bottom, gith, angle, filename, imgtype
End Sub

Public Sub windows_panda_setauthor _
    (ByVal pdf As Long, ByVal str As String)
    panda.windows_panda_setauthor pdf, str
End Sub

Public Sub windows_panda_setcreator _
    (ByVal pdf As Long, ByVal str As String)
    panda.windows_panda_setcreator pdf, str
End Sub

Public Sub windows_panda_settitle _
    (ByVal pdf As Long, ByVal str As String)
    panda.windows_panda_settitle pdf, str
End Sub

Public Sub windows_panda_setsubject _
    (ByVal pdf As Long, ByVal str As String)
    panda.windows_panda_setsubject pdf, str
End Sub

Public Sub windows_panda_setkeywords _
    (ByVal pdf As Long, ByVal str As String)
    panda.windows_panda_setkeywords pdf, str
End Sub

Public Function windows_panda_createfont _
    (ByVal pdf As Long, ByVal name As String, ByVal fonttype As Long, ByVal encoding As String) _
    As Long
    panda.windows_panda_createfont pdf, name, fonttype, encoding
End Function

Public Sub windows_panda_setfont _
    (ByVal pdf As Long, ByVal ident As Long)
    panda.windows_panda_setfont pdf, ident
End Sub

Public Sub windows_panda_setfontsize _
    (ByVal pdf As Long, ByVal size As Long)
    panda.windows_panda_setfontsize pdf, size
End Sub

Public Sub windows_panda_setfontmode _
    (ByVal pdf As Long, ByVal mode As Long)
    panda.windows_panda_setfontmode pdf, mode
End Sub

Public Sub windows_panda_setcharacterspacing _
    (ByVal pdf As Long, ByVal spacing As Double)
    panda.windows_panda_setcharacterspacing pdf, spacing
End Sub

Public Sub windows_panda_setwordspacing _
    (ByVal pdf As Long, ByVal spacing As Double)
    panda.windows_panda_setwordspacing pdf, spacing
End Sub

Public Sub windows_panda_sethorizontalscaling _
    (ByVal pdf As Long, ByVal scaling As Double)
    panda.windows_panda_sethorizontalscaling pdf, scaling
End Sub

Public Sub windows_panda_setleading _
    (ByVal pdf As Long, ByVal leading As Double)
    panda.windows_panda_setleading pdf, leading
End Sub

Public Function windows_panda_open _
    (ByVal filename As String, ByVal mode As String) _
    As Long
    panda.windows_panda_open filename, mode
End Function
    
Public Function windows_panda_newpage _
    (ByVal pdf As Long, ByVal size As String) _
    As Long
    panda.windows_panda_newpage pdf, size
End Function
    
Public Sub windows_panda_close _
    (ByVal pdf As Long)
    panda.windows_panda_close pdf
End Sub

Public Function windows_panda_newtemplate _
    (ByVal pdf As Long, ByVal pagesize As String) As Long
    panda.windows_panda_newtemplate pdf, pagesize
End Function

Public Sub windows_panda_applytemplate _
    (ByVal pdf As Long, ByVal page As Long, ByVal template As Long)
    panda.windows_panda_applytemplate pdf, page, template
End Sub

Public Sub windows_panda_textbox _
    (ByVal pdf As Long, ByVal page As Long, ByVal top As Long, ByVal left As Long, ByVal bottom As Long, ByVal right As Long, ByVal str As String)
    panda.windows_panda_textbox pdf, page, top, left, bottom, right, str
End Sub
