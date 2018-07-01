# Examine PDF meta data


A few useful commands:

* `mdls my.pdf` Printss PDF meta data. (Installed by default in MacOS)
* `pdfinfo my.pdf` Prints PDF meta data. Install with `brew install xpdf`
* `pdfimages -list my.pdf 0` Prints dimension details including DPI. `brew install poppler`
* `identify -verbose my.pdf` Prints long, detailed list of information. Comes with imagemagick.

Python script to tell the dimensions of a PDF when printed with default size meta data.

```
# pip install pypdf2
# Usage: pdfsize.py

import PyPDF2
import sys
pdf = PyPDF2.PdfFileReader(sys.argv[1], "rb")
p = pdf.getPage(0)

w_in_user_space_units = p.mediaBox.getWidth()
h_in_user_space_units = p.mediaBox.getHeight()

# 1 user space unit is 1/72 inch
# 1/72 inch ~ 0.352 millimeters

w = float(p.mediaBox.getWidth()) * 0.352
h = float(p.mediaBox.getHeight()) * 0.352

print 'PDF dimensions are %smm x %smm' % (w, h)
```
