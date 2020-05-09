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

#include "hive_types.h"
#include "hive_utils.h"
#include "os.h"
#include <stdbool.h>
#include "string.h"

void append(char subject[], const char insert[], int pos) {
    char buf[100] = {};

    strncpy(buf, subject, pos);
    int len = strlen(buf);
    strcpy(buf+len, insert);
    len += strlen(insert);
    strcpy(buf+len, subject+pos);

    strcpy(subject, buf);
}

uint8_t asset_to_string(asset_t *asset, char *out, uint32_t size) {
    if (asset == NULL) {
        THROW(INVALID_PARAMETER);
    }

    char amountSym[sizeof(asset_t)];
    os_memset(amountSym, 0, sizeof(asset_t));

    // convert asset amount to string
    i64toa(asset->amount, amountSym);

    // pad the string with leading zeros if value under 1000
    while ( strlen(amountSym) <= asset->precision ) {
        append(amountSym, "0", 0);
    }

    append(amountSym, ".", strlen(amountSym) - asset->precision);
    strcat(amountSym, " ");
    strcat(amountSym, asset->symbol);

    os_memmove(out, amountSym, sizeof(amountSym));

    return strlen(amountSym);
}

uint32_t unpack_variant32(uint8_t *in, uint32_t length, variant32_t *value) {
    uint32_t i = 0;
    uint64_t v = 0; char b = 0; uint8_t by = 0;
    do {
        b = *in; ++in; ++i;
        v |= (uint32_t)((uint8_t)b & 0x7f) << by;
        by += 7;
    } while( ((uint8_t)b) & 0x80 && by < 32 );
    
    *value = v;
    return i;
}

uint32_t public_key_to_wif(uint8_t *publicKey, uint32_t keyLength, char *out, uint32_t outLength) {
    if (publicKey == NULL || keyLength < 33) {
        THROW(INVALID_PARAMETER);
    }
    if (outLength < 40) {
        THROW(EXCEPTION_OVERFLOW);
    }

    uint8_t temp[33];
    // is even?
    temp[0] = (publicKey[64] & 0x1) ? 0x03 : 0x02;
    os_memmove(temp + 1, publicKey + 1, 32);
    return compressed_public_key_to_wif(temp, sizeof(temp), out, outLength);
}

uint32_t compressed_public_key_to_wif(uint8_t *publicKey, uint32_t keyLength, char *out, uint32_t outLength) {
    if (keyLength < 33) {
        THROW(INVALID_PARAMETER);
    }
    if (outLength < 40) {
        THROW(EXCEPTION_OVERFLOW);
    }
    
    uint8_t temp[37];
    os_memset(temp, 0, sizeof(temp));
    os_memmove(temp, publicKey, 33);
    
    uint8_t check[20];
    cx_ripemd160_t riprip;
    cx_ripemd160_init(&riprip);
    cx_hash(&riprip.header, CX_LAST, temp, 33, check, sizeof(check));
    os_memmove(temp + 33, check, 4);
    
    os_memset(out, 0, outLength);
    out[0] = 'S';
    out[1] = 'T';
    out[2] = 'M';
    uint32_t addressLen = outLength - 3;
    b58enc(temp, sizeof(temp), out + 3, &addressLen);
    if (addressLen + 3 >= outLength) {
        THROW(EXCEPTION_OVERFLOW);
    }
    return addressLen + 3;
}
