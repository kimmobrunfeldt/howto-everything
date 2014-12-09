# How to create universal USB disk

It is suprisingly hard to create USB drive which can be used universally. This gist gathers information about the subject.

I found this information from this blog: http://sipa.ulyssis.org/2010/02/filesystems-for-portable-disks/
I also copied the text into blog-post.txt just in case.

There seems to be a node tool for the job also: https://github.com/Daegalus/udfhd


**original-blog-post.txt**

    Filesystems for portable disks

    February 22nd, 2010 Posted in tech

    These days, the easiest way to transport large amounts of data from one system to another, is using external hard disks or flash drives. They come in all shapes and sizes (from some megabytes up to more than a terabyte), and any operating system from the last decade supports them. Yet what filesystem do you use on it? Typically, Windows users will stick to FAT32 or NTFS, Mac users may use HFS+ formatted disks, and Linux users… well anything really. Each has either some serious drawbacks, or is not universally supported.

    What do we want from a filesystem for data transport?

    Universal support: there’s no use for a filesystem that can’t be read by a friend who uses an exotic operating system.
    Large file support: since the disks are large, you probably want to transport some large files using them.
    Unix permissions: only an issue if you’re using some Unix-derived system, and maybe not a priority, but still useful (eg. for backups).
    Let’s have a look at what is typically used:

    FAT-based filesystems: de factor standard, but lacks support for +4GiB files, fragments easily, has no unix permissions
    NTFS: better choice for Windows users, and reasonably supported on Linux and Mac (through eg. the ntfs-3g prgram).
    HFS+: obviously very well supported on OS X, but hard to create and use (need to disable journalling) on Linux, and even harder in Windows (use a separate tool, or use a commercial driver) .
    Ext2, Ext3, Ext4, …: pretty much Linux-only, and hard to use on other systems.
    The best choice for a portable system would seem to be NTFS, yet i would prefer not to use a filesystem whose compatibility depends on a reverse-engineered driver.. at least not when there is an alternative. And apparently, there is.

    The UDF filesystem (Universal Disk Format)  seems to be made for this purpose. Although rarely used on hard drives, it is the basis for DVD’s and Blu-Ray disks, and has at least some level of support in each reasonably recent operating system. The problem is that there exist many versions of it (1.02, 1.5, 2.01, 2.5 and 2.6 are in use), it can be used in different “builds” (plain, vat and spared), and its blocksize can vary….

    I started doing some research about it, and almost every system out there seems to support different combinations of parameters and versions. I will limit myself to the plain build (which is what one would use on overwritable media like hard disks):

    Windows XP: supports read-only support for UDF up to version 2.01, but the UDF block size must match the block size of the underlying device (which for USB-sticks and disks is 512 bytes). Furthermore, Windows does not support hard disks without a partition table (even though UDF is meant to be used on whole disks, and there is no partition type id assigned for it), a limitation that seems strange, since it does not exist for flash drives, which are accessed identically…
    Windows Vista and 7 have full support up to UDF v2.6, but the UDF block size has the same constraint.
    Linux 2.6 supports UDF fully up to 2.01 and Linux 2.6.26 up to 2.5. However, before Linux 2.6.30 a command line option is necessary to mount if the UDF block size is not 2048.
    Mac OS X 10.5 supports UDF fully up to UDF 2.01, but only when used on a full disk, so not partitioned.
    As explained above, for USB harddisks, Windows requires the disk to be partitioned. On the other side, UDF only works in OSX when it is used on a full disk (unpartitioned). Rather surprisingly, there is a solution which works for both: having the disk partitioned and unpartitioned at the same time.

    DOS partition tables are stored in bytes 446-510 of the master boot record. This master boot record is stored in the first sector on disk, sector 0. Typically, the first partition specified will start some kilobytes further. However, it seems possible to construct a partition table whose first partition starts at sector 0, so the result is a partition which contains the partition table itself. Partition editor programs seem to refuse to create such a table, but at least recent Linux and Windows kernels don’t seem to bother.

    The nice thing is that UDF does not (i suppose deliberately) use the first few kilobytes of the partition or disk it is placed on, so this place can really be used to store a legacy partition table, referring to a “partition” that spans the whole disk. Some testing shows that this really works on Linux, Windows and Mac OS X. I’ll probably write a simple script to do all the partitioning and formatting.

    So to conclude, it is possible to have a portable disk which

    Mounts automatically read-write in Linux 2.6.30+, Mac OS X 10.5+, Windows Vista+
    Can be used read-only in Windows XP, and be used after a command line mount in Linux 2.6.0+
    Supports large files, UNIX permissions, Unicode filenames, symlinks, hardlinks, …
    I know what i’m going to format my portable storage devices with!

    UPDATE: you can find a Perl script for creating these partition tables here ( the perl script is included into gist )

