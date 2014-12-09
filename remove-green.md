# Remove green

Trying to remove 'green' pixels from image as fast as possible.
Test image: http://www.dumestudios.com/assets/malibu-green-screen.jpg

## Python version

```python
"""
Removes greenscreen from an image.
Usage: python greenscreen_remove.py image.jpg
"""

from PIL import Image
import sys
import os


def rgb_to_hsv(r, g, b):
    maxc = max(r, g, b)
    minc = min(r, g, b)
    v = maxc
    if minc == maxc:
        return 0.0, 0.0, v
    s = (maxc-minc) / maxc
    rc = (maxc-r) / (maxc-minc)
    gc = (maxc-g) / (maxc-minc)
    bc = (maxc-b) / (maxc-minc)
    if r == maxc:
        h = bc-gc
    elif g == maxc:
        h = 2.0+rc-bc
    else:
        h = 4.0+gc-rc
    h = (h/6.0) % 1.0
    return h, s, v


GREEN_RANGE_MIN_HSV = (100, 80, 70)
GREEN_RANGE_MAX_HSV = (185, 255, 255)

def main():
    # Load image and convert it to RGBA, so it contains alpha channel
    file_path = sys.argv[1]
    name, ext = os.path.splitext(file_path)
    im = Image.open(file_path)
    im = im.convert('RGBA')

    # Go through all pixels and turn each 'green' pixel to transparent
    pix = im.load()
    width, height = im.size
    for x in range(width):
        for y in range(height):
            r, g, b, a = pix[x, y]
            h_ratio, s_ratio, v_ratio = rgb_to_hsv(r / 255.0, g / 255.0, b / 255.0)
            h, s, v = (h_ratio * 360, s_ratio * 255, v_ratio * 255)

            min_h, min_s, min_v = GREEN_RANGE_MIN_HSV
            max_h, max_s, max_v = GREEN_RANGE_MAX_HSV
            if min_h <= h <= max_h and min_s <= s <= max_s and min_v <= v <= max_v:
                pix[x, y] = (0, 0, 0, 0)


    im.save(name + '.png')


if __name__ == '__main__':
    main()
```

