VERSION 5.00
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form Form1 
   Caption         =   "Windows Panda Example"
   ClientHeight    =   6285
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4530
   LinkTopic       =   "Form1"
   ScaleHeight     =   6285
   ScaleWidth      =   4530
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame2 
      Caption         =   "Debugging"
      Height          =   4812
      Left            =   120
      TabIndex        =   1
      Top             =   1320
      Width           =   4332
      Begin RichTextLib.RichTextBox status 
         Height          =   4452
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   4092
         _ExtentX        =   7223
         _ExtentY        =   7858
         _Version        =   393217
         ScrollBars      =   2
         TextRTF         =   $"interface.frx":0000
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Control"
      Height          =   1092
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4332
      Begin VB.CommandButton command1 
         Caption         =   "Start test"
         Height          =   372
         Left            =   120
         TabIndex        =   2
         Top             =   480
         Width           =   1452
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command1_Click()
Dim demo As Long
Dim currPage As Long
    
    status.Text = "Welcome to the Panda 0.4 sample application..." & vbCr & vbLf
    
    ' Setup Panda
    status.Text = status.Text & "Setup" & vbCr & vbLf
    panda.setup
    
    ' Create a new PDF
    status.Text = status.Text & "Open output.pdf" & vbCr & vbLf
    demo = panda.windows_panda_open("output.pdf", "w")
    
    '///////////////////////////////////////////////////////////////////////////
    '// Image functionality
    '///////////////////////////////////////////////////////////////////////////
    
    ' Create a page
    status.Text = status.Text & "Create a page" & vbCr & vbLf
    DoEvents
    currPage = panda.windows_panda_newpage(pdf, panda.panda_pagesize_a4)
    
    ' Put in the background images
    status.Text = status.Text & "TIFF images" & vbCr & vbLf
    DoEvents
    panda.windows_panda_imagebox demo, currPage, 0, 0, 200, 200, "input.tif", panda.panda_image_tiff
    panda.windows_panda_imagebox demo, currPage, 200, 0, 400, 200, "input2.tif", panda.panda_image_tiff
    
    status.Text = status.Text & "JPEG images" & vbCr & vbLf
    DoEvents
    panda.windows_panda_imagebox demo, currPage, 317, 317, 434, 434, "gnu_box.jpg", panda_image_jpeg
    panda.windows_panda_imagebox demo, currPage, 434, 434, 551, 551, "gnu_box.jpg", panda_image_jpeg

    ' Do an panda_imageboxrot or two to test the code included by Ceasar Miquel
    status.Text = status.Text & "JPEG images (rotated)" & vbCr & vbLf
    DoEvents
    panda.windows_panda_imageboxrot demo, currPage, 600, 0, 717, 117, 15, "gnu_box.jpg", panda_image_jpeg
    panda.windows_panda_imageboxrot demo, currPage, 600, 200, 717, 317, 30, "gnu_box.jpg", panda_image_jpeg
    panda.windows_panda_imageboxrot demo, currPage, 600, 400, 717, 517, 42, "gnu_box.jpg", panda_image_jpeg
    panda.windows_panda_imageboxrot demo, currPage, 700, 0, 817, 117, 90, "gnu_box.jpg", panda_image_jpeg
    panda.windows_panda_imageboxrot demo, currPage, 700, 200, 817, 317, 132, "gnu_box.jpg", panda_image_jpeg

    ' Insert a PNG to show that I can
    status.Text = status.Text & "PNG images" & vbCr & vbLf
    DoEvents
    panda.windows_panda_imageboxrot demo, currPage, 100, 200, 200, 300, 0, "libpng.png", panda_image_png
    panda.windows_panda_imageboxrot demo, currPage, 300, 200, 400, 300, 0, "gnu.png", panda_image_png
    panda.windows_panda_imageboxrot demo, currPage, 100, 420, 310, 620, 36, "RedbrushAlpha.png", panda_image_png

    ' (c) statement
    panda.windows_panda_setfont demo, panda.windows_panda_createfont(demo, "Times-Roman", 1, "MacRomanEncoding")
    panda.windows_panda_textbox demo, currPage, 600, 10, 700, 300, _
        "The background image on this page is Copyright 2000 Andrew Cagney"
    panda.windows_panda_textbox demo, currPage, 620, 10, 720, 300, _
        "and is distributed under the terms of the GPL..."
    panda.windows_panda_textbox demo, currPage, 310, 320, 330, 800, _
        "Flower (c) 1999 Pieter S. van der Meulen"
    
  '///////////////////////////////////////////////////////////////////////////
  '// Text functionality (with a few images thrown in as well)
  '///////////////////////////////////////////////////////////////////////////

  currPage = panda.windows_panda_newpage(demo, panda.panda_pagesize_a4)

  ' I am not drawing a multiline string here because I am not sure how to
  ' represent this in the PDF at the moment
