
I followed these steps mostly: https://medium.com/@scr34mz/make-your-mac-keyboard-work-on-medium-and-others-editors-without-dead-keys-issues-french-keyboard-4666b72fa2ae
except I installed the layout to my machine using Ukulele's organizer. In addition I also added Finland as the region.

It might be enough to be able to remove the existing default layout (Finnish for me) after these steps. If not read:
https://apple.stackexchange.com/questions/44921/how-to-remove-or-disable-a-default-keyboard-layout


I removed the enabled inputs and it seemed to persist after reboot:
```
cp ~/Library/Preferences/com.apple.HIToolbox.plist /tmp
plutil -convert xml1 /tmp/com.apple.HIToolbox.plist
# Edit the file
sudo mv /tmp/com.apple.HIToolbox.plist /Library/Preferences/com.apple.HIToolbox.plist
# Changes owner, but doesn't seem to matter
``` 

It seems that input sources are a bit different in the login screens, especially the one that's coming up first when disk encryption is enabled. 
