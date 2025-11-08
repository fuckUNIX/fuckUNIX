#!/usr/bin/env python3
import struct
import sys
import os

MAGIC = b'FFAT'
VERSION = 1
ENTRY_SIZE = 32

class FileEntry:
    def __init__(self, name, offset, size):
        self.name = name.encode('ascii')[:16].ljust(16, b'\0')
        self.offset = offset
        self.size = size
        self.reserved = b'\0\0\0\0'

    def pack(self):
        return self.name + struct.pack('<II4s', self.offset, self.size, self.reserved)

def create_fuckFAT(disk_file, input_files):
    # Calculate offsets
    header_size = 7
    num_files = len(input_files)
    table_size = num_files * ENTRY_SIZE
    data_offset = header_size + table_size

    entries = []
    current_offset = data_offset
    file_contents = []

    for f in input_files:
        size = os.path.getsize(f)
        entries.append(FileEntry(os.path.basename(f), current_offset, size))
        with open(f, 'rb') as fp:
            file_contents.append(fp.read())
        current_offset += size

    # Write image
    with open(disk_file, 'wb') as out:
        # Header
        out.write(MAGIC)
        out.write(struct.pack('<B', VERSION))
        out.write(struct.pack('<H', num_files))

        # File table
        for entry in entries:
            out.write(entry.pack())

        # Data region
        for data in file_contents:
            out.write(data)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} disk.img file1 [file2 ...]")
        sys.exit(1)
    disk_file = sys.argv[1]
    files = sys.argv[2:]
    create_fuckFAT(disk_file, files)
    print(f"Created {disk_file} with {len(files)} file(s).")
