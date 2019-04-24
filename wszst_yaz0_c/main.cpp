// Copyright 2019 leoetlino <leo@leolam.fr>
// Licensed under GPLv2+

#include <pybind11/pybind11.h>

#include "binary_reader.h"
#include "common_types.h"

namespace py = pybind11;
using namespace py::literals;

// based on wszst
static void decompress(const u8* const srcBegin, const u8* const srcEnd,
                       u8* const destBegin, u8* const destEnd) {
  const u8* src = srcBegin;
  u8* dest = destBegin;
  u8 groupHead, groupHeadLen = 0;
  while (src < srcEnd && dest < destEnd) {
    if (groupHeadLen == 0) {
      groupHead = *src++;
      groupHeadLen = 8;
    }
    groupHeadLen--;
    if (groupHead & 0x80) {
      *dest++ = *src++;
    } else {
      const u8 b1 = *src++;
      const u8 b2 = *src++;
      const u8* copySrc = dest - ((b1 & 0xf) << 8 | b2) - 1;
      u32 n = (b1 >> 4) ? ((b1 >> 4) + 2) : (*src++ + 0x12);
      if (copySrc < destBegin || copySrc + n > destEnd || dest + n > destEnd)
        throw std::out_of_range("invalid data");
      while (n--)
        *dest++ = *copySrc++;
    }
    groupHead <<= 1;
  }
}

PYBIND11_MODULE(wszst_yaz0_c, m) {
  m.def("decompress", [](py::buffer b) {
    const py::buffer_info buffer = b.request();
    if (buffer.itemsize != 1 || buffer.ndim != 1 || buffer.size <= 0)
      throw py::value_error("needs a non-empty unsigned char* like buffer");

    if (buffer.size < 0x10)
      throw py::value_error("buffer is too small to contain header data");

    const u8* data = static_cast<const u8*>(buffer.ptr);

    if (data[0] != 'Y' || data[1] != 'a' || data[2] != 'z' || data[3] != '0')
      throw py::value_error("not Yaz0 compressed data");

    const u32 destSize = common::BinaryReader{data, true}.read<u32>(4);
    py::bytes destBuffer{nullptr, destSize};
    u8* dest = reinterpret_cast<u8*>(PYBIND11_BYTES_AS_STRING(destBuffer.ptr()));

    {
      py::gil_scoped_release gilReleaseScope;
      decompress(data + 0x10, data + buffer.size, dest, dest + destSize);
    }

    return destBuffer;
  }, "buffer"_a);
}
