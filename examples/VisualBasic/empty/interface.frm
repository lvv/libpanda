VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   6276
   ClientLeft      =   48
   ClientTop       =   336
   ClientWidth     =   4524
   LinkTopic       =   "Form1"
   ScaleHeight     =   6276
   ScaleWidth      =   4524
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame2 
      Caption         =   "Debugging"
      Height          =   4812
      Left            =   120
      TabIndex        =   1
      Top             =   1320
      Width           =   4332
   End
   Begin VB.Frame Frame1 
      Caption         =   "Control"
      Height          =   1092
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4332
      Begin VB.CommandButton Command1 
         Caption         =   "Command1"
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
Dim pdf As Long
Dim page As Long
    
    panda.setup
    
    pdf = panda.windows_panda_open("output.pdf", "w")
    page = panda.windows_panda_newpage(pdf, panda.panda_pagesize_a4)
    panda.windows_panda_close pdf
End Sub
