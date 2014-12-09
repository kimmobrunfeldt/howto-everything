# Raspberry cheat sheet


## Audio

### Setup audio output

    sudo amixer cset numid=3 <output>

**Possible outputs*

* 0=auto
* 1=headphones
* 2=hdmi

### Setup volume

    amixer set PCM -- -2200

### Install text-to-speech

If you have HDMI plugged in, make sure the audio is forced to 3.5mm jack.

    sudo apt-get install festival festival-freebsoft-utils
    echo  "Hello World"| festival --tts