'  sprintf (tempString,
'       "Hello %c5World! %cMy name %c5is Panda!\nAnd I am a PDF generator\nI handle multiple line text ok .once you have set a leading.",
'       4, 6, 5);
'  panda_textbox (demo, currPage, lineDepth * 20 + 10, 10 + lineDepth, 100,
'         30, tempString);
'
'  panda_setfont (demo, tempPtr = panda_createfont (demo, "Symbol", 1,
'                           "MacRomanEncoding"));
'  panda_textbox (demo, currPage, lineDepth * 20 + 50, 10 + lineDepth, 100,
'         30, "Symbol");
'  free (tempPtr);

'  panda_setfont (demo, tempPtr =
'         panda_createfont (demo, "Helvetica-Bold", 1,
'                   "MacRomanEncoding"));
'  panda_textbox (demo, currPage, lineDepth * 20 + 70, 30 + lineDepth, 100,
'         30, "A line in Helvetica-Bold");
'  free (tempPtr);

'  panda_imagebox (demo, currPage, 100, 100, 150, 150, "gnu-head.jpg",
'          panda_image_jpeg);
'  panda_textbox (demo, currPage, 90, 110, 200, 200, "INFRONTINFRONTINFRONT");

'  panda_textbox (demo, currPage, 190, 210, 300, 300, "BEHINDBEHINDBEHIND");
'  panda_imagebox (demo, currPage, 200, 200, 317, 317, "gnu_box.jpg",
'          panda_image_jpeg);

'  panda_textbox (demo, currPage, 300, 10, 400, 50,
'         "A second textbox on the page");
    
    
'  ///////////////////////////////////////////////////////////////////////////
'  // Demonstrate the supported text modes
'  ///////////////////////////////////////////////////////////////////////////

'  currPage = panda_newpage (demo, panda_pagesize_a4);
'  panda_setleading (demo, 16.0);

'  for (lineDepth = 0; lineDepth < 8; lineDepth++)
'    {
'      panda_setfontmode (demo, panda_textmode_normal);

'      Switch (lineDepth)
'    {
'    Case panda_textmode_normal:
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
'             40 + (lineDepth * 20), 400, "Normal");
'      break;

'    Case panda_textmode_outline:
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
'             40 + (lineDepth * 20), 400, "Outline");
'      break;

'    Case panda_textmode_filledoutline:
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
'             40 + (lineDepth * 20), 400, "FilledOutline");
'      break;

'    Case panda_textmode_invisible:
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
'             40 + (lineDepth * 20), 400, "Invisible");
'      break;

'    Case panda_textmode_filledclipped:
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
'             40 + (lineDepth * 20), 400, "FilledClipped");
'      break;

'    Case panda_textmode_strokedclipped:
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
'             40 + (lineDepth * 20), 400, "Stroked Clipped");
'      break;

'    Case panda_textmode_filledstrokedclipped:
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
'             40 + (lineDepth * 20), 400,
'             "Filled Stroked Clipped");
'      break;

'    Case panda_textmode_clipped:
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 10,
'             40 + (lineDepth * 20), 400, "Clipped");
'      break;
'    }

'      panda_setcharacterspacing (demo, (double) lineDepth);
'      panda_setwordspacing (demo, (double) lineDepth * 10);
'      panda_sethorizontalscaling (demo, (double) 1 - (lineDepth * 0.1));

'      panda_setfontmode (demo, lineDepth);
'      panda_textbox (demo, currPage, 20 + (lineDepth * 20), 200,
'             40 + (lineDepth * 20), 400,
'             "Demonstration of a text mode");
'    }

    '///////////////////////////////////////////////////////////////////////////
    '// Demonstrate the supported lines and curve thingies -- note that no
    '// graphics state is held from the previous set of lines, so you'll need
    '// to rebuild it each time.
    '///////////////////////////////////////////////////////////////////////////

    status.Text = status.Text & "Create a page" & vbCr & vbLf
    DoEvents
    currPage = panda.windows_panda_newpage(demo, panda.panda_pagesize_a4)

    status.Text = status.Text & "Write some text" & vbCr & vbLf
    DoEvents
