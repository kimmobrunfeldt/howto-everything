## Summary of audio/media support in Windows Phones' Internet Explorer(versions 9-11)

I also made a stackoverflow Q&A from this: http://stackoverflow.com/questions/23679205/how-to-record-audio-in-web-page-using-windows-phones-microphone

---

Let's take recording audio using device's microphone for example. With web techs you would have following options:

1. Get audio using HTML5 file API
2. Record audio using Flash/Silverlight
3. Record audio using HTML5 APIs
4. Use IEs own custom API for audio record(**as far as I know, there is no such API**)

#### 1. Option - File API

It would be possible to provide a file picker where user can upload their audio file to server.

IE 9 and 10 versios do not support file api. IE 11 supports file api. This was tested using [simple file API demo](http://html5demos.com/file-api-simple). Also see http://viljamis.com/blog/2012/file-upload-support-on-mobile/ and http://caniuse.com/fileapi

To seamlessly integrate audio recording to web app running on IE 11, you would need to have native audio recorder app which would appear in the file upload's "Choose an app" - app picker. This option wouldn't be so user friendly. Also it should be noted that mobile IEs do not support audio API which can be used to manipulate audio [source](http://caniuse.com/#feat=audio-api)

#### 2. Option - Flash/Silverlight

Flash is not supported or developed for mobile devices anymore: http://blogs.adobe.com/conversations/2011/11/flash-focus.html

Quote from the source:

    Our future work with Flash on mobile devices will be focused on enabling Flash developers to package native apps with Adobe AIR for all the major app stores.  We will no longer continue to develop Flash Player in the browser to work with new mobile device configurations (chipset, browser, OS version, etc.) following the upcoming release of Flash Player 11.1 for Android and BlackBerry PlayBook.  We will of course continue to provide critical bug fixes and security updates for existing device configurations.  We will also allow our source code licensees to continue working on and release their own implementations.

Silverlight plugin for IE is not apparently* available for any mobile IE. It is possible to [create Windows Phone apps with Silverlight](http://www.microsoft.com/silverlight/windows-phone/), but it is a separate from Silverlight browser plugin, which would be needed to record audio in web browser.

* Apparently means that I didn't find any references of Silverlight plugin for mobile IE. Some useful links:

    - [Silverlight website on mobile phone](http://stackoverflow.com/questions/5527170/silverlight-website-on-mobile-phone)
    - [Q&A about silverlight support](http://answers.microsoft.com/en-us/winphone/forum/wp8-wpapps/microsoft-silverlight/6183e733-9ba9-4c01-82b7-47e455e4b25d)
    - [Integrating Silverlight with a Web Page](http://msdn.microsoft.com/en-us/library/cc838145(v=vs.95).aspx)
    - [Silverlight.js (deprecated)](http://archive.msdn.microsoft.com/silverlightjs)


#### 3. Option - getUserMedia HTML5 API

[IEs do not support getUserMedia API](http://caniuse.com/#search=getusermedia) which is needed for capturing audio/video. I also confirmed this by testing mobile IE10 and IE11 using these demo-pages: [demo1](http://webaudiodemos.appspot.com/AudioRecorder/index.html) and [demo2](kimmobrunfeldt.github.io/html5record/sample1.html). These demo pages worked with mobile Chrome. See also http://stackoverflow.com/questions/19815442/how-to-record-audio-on-webpage-ios-android-pc-mac-no-flash and http://blogs.msdn.com/b/ie/archive/2011/12/09/media-capture-api-helping-web-developers-directly-import-image-video-and-sound-data-into-web-apps.aspx


#### Conclusion

Recording audio from web app in mobile IEs is not possible. You could make a separate audio recorder app and use it with browser's file API, but it would require users to first download a audio recorder app before using the web app.