### Profiling

    $ time python -m cProfile -s time remove_green.py malibu-green-screen.jpg
        2033823 function calls (2033805 primitive calls) in 2.899 seconds

       Ordered by: internal time

       ncalls  tottime  percall  cumtime  percall filename:lineno(function)
            1    1.340    1.340    2.872    2.872 remove_green.py:35(main)
       677000    1.002    0.000    1.266    0.000 remove_green.py:12(rgb_to_hsv)
            4    0.196    0.049    0.196    0.049 {method 'encode' of 'ImagingEncoder' objects}
       677001    0.145    0.000    0.145    0.000 {max}
       677008    0.118    0.000    0.118    0.000 {min}
            2    0.038    0.019    0.038    0.019 {method 'decode' of 'ImagingDecoder' objects}
            1    0.013    0.013    0.024    0.024 Image.py:27(<module>)
           12    0.007    0.001    0.007    0.001 {PIL._imaging.crc32}
            2    0.004    0.002    0.014    0.007 Image.py:336(preinit)
            1    0.004    0.004    0.005    0.005 __init__.py:4(<module>)
         1002    0.004    0.000    0.004    0.000 {range}
            1    0.003    0.003    2.899    2.899 remove_green.py:4(<module>)
            1    0.003    0.003    0.003    0.003 {method 'convert' of 'ImagingCore' objects}
            1    0.002    0.002    0.007    0.007 BmpImagePlugin.py:27(<module>)
            1    0.002    0.002    0.002    0.002 {PIL._imaging.new}
            1    0.002    0.002    0.002    0.002 io.py:34(<module>)
            1    0.002    0.002    0.003    0.003 collections.py:1(<module>)
            1    0.002    0.002    0.002    0.002 heapq.py:31(<module>)
            1    0.001    0.001    0.002    0.002 PngImagePlugin.py:34(<module>)
            1    0.001    0.001    0.004    0.004 ImageFile.py:30(<module>)
            1    0.001    0.001    0.001    0.001 ImagePalette.py:19(<module>)
            1    0.001    0.001    0.006    0.006 FixTk.py:1(<module>)
           19    0.001    0.000    0.001    0.000 {method 'write' of 'file' objects}
            2    0.001    0.000    0.007    0.004 {__import__}
            1    0.001    0.001    0.001    0.001 {PIL._imaging.jpeg_decoder}
            1    0.001    0.001    0.001    0.001 JpegImagePlugin.py:35(<module>)
            1    0.001    0.001    0.001    0.001 {PIL._imaging.zip_encoder}
            3    0.001    0.000    0.001    0.000 __init__.py:78(CFUNCTYPE)
            2    0.000    0.000    0.000    0.000 {open}
            1    0.000    0.000    0.000    0.000 __init__.py:265(_reset_cache)
           10    0.000    0.000    0.000    0.000 {built-in method __new__ of type object at 0x103bb17c8}
            9    0.000    0.000    0.001    0.000 abc.py:86(__new__)
           37    0.000    0.000    0.000    0.000 {method 'read' of 'file' objects}
            1    0.000    0.000    0.000    0.000 GifImagePlugin.py:28(<module>)
            1    0.000    0.000    0.198    0.198 ImageFile.py:435(_save)
           35    0.000    0.000    0.000    0.000 _weakrefset.py:36(__init__)
            6    0.000    0.000    0.008    0.001 PngImagePlugin.py:474(putchunk)
          235    0.000    0.000    0.000    0.000 {getattr}
            1    0.000    0.000    0.000    0.000 _endian.py:4(<module>)
            1    0.000    0.000    0.041    0.041 ImageFile.py:125(load)
            3    0.000    0.000    0.000    0.000 __init__.py:493(PYFUNCTYPE)
            1    0.000    0.000    0.000    0.000 ascii.py:8(<module>)
           12    0.000    0.000    0.000    0.000 abc.py:148(__subclasscheck__)
            1    0.000    0.000    0.001    0.001 PpmImagePlugin.py:18(<module>)
           14    0.000    0.000    0.000    0.000 __init__.py:147(_check_size)
            2    0.000    0.000    0.000    0.000 sre_parse.py:379(_parse)
            1    0.000    0.000    0.000    0.000 _util.py:1(<module>)
            1    0.000    0.000    0.205    0.205 PngImagePlugin.py:493(_save)
            2    0.000    0.000    0.000    0.000 {_ctypes.POINTER}
           56    0.000    0.000    0.000    0.000 _binary.py:18(o8)
            1    0.000    0.000    0.000    0.000 JpegImagePlugin.py:278(_open)
           56    0.000    0.000    0.000    0.000 {chr}
            1    0.000    0.000    0.001    0.001 numbers.py:6(<module>)
            1    0.000    0.000    0.044    0.044 Image.py:773(convert)
           50    0.000    0.000    0.000    0.000 abc.py:89(<genexpr>)
            1    0.000    0.000    0.000    0.000 ImageColor.py:20(<module>)
           24    0.000    0.000    0.000    0.000 _weakrefset.py:58(__iter__)
           12    0.000    0.000    0.000    0.000 _binary.py:60(o16be)
          4/2    0.000    0.000    0.000    0.000 sre_compile.py:32(_compile)
            1    0.000    0.000    0.000    0.000 Image.py:501(_new)
            1    0.000    0.000    0.000    0.000 __init__.py:349(__init__)
           12    0.000    0.000    0.000    0.000 abc.py:105(register)
            8    0.000    0.000    0.000    0.000 _binary.py:63(o32be)
            1    0.000    0.000    0.000    0.000 Image.py:475(Image)
            1    0.000    0.000    0.000    0.000 {method 'close' of 'file' objects}
           24    0.000    0.000    0.000    0.000 _weakrefset.py:83(add)
            1    0.000    0.000    0.015    0.015 Image.py:2195(open)
           42    0.000    0.000    0.000    0.000 {isinstance}
        36/24    0.000    0.000    0.000    0.000 {issubclass}
            7    0.000    0.000    0.000    0.000 {hasattr}
            1    0.000    0.000    0.205    0.205 Image.py:1593(save)
            1    0.000    0.000    0.000    0.000 __future__.py:48(<module>)
            5    0.000    0.000    0.000    0.000 Image.py:729(load)
            4    0.000    0.000    0.000    0.000 JpegImagePlugin.py:57(APP)
            2    0.000    0.000    0.000    0.000 locale.py:363(normalize)
            1    0.000    0.000    0.000    0.000 {posix.uname}
          6/4    0.000    0.000    0.000    0.000 sre_parse.py:140(getwidth)
           64    0.000    0.000    0.000    0.000 _binary.py:15(i8)
           18    0.000    0.000    0.000    0.000 {_struct.calcsize}
            1    0.000    0.000    0.000    0.000 numbers.py:34(Complex)
           12    0.000    0.000    0.000    0.000 _weakrefset.py:26(__exit__)
            2    0.000    0.000    0.000    0.000 genericpath.py:85(_splitext)
           17    0.000    0.000    0.000    0.000 sre_parse.py:182(__next)
            1    0.000    0.000    0.000    0.000 __init__.py:71(search_function)
            1    0.000    0.000    0.000    0.000 collections.py:26(OrderedDict)
           20    0.000    0.000    0.000    0.000 _binary.py:47(i16be)
            2    0.000    0.000    0.000    0.000 re.py:226(_compile)
            5    0.000    0.000    0.000    0.000 sre_compile.py:178(_compile_charset)
            2    0.000    0.000    0.000    0.000 {posix.stat}
           37    0.000    0.000    0.000    0.000 abc.py:15(abstractmethod)
            4    0.000    0.000    0.000    0.000 {method 'pixel_access' of 'ImagingCore' objects}
            1    0.000    0.000    0.000    0.000 locale.py:493(getdefaultlocale)
            1    0.000    0.000    0.000    0.000 posixpath.py:336(normpath)
            1    0.000    0.000    0.000    0.000 {method 'flush' of 'file' objects}
            5    0.000    0.000    0.000    0.000 sre_compile.py:207(_optimize_charset)
            1    0.000    0.000    0.000    0.000 {method 'encode' of 'unicode' objects}
            1    0.000    0.000    0.001    0.001 Image.py:416(_getencoder)
            1    0.000    0.000    0.000    0.000 JpegPresets.py:67(<module>)
            2    0.000    0.000    0.000    0.000 {method 'seek' of 'file' objects}
            1    0.000    0.000    0.000    0.000 {method 'setimage' of 'ImagingEncoder' objects}
           12    0.000    0.000    0.000    0.000 _weakrefset.py:20(__enter__)
            2    0.000    0.000    0.000    0.000 sre_compile.py:359(_compile_info)
           16    0.000    0.000    0.000    0.000 _weakrefset.py:70(__contains__)
            1    0.000    0.000    0.000    0.000 JpegImagePlugin.py:173(DQT)
            5    0.000    0.000    0.000    0.000 Image.py:2361(register_open)
            1    0.000    0.000    0.001    0.001 Image.py:399(_getdecoder)
            1    0.000    0.000    0.000    0.000 JpegImagePlugin.py:125(SOF)
           92    0.000    0.000    0.000    0.000 {method 'add' of 'set' objects}
            2    0.000    0.000    0.000    0.000 sre_parse.py:677(parse)
           21    0.000    0.000    0.000    0.000 {method 'get' of 'dict' objects}
        72/70    0.000    0.000    0.000    0.000 {len}
            2    0.000    0.000    0.000    0.000 sre_compile.py:493(compile)
            1    0.000    0.000    0.000    0.000 traceback.py:1(<module>)
           85    0.000    0.000    0.000    0.000 {method 'append' of 'list' objects}
            2    0.000    0.000    0.000    0.000 {method 'sort' of 'list' objects}
            2    0.000    0.000    0.000    0.000 posixpath.py:68(join)
            4    0.000    0.000    0.000    0.000 Image.py:596(__getattr__)
           15    0.000    0.000    0.000    0.000 sre_parse.py:201(get)
            8    0.000    0.000    0.000    0.000 ImageFile.py:485(_safe_read)
           10    0.000    0.000    0.000    0.000 Image.py:2398(register_extension)
            1    0.000    0.000    0.000    0.000 numbers.py:295(Integral)
           67    0.000    0.000    0.000    0.000 {ord}
            2    0.000    0.000    0.000    0.000 genericpath.py:15(exists)
            4    0.000    0.000    0.002    0.000 PngImagePlugin.py:490(write)
            1    0.000    0.000    0.002    0.002 ImageFile.py:246(load_prepare)
            1    0.000    0.000    0.000    0.000 _binary.py:14(<module>)
           12    0.000    0.000    0.000    0.000 sre_parse.py:130(__getitem__)
            2    0.000    0.000    0.000    0.000 Image.py:489(__init__)
            1    0.000    0.000    0.000    0.000 keyword.py:11(<module>)
            2    0.000    0.000    0.000    0.000 UserDict.py:58(get)
           11    0.000    0.000    0.000    0.000 {method 'items' of 'dict' objects}
            2    0.000    0.000    0.000    0.000 sre_compile.py:354(_simple)
           12    0.000    0.000    0.000    0.000 _weakrefset.py:52(_commit_removals)
           38    0.000    0.000    0.000    0.000 {_ctypes.sizeof}
            1    0.000    0.000    0.000    0.000 ImageFile.py:73(__init__)
            6    0.000    0.000    0.000    0.000 {method 'split' of 'str' objects}
            4    0.000    0.000    0.000    0.000 {method 'rfind' of 'str' objects}
            1    0.000    0.000    0.000    0.000 collections.py:390(Counter)
            1    0.000    0.000    0.000    0.000 numbers.py:169(Real)
            2    0.000    0.000    0.000    0.000 {_sre.compile}
            1    0.000    0.000    0.000    0.000 ascii.py:41(getregentry)
            2    0.000    0.000    0.000    0.000 locale.py:447(_parse_localename)
           11    0.000    0.000    0.000    0.000 {method 'lower' of 'str' objects}
            2    0.000    0.000    0.000    0.000 sre_parse.py:301(_parse_sub)
            9    0.000    0.000    0.000    0.000 sre_parse.py:138(append)
           24    0.000    0.000    0.000    0.000 {method 'upper' of 'str' objects}
            1    0.000    0.000    0.000    0.000 {method 'decode' of 'str' objects}
            2    0.000    0.000    0.000    0.000 sre_compile.py:478(_code)
            5    0.000    0.000    0.000    0.000 Image.py:2387(register_save)
            2    0.000    0.000    0.000    0.000 posixpath.py:104(splitext)
            1    0.000    0.000    0.000    0.000 locale.py:347(_replace_encoding)
            1    0.000    0.000    0.000    0.000 locale.py:546(getlocale)
            1    0.000    0.000    0.000    0.000 {_codecs.utf_8_decode}
            2    0.000    0.000    0.000    0.000 __init__.py:49(normalize_encoding)
            1    0.000    0.000    0.000    0.000 ImageMode.py:17(<module>)
            1    0.000    0.000    0.000    0.000 ImageFile.py:70(ImageFile)
            1    0.000    0.000    0.000    0.000 {_ctypes.set_conversion_mode}
            9    0.000    0.000    0.000    0.000 {method 'join' of 'str' objects}
            1    0.000    0.000    0.000    0.000 numbers.py:270(Rational)
            2    0.000    0.000    0.000    0.000 ImageFile.py:62(_tilesort)
            1    0.000    0.000    0.000    0.000 codecs.py:77(__new__)
            1    0.000    0.000    0.000    0.000 io.py:76(BufferedIOBase)
            3    0.000    0.000    0.000    0.000 __init__.py:494(CFunctionType)
            2    0.000    0.000    0.000    0.000 sre_parse.py:178(__init__)
            1    0.000    0.000    0.000    0.000 {method 'setimage' of 'ImagingDecoder' objects}
           12    0.000    0.000    0.000    0.000 _weakrefset.py:16(__init__)
            4    0.000    0.000    0.000    0.000 sre_parse.py:90(__init__)
            7    0.000    0.000    0.000    0.000 {method 'startswith' of 'str' objects}
            2    0.000    0.000    0.000    0.000 JpegImagePlugin.py:52(Skip)
            1    0.000    0.000    0.000    0.000 posixpath.py:365(abspath)
            4    0.000    0.000    0.000    0.000 sre_compile.py:472(isstring)
            1    0.000    0.000    0.000    0.000 PngImagePlugin.py:78(ChunkStream)
            1    0.000    0.000    0.000    0.000 {_ctypes.dlopen}
            2    0.000    0.000    0.000    0.000 re.py:188(compile)
            1    0.000    0.000    0.000    0.000 {_locale.setlocale}
            1    0.000    0.000    0.000    0.000 {sorted}
            1    0.000    0.000    0.000    0.000 __init__.py:332(CDLL)
            1    0.000    0.000    0.000    0.000 utf_8.py:15(decode)
            4    0.000    0.000    0.000    0.000 sre_parse.py:195(match)
            1    0.000    0.000    0.000    0.000 Image.py:440(_E)
           12    0.000    0.000    0.000    0.000 {method 'remove' of 'set' objects}
            7    0.000    0.000    0.000    0.000 __future__.py:75(__init__)
            4    0.000    0.000    0.000    0.000 {method 'endswith' of 'str' objects}
           12    0.000    0.000    0.000    0.000 {method '__subclasses__' of 'type' objects}
            1    0.000    0.000    0.000    0.000 ImagePalette.py:23(ImagePalette)
            6    0.000    0.000    0.000    0.000 {method 'pop' of 'dict' objects}
            1    0.000    0.000    0.000    0.000 BmpImagePlugin.py:181(DibImageFile)
            1    0.000    0.000    0.000    0.000 io.py:79(TextIOBase)
            3    0.000    0.000    0.000    0.000 __init__.py:104(CFunctionType)
            1    0.000    0.000    0.000    0.000 __init__.py:189(c_int)
            2    0.000    0.000    0.000    0.000 sre_parse.py:67(__init__)
            1    0.000    0.000    0.000    0.000 ascii.py:13(Codec)
            1    0.000    0.000    0.000    0.000 PngImagePlugin.py:179(PngStream)
            1    0.000    0.000    0.000    0.000 GifImagePlugin.py:53(GifImageFile)
            1    0.000    0.000    0.000    0.000 ImageFile.py:268(StubImageFile)
            1    0.000    0.000    0.000    0.000 ImageMode.py:22(ModeDescriptor)
            1    0.000    0.000    0.000    0.000 ascii.py:24(IncrementalDecoder)
            1    0.000    0.000    0.000    0.000 __init__.py:291(c_wchar_p)
            1    0.000    0.000    0.000    0.000 JpegImagePlugin.py:266(_accept)
            1    0.000    0.000    0.000    0.000 __init__.py:238(c_char)
            2    0.000    0.000    0.000    0.000 {method 'clear' of 'dict' objects}
            1    0.000    0.000    0.000    0.000 __init__.py:260(c_bool)
            1    0.000    0.000    0.000    0.000 {method 'keys' of 'dict' objects}
            1    0.000    0.000    0.000    0.000 ImageFile.py:255(load_end)
            3    0.000    0.000    0.000    0.000 {method 'replace' of 'str' objects}
            4    0.000    0.000    0.000    0.000 _util.py:6(isPath)
            1    0.000    0.000    0.000    0.000 __init__.py:205(c_longdouble)
            1    0.000    0.000    0.000    0.000 BmpImagePlugin.py:60(BmpImageFile)
           12    0.000    0.000    0.000    0.000 {method '__subclasshook__' of 'object' objects}
            1    0.000    0.000    0.000    0.000 __init__.py:14(<module>)
            1    0.000    0.000    0.000    0.000 UserDict.py:18(__getitem__)
            1    0.000    0.000    0.000    0.000 Image.py:2181(_decompression_bomb_check)
            1    0.000    0.000    0.000    0.000 Image.py:1956(ImageTransformHandler)
            1    0.000    0.000    0.000    0.000 {method 'index' of 'str' objects}
            1    0.000    0.000    0.000    0.000 PngImagePlugin.py:341(PngImageFile)
            3    0.000    0.000    0.000    0.000 Image.py:2376(register_mime)
            1    0.000    0.000    0.000    0.000 Image.py:1915(_ImageCrop)
            1    0.000    0.000    0.000    0.000 GifImagePlugin.py:46(_accept)
            1    0.000    0.000    0.000    0.000 __future__.py:74(_Feature)
            1    0.000    0.000    0.000    0.000 ascii.py:20(IncrementalEncoder)
            2    0.000    0.000    0.000    0.000 __init__.py:429(__init__)
            1    0.000    0.000    0.000    0.000 ascii.py:34(StreamConverter)
            1    0.000    0.000    0.000    0.000 ImageFile.py:298(Parser)
            1    0.000    0.000    0.000    0.000 JpegImagePlugin.py:273(JpegImageFile)
            1    0.000    0.000    0.000    0.000 ascii.py:31(StreamReader)
            2    0.000    0.000    0.000    0.000 UserDict.py:70(__contains__)
            4    0.000    0.000    0.000    0.000 sre_parse.py:257(_escape)
            1    0.000    0.000    0.000    0.000 io.py:69(IOBase)
            1    0.000    0.000    0.000    0.000 {method 'copy' of 'dict' objects}
            1    0.000    0.000    0.000    0.000 PngImagePlugin.py:487(__init__)
            1    0.000    0.000    0.000    0.000 __init__.py:428(LibraryLoader)
            1    0.000    0.000    0.000    0.000 __init__.py:159(py_object)
            1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}
            1    0.000    0.000    0.000    0.000 __init__.py:388(PyDLL)
            1    0.000    0.000    0.000    0.000 __init__.py:193(c_uint)
            1    0.000    0.000    0.000    0.000 BmpImagePlugin.py:54(_accept)
            1    0.000    0.000    0.000    0.000 _util.py:18(deferred_error)
            1    0.000    0.000    0.000    0.000 __init__.py:255(c_void_p)
            1    0.000    0.000    0.000    0.000 numbers.py:13(Number)
            1    0.000    0.000    0.000    0.000 PpmImagePlugin.py:56(PpmImageFile)
            1    0.000    0.000    0.000    0.000 __init__.py:359(_FuncPtr)
            1    0.000    0.000    0.000    0.000 PngImagePlugin.py:154(PngInfo)
            1    0.000    0.000    0.000    0.000 PngImagePlugin.py:484(_idat)
            2    0.000    0.000    0.000    0.000 sre_parse.py:134(__setitem__)
            1    0.000    0.000    0.000    0.000 __init__.py:180(c_ulong)
            1    0.000    0.000    0.000    0.000 __init__.py:197(c_float)
            1    0.000    0.000    0.000    0.000 _endian.py:49(BigEndianStructure)
            1    0.000    0.000    0.000    0.000 __init__.py:172(c_ushort)
            1    0.000    0.000    0.000    0.000 __init__.py:243(c_char_p)
            1    0.000    0.000    0.000    0.000 Image.py:1951(ImagePointHandler)
            2    0.000    0.000    0.000    0.000 {method 'pop' of 'list' objects}
            1    0.000    0.000    0.000    0.000 __init__.py:233(c_byte)
            1    0.000    0.000    0.000    0.000 __init__.py:176(c_long)
            1    0.000    0.000    0.000    0.000 io.py:73(RawIOBase)
            1    0.000    0.000    0.000    0.000 __init__.py:201(c_double)
            1    0.000    0.000    0.000    0.000 Image.py:34(DecompressionBombWarning)
            1    0.000    0.000    0.000    0.000 Image.py:37(_imaging_not_installed)
            1    0.000    0.000    0.000    0.000 __init__.py:294(c_wchar)
            1    0.000    0.000    0.000    0.000 posixpath.py:59(isabs)
            1    0.000    0.000    0.000    0.000 _endian.py:26(_swapped_meta)
            4    0.000    0.000    0.000    0.000 {method 'translate' of 'str' objects}
            1    0.000    0.000    0.000    0.000 __init__.py:168(c_short)
            8    0.000    0.000    0.000    0.000 sre_parse.py:126(__len__)
            1    0.000    0.000    0.000    0.000 __init__.py:226(c_ubyte)
            1    0.000    0.000    0.000    0.000 ascii.py:28(StreamWriter)


    python -m cProfile -s time remove_green.py malibu-green-screen.jpg  2,86s user 0,03s system 97% cpu 2,972 total

