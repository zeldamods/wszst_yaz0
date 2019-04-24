# Copyright 2018 leoetlino <leo@leolam.fr>
# Licensed under GPLv2+
import os
import subprocess
from typing import Union

_use_c_module = False
try:
    import wszst_yaz0_c
    _use_c_module = True
except ImportError:
    pass

_tool_name = 'wszst' if os.name != 'nt' else 'wszst.exe'

def compress(data: Union[bytes, memoryview], level: int = 10) -> bytes:
    return subprocess.run([_tool_name, "comp", "-", "-d-", f"-C{level}", "-M1000"], input=data, # type: ignore
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True).stdout

def decompress(data: Union[bytes, memoryview]) -> bytes:
    if data[0:4] != b"Yaz0":
        return data if isinstance(data, bytes) else bytes(data)
    if _use_c_module:
        return wszst_yaz0_c.decompress(data)
    return subprocess.run([_tool_name, "de", "-", "-d-", "-M1000"], input=data, # type: ignore
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True).stdout

def decompress_file(file_path: str) -> bytes:
    with open(file_path, "rb") as f:
        return decompress(f.read())