**udfhd.pl**

```perl
#!/usr/bin/perl -w

#    udfhd.pl - partition and format a hard disk using UDF
#    Copyright (C) 2010   Pieter Wuille
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

use Fcntl qw(SEEK_SET SEEK_END);

my $SECTORSIZE=512;

sub encode_lba {
  my ($lba) = @_;
  my $res=pack("V",$lba);
  return $res;
}

sub encode_chs {
  my ($lba,$heads,$sects) = @_;
  my $C= $lba/($heads*$sects);
  $C=1023 if ($C>1023);
  my $S=1+($lba % $sects);
  my $H=($lba/$sects) % $heads;
  my $res=pack("WWW",$H & 255,($S&63)|((($C/256)&3)*64),$C&255);
  return $res;
}

sub encode_entry {
  my ($begin_sect,$size_sect,$bootable,$type,$heads,$sects) = @_;
  return (pack("W",0) x 16) if ($size_sect == 0);
  my $res="";
  if ($bootable) { $res=pack("W",0x80); } else { $res=pack("W",0); }
  $res .= encode_chs($begin_sect,$heads,$sects);
  $res .= pack("W",$type);
  $res .= encode_chs($begin_sect+$size_sect-1,$heads,$sects);
  $res .= encode_lba($begin_sect);
  $res .= encode_lba($size_sect);
  return $res;
}

sub generate_fmbr {
  use integer;
  my ($maxlba,$heads,$sects)=@_;
  $maxlba -= ($maxlba % ($heads*$sects));
  my $res=pack("W",0) x 440; # code section
  $res .= pack("V",0);       # disk signature
  $res .= pack("W",0) x 2;   # padding
  $res .= encode_entry(0,$maxlba,0,0x0B,$heads,$sects); # primary partition spanning whole disk
  $res .= pack("W",0) x 48;  # 3 unused partition entries
  $res .= pack("W",0x55);    # signature part 1
  $res .= pack("W",0xAA);    # signature part 2
  return ($res,$maxlba);
}

$|=1;

if (! -e $ARGV[0]) {
  print "Syntax: $0 /dev/diskdevice [label] [size_in_bytes]"
}

my $udfpath="";
my $udftype;
if (-x "/usr/bin/mkudffs") { $udfpath="/usr/bin/mkudffs"; $udftype="mkudffs" }
if (-x "/sbin/newfs_udf") { $udfpath="/sbin/newfs_udf"; $udftype="newfs_udf" }

if (! defined($udftype)) {
  print STDERR "Neither mkudffs or newfs_udf could be found. Exiting.";
}

my $dev=shift @ARGV;
my $label="UDF";
if (defined $ARGV[0]) {
  $label=shift @ARGV;
}


open DISK,"+<",$dev || die "Cannot open '$dev' read/write: $!";
my $size=(-s $dev);
if (defined $ARGV[0]) {
  $size=shift @ARGV;
}
if ($size<=0) {
  $size=sysseek DISK, 0, 2;
  sysseek DISK, 0, 0;
}
if ($size<=0) {
  seek(DISK,0,SEEK_END) || die "Cannot seek to end of device: $!";
  my $size=tell(DISK);
}
seek(DISK,0,SEEK_SET) || die "Cannot seek to begin of device: $!";

$size = (-s $dev) if ($size<=0);
if ($size<=0) {
  die "Cannot calculate device size, please use: $0 device label [size_in_bytes]";
}

print "Writing MBR...";
my ($mbr,$maxlba) = generate_fmbr($size/$SECTORSIZE,255,63);
print DISK $mbr || die "Cannot write MBR: $!";
print "done\n";

print "Cleaning first 4096 sectors...";
for (my $i=1; $i<4096; $i++) {
  print DISK (pack("W",0)x$SECTORSIZE) || die "Cannot clear sector $i: $!";
}
print "done\n";

close DISK || die "Cannot close disk device: $!";

print "Creating $maxlba-sector UDF v2.01 filesystem with label '$label' on $dev using $udftype...\n";
if ($udftype eq "mkudffs") {
  system($udfpath,"--blocksize=$SECTORSIZE","--udfrev=0x0201","--lvid=$label","--vid=$label","--media-type=hd","--utf8",$dev,$maxlba);
} elsif ($udftype eq "newfs_udf") {
  system($udfpath,"-b",$SECTORSIZE,"-m","blk","-t","ow","-s",$maxlba,"-r","2.01","-v",$label,"--enc","utf8",$dev);
}
```