'  panda_setfontmode (demo, panda_textmode_normal);
'    panda.windows_panda_textbox demo, currPage, 40, 10, 55, 200, _
'        "Please note that these shapes are lines, and there is no"
'    panda.windows_panda_textbox demo, currPage, 60, 10, 75, 200, _
'         "requirement to have the shapes closed..."

    ' Straight lines of all types -- stroked
    status.Text = status.Text & "Straight lines" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinestart currPage, 110, 110
    panda.windows_panda_addlinesegment currPage, 160, 130
    panda.windows_panda_addlinesegment currPage, 210, 186
    panda.windows_panda_addlinesegment currPage, 96, 22
    panda.windows_panda_closeline currPage
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    ' Now some curves -- stroked
    status.Text = status.Text & "Curves" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinestart currPage, 210, 210
    panda.windows_panda_addcubiccurvesegment currPage, 310, 210, 225, 300, 275, 400
    panda.windows_panda_addquadraticcurvesegmentone currPage, 160, 160, 200, 225
    panda.windows_panda_addquadraticcurvesegmenttwo currPage, 210, 210, 250, 375
    panda.windows_panda_closeline currPage
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    ' Rectangles -- stroked
    status.Text = status.Text & "Rectangle" & vbCr & vbLf
    DoEvents
    panda.windows_panda_rectangle currPage, 210, 210, 310, 310
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    ' Straight lines of all types -- stroked and filled
    status.Text = status.Text & "Filled shapes" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinecolor currPage, 99, 33, 255
    panda.windows_panda_setfillcolor currPage, 112, 38, 300
    panda.windows_panda_setlinestart currPage, 110, 310
    panda.windows_panda_setlinewidth currPage, 5
    panda.windows_panda_addlinesegment currPage, 160, 330
    panda.windows_panda_addlinesegment currPage, 210, 386
    panda.windows_panda_addlinesegment currPage, 96, 222
    panda.windows_panda_closeline currPage
    panda.windows_panda_fillline currPage
    panda.windows_panda_endline currPage
    
    ' Now some curves -- stroked and filled
    status.Text = status.Text & "Filled curves" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setfillcolor currPage, 112, 138, 37
    panda.windows_panda_setlinestart currPage, 210, 410
    panda.windows_panda_setlinewidth currPage, 5
    panda.windows_panda_addcubiccurvesegment currPage, 310, 410, 225, 500, 275, 600
    panda.windows_panda_addquadraticcurvesegmentone currPage, 160, 360, 200, 425
    panda.windows_panda_addquadraticcurvesegmenttwo currPage, 210, 410, 250, 575
    panda.windows_panda_closeline currPage
    panda.windows_panda_fillline currPage
    panda.windows_panda_endline currPage
    
    ' Rectangles -- stroked filled
    status.Text = status.Text & "Filled rectangles" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setfillcolor currPage, 38, 38, 38
    panda.windows_panda_setlinewidth currPage, 5
    panda.windows_panda_rectangle currPage, 410, 210, 510, 310
    panda.windows_panda_fillline currPage
    panda.windows_panda_endline currPage
    
    ' Straight lines of all types -- stroked and capped
    status.Text = status.Text & "Line capping" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinewidth currPage, 10
    panda.windows_panda_setlinestart currPage, 100, 600
    panda.windows_panda_addlinesegment currPage, 200, 600
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    status.Text = status.Text & "Line capping, butt join" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinewidth currPage, 10
    panda.windows_panda_setlinecap currPage, panda.panda_linecap_butt
    panda.windows_panda_setlinestart currPage, 100, 625
    panda.windows_panda_addlinesegment currPage, 200, 625
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    status.Text = status.Text & "Line capping, round join" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinewidth currPage, 10
    panda.windows_panda_setlinecap currPage, panda.panda_linecap_round
    panda.windows_panda_setlinestart currPage, 100, 650
    panda.windows_panda_addlinesegment currPage, 200, 650
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    status.Text = status.Text & "Line capping, projected square join" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinewidth currPage, 10
    panda.windows_panda_setlinecap currPage, panda.panda_linecap_projectedsquare
    panda.windows_panda_setlinestart currPage, 100, 675
    panda.windows_panda_addlinesegment currPage, 200, 675
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    ' Mitre joints
    status.Text = status.Text & "Mitre joins" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinewidth currPage, 10
    panda.windows_panda_setlinecap currPage, panda.panda_linecap_butt
    panda.windows_panda_setlinestart currPage, 300, 600
    panda.windows_panda_addlinesegment currPage, 350, 650
    panda.windows_panda_addlinesegment currPage, 400, 600
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage

    status.Text = status.Text & "Mitre joins, mitred" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinewidth currPage, 10
    panda.windows_panda_setlinejoin currPage, panda.panda_linejoin_miter
    panda.windows_panda_setlinestart currPage, 300, 625
    panda.windows_panda_addlinesegment currPage, 350, 675
    panda.windows_panda_addlinesegment currPage, 400, 625
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    status.Text = status.Text & "Mitred joins, round" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinewidth currPage, 10
    panda.windows_panda_setlinejoin currPage, panda.panda_linejoin_round
    panda.windows_panda_setlinestart currPage, 300, 650
    panda.windows_panda_addlinesegment currPage, 350, 700
    panda.windows_panda_addlinesegment currPage, 400, 650
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    status.Text = status.Text & "Mitred joins, beveled" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinewidth currPage, 10
    panda.windows_panda_setlinejoin currPage, panda.panda_linejoin_bevel
    panda.windows_panda_setlinestart currPage, 300, 675
    panda.windows_panda_addlinesegment currPage, 350, 725
    panda.windows_panda_addlinesegment currPage, 400, 675
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    ' Do some work with line dashing
    status.Text = status.Text & "Dashed lines" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinedash currPage, 1, 0, 0
    panda.windows_panda_setlinejoin currPage, panda.panda_linejoin_round
    
    status.Text = status.Text & "Dashed lines, 2" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinestart currPage, 100, 800
    panda.windows_panda_addlinesegment currPage, 100, 750
    panda.windows_panda_addlinesegment currPage, 140, 800
    panda.windows_panda_closeline currPage
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    status.Text = status.Text & "Dashed lines, 3" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinedash currPage, 3, 3, 0
    panda.windows_panda_setlinestart currPage, 150, 800
    panda.windows_panda_addlinesegment currPage, 150, 750
    panda.windows_panda_addlinesegment currPage, 190, 800
    panda.windows_panda_closeline currPage
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage
    
    status.Text = status.Text & "Dashed lines, 4" & vbCr & vbLf
    DoEvents
    panda.windows_panda_setlinedash currPage, 2, 1, 1
    panda.windows_panda_setlinestart currPage, 200, 800
    panda.windows_panda_addlinesegment currPage, 200, 750
    panda.windows_panda_addlinesegment currPage, 240, 800
    panda.windows_panda_closeline currPage
    panda.windows_panda_strokeline currPage
    panda.windows_panda_endline currPage

'  ///////////////////////////////////////////////////////////////////////////
'  // We can also setup template pages to make life a little easier (and the
'  // document a little smaller)
'  ///////////////////////////////////////////////////////////////////////////

'  templatePage = panda_newtemplate(demo, panda_pagesize_a4);
'  panda_setfillcolor (templatePage, 112, 138, 37);
'  panda_setlinestart (templatePage, 210, 410);
'  panda_setlinewidth (templatePage, 5);
'  panda_addcubiccurvesegment (templatePage, 310, 410, 225, 500, 275, 600);
'  panda_addquadraticcurvesegmentone (templatePage, 160, 360, 200, 425);
'  panda_addquadraticcurvesegmenttwo (templatePage, 210, 410, 250, 575);
'  panda_closeline (templatePage);
'  //panda_strokeline (templatePage);
'  panda_fillline (templatePage);
'  panda_endline (templatePage);

'  currPage = panda_newpage(demo, panda_pagesize_a4);
'  panda_applytemplate(demo, currPage, templatePage);
    
    
    
    
    
    
    status.Text = status.Text & "Finished" & vbCr & vbLf
    DoEvents
    panda.windows_panda_close pdf
    
    
    
    
    
    

End Sub
