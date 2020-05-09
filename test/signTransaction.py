#!/usr/bin/env python
"""
/*******************************************************************************
*   Andrew (netuoso) Chaney
*   (c) 2020 Andrew Chaney
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/
"""

import binascii
import json
import struct
from hiveBase import Transaction
from ledgerblue.comm import getDongle
import argparse

def parse_bip32_path(path):
    if len(path) == 0:
        return ""
    result = ""
    elements = path.split('/')
    for pathElement in elements:
        element = pathElement.split('\'')
        if len(element) == 1:
            result = result + struct.pack(">I", int(element[0]))
        else:
            result = result + struct.pack(">I", 0x80000000 | int(element[0]))
    return result


parser = argparse.ArgumentParser()
parser.add_argument('--path', help="BIP 32 path to retrieve")
parser.add_argument('--file', help="Transaction in JSON format")
args = parser.parse_args()

if args.path is None:
    args.path = "48'/13'/0'/0'/0'"

if args.file is None:
    args.file = 'txs/tx-transfer.json'

donglePath = parse_bip32_path(args.path)
pathSize = len(donglePath) / 4

with file(args.file) as f:
    obj = json.load(f)
    tx = Transaction.parse(obj)
    tx_raw = tx.encode()
    signData = tx_raw

    dongle = getDongle(True)
    offset = 0
    first = True
    singSize = len(signData)
    while offset != singSize:
        if singSize - offset > 200:
            chunk = signData[offset: offset + 200]
        else:
            chunk = signData[offset:]

        if first:
            print("LENGTH DONGLE")
            totalSize = len(donglePath) + 1 + len(chunk)
            print(binascii.hexlify(chunk))
            apdu = "D4040000".decode('hex') + chr(totalSize) + chr(pathSize) + donglePath + chunk
            first = False
        else:
            totalSize = len(chunk)
            apdu = "D4048000".decode('hex') + chr(totalSize) + chunk

        offset += len(chunk)
        result = dongle.exchange(bytes(apdu))
        # print binascii.hexlify(result)

print(binascii.hexlify(result))
