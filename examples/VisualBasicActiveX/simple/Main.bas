Attribute VB_Name = "basMain"
Option Explicit

Sub main()
    
    Dim oPDF As panda_com.clsPDF
    Dim oPage As panda_com.clsPage
    
    ' Create a new PDF
    Set oPDF = PDFOpen("example.pdf", "w")
    
    ' Create a new page (Page 1)
    Set oPage = oPDF.NewPage(panda_pagesize_a4)
    
    ' Put a tiff image on the page
    oPage.ImageBox 0, 0, 841, 594, "input.tif", panda_image_tiff

    ' Create a new page (Page 2)
    Set oPage = oPDF.NewPage(panda_pagesize_a4)

    ' Put a PNG image on the page
    oPage.ImageBox 0, 0, 841, 594, "libpng.png", panda_image_png
    
    ' Close the PDF file
    oPDF.PDFClose

End Sub
