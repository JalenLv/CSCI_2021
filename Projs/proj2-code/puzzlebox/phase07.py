hash = 84035164

"""
given code
---------
union {
        int i;
        float f;
} flint;
flint.i = hash;
---------
what is flint.f?
"""

# Python trick
import struct
flint_f = struct.unpack('f', struct.pack('I', hash))[0]
print(flint_f)

# Manual analysis
flint_i = hash
sign = (flint_i >> 31) & 0x1
exponent = (flint_i >> 23) & 0xFF
mantissa = flint_i & 0x7FFFFF
flint_f = (-1)**sign * (1 + mantissa / (1 << 23)) * (2 ** (exponent - 127))
print(flint_f)

