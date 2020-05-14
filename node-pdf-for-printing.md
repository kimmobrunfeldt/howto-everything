# Resources for generating printable PDFs with Node.js


This doc gathers useful links and information about generating PDFs for printing purposes with Node.js. 
PDF is a flexible file format, and there are standards which allow only a subset of PDF features to 
enable better compability for certain use cases [[1][1]]:

* PDF/X: Ensures the file will work when sent for professional printing (all fonts embedded, no RGB colors, etc.).
* PDF/A: Ensures the file is appropriate for archiving-that is, it will remain readable for a long time, despite changes in technology.
* PDF/E: Makes the file particularly useful for engineering documents.


## Links

* https://github.com/Hopding/pdf-lib/issues/230
* https://github.com/foliojs/pdfkit/issues/1024
* https://github.com/foliojs/pdfkit/issues/1024
* https://github.com/alafr/SVG-to-PDFKit/
* https://stackoverflow.com/questions/31591554/embed-icc-color-profile-in-pdf
* https://www.ghostscript.com/doc/9.27/VectorDevices.htm#PDFX
* https://graphicdesign.stackexchange.com/questions/119758/why-isnt-convert-all-spots-to-cmyk-converting-my-pdf-to-cmyk-in-adobe-acroba
* https://github.com/alafr/SVG-to-PDFKit/pull/62


[1]: https://www.peachpit.com/articles/article.aspx?p=1765610&seqNum=11
