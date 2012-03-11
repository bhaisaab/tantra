#About

<pre>
 \_\_    _/_/
     \__/
     (o0)\________
     (#_)\ tantra )\
        ||------w |
        ||       ||
</pre>

Tantra is a x86 unix based monolithic kernel development project.
The sanskrit word `tantra` is means system, os:: संचालन (sanchalan: operating) तंत्र (tantra: system).

Tantra was created from scratch by: `Rohit Yadav <rohityadav89@gmail.com>`

##Requirements

Development: gcc, as, ld, objdump, objcopy, nm, dd, python, perl, mkisofs

Testing: qemu, gdb, VirtualBox
 
##Building

make qemu     -> builds the kernel and runs on qemu

make qemu-gdb -> builds the kernel for debugging with gdb

make vbox     -> builds a vm based on built kernel image and runs on VirtualBox

make iso      -> creates a bootable cd image

make setup-vm -> creates a VirtualBox compatible hard disk image

make clean    -> cleans all the build mess

##Acknowledgments
- jos/xv6 for its build system and boot loading concepts
