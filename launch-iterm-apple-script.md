### Launch iTerm and execute command using AppleScript

With this script, you can create an app which runs certain command using iTerm and activates the window above others.

Use instructions from [this stackexchange answer](http://apple.stackexchange.com/a/46226) to create an app from AppleScript code. Instead of the code used as an example, use the **launch-iterm.app** below.

**launch-iterm.app**

    tell application "iTerm"
        if (count of terminals) = 0 then
            set myterm to (make new terminal)
        else
            set myterm to current terminal
        end if

        tell myterm
            activate
            if (count of sessions) = 0 then
                launch session "Default Session"
                set mysession to (last session)
            else
                set mysession to (current session)
            end if

            tell mysession
                set name to "SSH"
                write text "ssh user@server.com"
            end tell
        end tell
    end tell