## C++ version

Remove green c++. Make with OS X: brew install imagemagick && c++ -std=c++11 -O2 -o removegreen removegreen.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs`

```cpp
#include <Magick++.h>
#include <iostream>

using namespace std;
using namespace Magick;

const double GREEN_RANGE_MIN_H = 100;
const double GREEN_RANGE_MIN_S = 80;
const double GREEN_RANGE_MIN_V = 70;
const double GREEN_RANGE_MAX_H = 185;
const double GREEN_RANGE_MAX_S = 255;
const double GREEN_RANGE_MAX_V = 255;


std::vector<double> rgbToHsv(double r, double g, double b) {
    std::vector<double> colors = {r, g, b};

    double maxc = *max_element(colors.begin(), colors.end());
    double minc = *min_element(colors.begin(), colors.end());
    double v = maxc;

    if (minc == maxc) {
        std::vector<double> hsv = {0.0, 0.0, v};
        return hsv;
    }

    double s = (maxc-minc) / maxc;
    double rc = (maxc-r) / (maxc-minc);
    double gc = (maxc-g) / (maxc-minc);
    double bc = (maxc-b) / (maxc-minc);
    double h = 0.0;

    if (r == maxc)
        h = bc-gc;
    else if (g == maxc)
        h = 2.0+rc-bc;
    else
        h = 4.0+gc-rc;
    h = fmod(h/6.0, 1.0);

    std::vector<double> hsv = {h, s, v};
    return hsv;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: /removegreen image.png" << endl;
        return 1;
    }

    InitializeMagick(*argv);

    // Construct the image object. Separating image construction from the
    // the read operation ensures that a failure to read the image file
    // doesn't render the image object useless.
    Image image;
    try {
        // Read a file into image object
        image.read(argv[1]);

        size_t columns = image.baseColumns();
        size_t rows = image.baseRows();

        for (ssize_t row = 0; row < rows; ++row) {
            for (ssize_t column = 0; column < columns; ++column) {
                ColorRGB rgb(image.pixelColor(column, row));

                std::vector<double> hsv = rgbToHsv(rgb.red(), rgb.green(), rgb.blue());
                double h = hsv[0] * 360;
                double s = hsv[1] * 255;
                double v = hsv[2] * 255;

                if ((GREEN_RANGE_MIN_H <= h && h <= GREEN_RANGE_MAX_H) &&
                    (GREEN_RANGE_MIN_S <= s && s <= GREEN_RANGE_MAX_S) &&
                    (GREEN_RANGE_MIN_V <= v && v <= GREEN_RANGE_MAX_V)) {
                    image.pixelColor(column, row, Color(0, 0, 0, 1));
                }
            }
        }

        image.syncPixels();
        image.write("new.png");
    }
    catch (Exception &error_) {
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
    }

    return 0;
}
```
