#!/usr/bin/env python
import sys
def main():
  argc = len(sys.argv)
  if argc < 2: print "Please provide path to the bootloader object"
  bootloader = sys.argv[1]
  f = open(bootloader, 'ab+')
  data = f.read(1000)
  if len(data) > 512: print "Boot block too large! (max 510)\n"
  print "Signing bootloader (size = %d): %s" % (len(data), bootloader)
  pads = '\x00' * (510 - len(data))
  f.write(pads)
  f.write('\x55\xaa') # Boot sector flag makes medium bootable
  f.close()

if __name__ == '__main__':
  main()
