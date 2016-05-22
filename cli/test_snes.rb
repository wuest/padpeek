#!/usr/bin/env ruby

BUTTON_BITS = { A:   0x0001, B: 0x0100, X:      0x0002, Y:     0x0200,
                '^': 0x1000, v: 0x2000, '<':    0x4000, '>':   0x8000,
                L:   0x0004, R: 0x0008, SELECT: 0x0400, START: 0x0800 }

def help
  puts "Usage: #{$PROGRAM_NAME} <DEVICE>"
  puts 'Reads output from a padspy arduino-modded controller'
  exit
end

def synced_packet(serial)
  buf = []
  until buf[0, 2] == [0xff, 0xff] && buf[6, 2] == [0x00, 0x00]
    s_in = serial.read(1)
    buf << s_in.ord unless s_in.nil?
    buf = buf[-8, 8] if buf.size > 8
  end
  buf[2, 4]
end

def on((name, bit), byte)
  format("#{name} (%s) ", byte & bit != 0 ? ' ' : 'X')
end

def printable_state(bitmask)
  BUTTON_BITS
    .each_with_object(bitmask)
    .map(&method(:on))
    .each_slice(4)
    .each_with_object(' ')
    .map(&:join)
    .join("\n")
end

help unless ARGV.size == 1

File.open(File.expand_path(ARGV.first)) do |serial|
  print "\033[2J"
  loop do
    synced = synced_packet(serial)
    print "\033[1;1H"
    print printable_state(synced[2] << 8 | synced[3])
  end
end
