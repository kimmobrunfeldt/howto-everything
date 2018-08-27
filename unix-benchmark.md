# Benchmarking unix system

I use my edition of [nench](https://github.com/n-st/nench), which always uses system ioping instead of the random binary in that repo.

To run, copy paste the nench.sh in this repo into the server and run `sudo apt-get install curl ioping && bash nench.sh`


## Results

```
-------------------------------------------------
 nench.sh v2018.04.14 -- https://git.io/nench.sh
 benchmark timestamp:    2018-08-27 20:22:23 UTC
-------------------------------------------------

Processor:    Intel(R) Xeon(R) W-2145 CPU @ 3.70GHz
CPU cores:    16
Frequency:    1820.718 MHz
RAM:          125G
Swap:         39G
Kernel:       Linux 4.15.0-33-generic x86_64

Disks:
nvme0n1    477G  SSD
nvme1n1    477G  SSD

CPU: SHA256-hashing 500 MB
    1.849 seconds
CPU: bzip2-compressing 500 MB
    3.119 seconds
CPU: AES-encrypting 500 MB
    0.582 seconds

ioping: seek rate
    min/avg/max/mdev = 35.3 us / 72.5 us / 174.4 us / 8.60 us
ioping: sequential read speed
    generated 22.7 k requests in 5.00 s, 5.55 GiB, 4.54 k iops, 1.11 GiB/s

dd: sequential write speed
    1st run:    1144.41 MiB/s
    2nd run:    1144.41 MiB/s
    3rd run:    1144.41 MiB/s
    average:    1144.41 MiB/s

IPv4 speedtests
    your IPv4:    159.69.63.xxxx

    Cachefly CDN:         108.34 MiB/s
    Leaseweb (NL):        83.22 MiB/s
    Softlayer DAL (US):   11.76 MiB/s
    Online.net (FR):      59.72 MiB/s
    OVH BHS (CA):         14.90 MiB/s

IPv6 speedtests
    your IPv6:    2a01:4f8:231:xxxx

    Leaseweb (NL):        58.69 MiB/s
    Softlayer DAL (US):   13.21 MiB/s
    Online.net (FR):      67.93 MiB/s
    OVH BHS (CA):         13.95 MiB/s
-------------------------------------------------
```
