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

#ifndef __HIVE_TYPES_H__
#define __HIVE_TYPES_H__

#include <stdint.h>

typedef uint32_t variant32_t;
typedef char symbol_t[7];
typedef uint8_t checksum256[32];
typedef uint8_t public_key_t[33];

typedef struct transaction_header_t {
    uint16_t ref_block_num;
    uint32_t ref_block_prefix;
    uint32_t expiration;
} transaction_header_t;

typedef struct asset_t {
    int64_t amount;
    uint8_t precision;
    symbol_t symbol;
} asset_t;

uint32_t unpack_variant32(uint8_t *in, uint32_t length, variant32_t *value);

uint8_t asset_to_string(asset_t *asset, char *out, uint32_t size);

uint32_t public_key_to_wif(uint8_t *publicKey, uint32_t keyLength, char *out, uint32_t outLength);
uint32_t compressed_public_key_to_wif(uint8_t *publicKey, uint32_t keyLength, char *out, uint32_t outLength);

#endif // __HIVE_TYPES_H__
