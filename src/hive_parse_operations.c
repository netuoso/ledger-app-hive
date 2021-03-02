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

#include "hive_parse_operations.h"
#include "hive_types.h"
#include <string.h>
#include "os.h"

void parseHiveVote(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Voter", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Author", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Permlink", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseUint16Field(buffer, bufferLength, "Weight", arg, &read, &written);
}

void parseHiveComment(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Parent Author", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Parent Permlink", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Author", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Permlink", arg, &read, &written);
    if (argNum == 3) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Title", arg, &read, &written);
    if (argNum == 4) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Body", arg, &read, &written);
    if (argNum == 5) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "JSON Metadata", arg, &read, &written);
}

void parseHiveTransfer(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "From", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "To", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, sizeof(asset_t), "Amount", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Memo", arg, &read, &written);
}

void parseHiveTransferToVesting(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "From", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "To", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, sizeof(asset_t), "Amount", arg, &read, &written);
}

void parseHiveWithdrawVesting(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Account", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, sizeof(asset_t), "Vesting Shares", arg, &read, &written);
}

void parseHiveLimitOrderCreate(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Owner", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseUint32Field(buffer, bufferLength, "Order ID", arg, &read, &written);
    if (argNum == 1) return;

    buffer += sizeof(uint32_t); bufferLength -= sizeof(uint32_t);
    parseAssetField(buffer, sizeof(asset_t), "Amount To Sell", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, sizeof(asset_t), "Min To Receive", arg, &read, &written);
    if (argNum == 3) return;

    buffer += read; bufferLength -= read;
    printString(buffer[0] == 0x01 ? "true" : "false", "Fill or Kill", arg);
    if (argNum == 4) return;

    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);
    parseUint32Field(buffer, bufferLength, "Expiration", arg, &read, &written);
}

void parseHiveLimitOrderCancel(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Owner", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseUint32Field(buffer, bufferLength, "Order ID", arg, &read, &written);
}

void parseHiveFeedPublish(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Publisher", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Base", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Quote", arg, &read, &written);
}

void parseHiveConvert(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Owner", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseUint32Field(buffer, bufferLength, "Request ID", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, sizeof(asset_t), "Amount", arg, &read, &written);
}

void parseHiveAccountCreate(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte
    
    parseAssetField(buffer, bufferLength, "Fee", arg, &read, &written);
    buffer += read; bufferLength -= read;
    if (argNum == 0) return;

    parseStringField(buffer, bufferLength, "Creator", arg, &read, &written);
    buffer += read; bufferLength -= read;
    if (argNum == 1) return;

    parseStringField(buffer, bufferLength, "New Account Name", arg, &read, &written);
    buffer += read; bufferLength -= read;
    if (argNum == 2) return;

    char tmp[128];

    parseUint32Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numOwnerAccountAuths = 0;
    os_memmove(&numOwnerAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numOwnerAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numOwnerKeyAuths = 0;
    os_memmove(&numOwnerKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numOwnerKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 3) { // owner auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(tmp, 0, sizeof(tmp));

    parseUint32Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numActiveAccountAuths = 0;
    os_memmove(&numActiveAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numActiveAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numActiveKeyAuths = 0;
    os_memmove(&numActiveKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numActiveKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 4) { // active auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(tmp, 0, sizeof(tmp));
    os_memset(arg->data, 0, sizeof(arg->data));

    parseUint32Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numPostingAccountAuths = 0;
    os_memmove(&numPostingAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numPostingAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numPostingKeyAuths = 0;
    os_memmove(&numPostingKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numPostingKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 5) { // posting auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(arg->data, 0, sizeof(arg->data));

    parsePublicKeyField(buffer, bufferLength, "Memo Key", arg, &read, &written);
    buffer += read; bufferLength -= read;

    if( argNum == 6) return;

    os_memset(arg->data, 0, sizeof(arg->data));

    parseStringField(buffer, bufferLength, "JSON Metadata", arg, &read, &written);
}

void parseHiveAccountUpdate(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, sizeof(asset_t), "Account", arg, &read, &written);
    buffer += read; bufferLength -= read;
    if (argNum == 0) return;

    char tmp[128];

    parseUint32Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numOwnerAccountAuths = 0;
    os_memmove(&numOwnerAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numOwnerAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numOwnerKeyAuths = 0;
    os_memmove(&numOwnerKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numOwnerKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 1) { // owner auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(tmp, 0, sizeof(tmp));

    parseUint32Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numActiveAccountAuths = 0;
    os_memmove(&numActiveAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numActiveAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numActiveKeyAuths = 0;
    os_memmove(&numActiveKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numActiveKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 2) { // active auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(tmp, 0, sizeof(tmp));
    os_memset(arg->data, 0, sizeof(arg->data));

    parseUint32Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numPostingAccountAuths = 0;
    os_memmove(&numPostingAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numPostingAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numPostingKeyAuths = 0;
    os_memmove(&numPostingKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numPostingKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 3) { // posting auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(arg->data, 0, sizeof(arg->data));

    parsePublicKeyField(buffer, bufferLength, "Memo Key", arg, &read, &written);
    buffer += read; bufferLength -= read;

    if( argNum == 4) return;

    os_memset(arg->data, 0, sizeof(arg->data));

    parseStringField(buffer, bufferLength, "JSON Metadata", arg, &read, &written);

}

void parseHiveWitnessUpdate(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Owner", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "URL", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parsePublicKeyField(buffer, bufferLength, "Signing Key", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;

    // props
    char tmp[128];

    snprintf(tmp, sizeof(tmp), "Account Creation Fee: ");

    parseAssetField(buffer, sizeof(asset_t), "Witness Props", arg, &read, &written);
    buffer += read; bufferLength -= read;
    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s", arg->data);

    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), " - Max Block Size: ");

    parseUint32Field(buffer, sizeof(asset_t), "Witness Props", arg, &read, &written);
    buffer += read; bufferLength -= read;
    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s", arg->data);

    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), " - HBD Interest Rate: ");

    parseUint16Field(buffer, sizeof(asset_t), "Witness Props", arg, &read, &written);
    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s", arg->data);

    os_memset(arg->data, 0, sizeof(arg->data));
    os_memmove(arg->data, tmp, sizeof(tmp));
}

void parseHiveAccountWitnessVote(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Account", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Witness", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    printString(buffer[0] == 0x01 ? "true" : "false", "Approve", arg);
}

void parseHiveAccountWitnessProxy(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Account", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Proxy", arg, &read, &written);
}

void parseHiveDeleteComment(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Author", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Permlink", arg, &read, &written);
}

void parseHiveCustomJson(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    char tmp[128];

    uint32_t requiredAuths = 0;
    read = unpack_variant32(buffer, sizeof(uint32_t), &requiredAuths);
    buffer += read; bufferLength -= read;

    // TODO it displays ID [] when no required auth provided, should display "Required Auth"

    snprintf(tmp, sizeof(tmp), "[ ");

    for(uint32_t i = 0; i < requiredAuths; ++i) {
        parseStringField(buffer, bufferLength, "Required Auths", arg, &read, &written);
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), i == requiredAuths-1 ? "%s" : "%s, ", arg->data);
        buffer += read; bufferLength -= read;
    }

    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), " ]");

    if (argNum == 0) {
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    uint32_t requiredPostingAuths = 0;
    read = unpack_variant32(buffer, sizeof(uint32_t), &requiredPostingAuths);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "[ ");

    for(uint32_t i = 0; i < requiredPostingAuths; ++i) {
        parseStringField(buffer, bufferLength, "Required Auths", arg, &read, &written);
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), i == requiredPostingAuths-1 ? "%s" : "%s, ", arg->data);
        buffer += read; bufferLength -= read;
    }

    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), " ]");

    if (argNum == 1) {
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    parseStringField(buffer, bufferLength, "ID", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "JSON", arg, &read, &written);
}

void parseHiveCommentOptions(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Author", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Permlink", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Max Payout", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseUint16Field(buffer, bufferLength, "Percent HBD", arg, &read, &written);
    if (argNum == 3) return;

    buffer += read; bufferLength -= read;
    printString(buffer[0] == 0x01 ? "true" : "false", "Allow Votes", arg);
    if (argNum == 4) return;

    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);
    printString(buffer[0] == 0x01 ? "true" : "false", "Allow Curation Rewards", arg);
    if (argNum == 5) return;

    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    uint8_t numExtensions = 0;
    os_memmove(&numExtensions, buffer, sizeof(uint8_t));

    if(numExtensions == 0) {
        printString("[]", "Beneficiaries", arg);
        return;
    } else if (numExtensions > 1) {
        THROW(EXCEPTION);
    }

    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    if(buffer[0] != 0x00) THROW(EXCEPTION); // only beneficiaries (0x00) are implemented

    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    char tmp[128];

    uint8_t numBeneficiaries = 0;
    os_memmove(&numBeneficiaries, buffer, sizeof(uint8_t));

    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    snprintf(tmp, sizeof(tmp), "[ ");

    for(uint8_t i = 0; i < numBeneficiaries; ++i) {
        parseStringField(buffer, bufferLength, "Beneficiaries", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s - ", arg->data);
        parseUint16Field(buffer, bufferLength, "Beneficiaries", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), i == numBeneficiaries-1 ? "%s" : "%s, ", arg->data);
    }

    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), " ]");

    os_memset(arg->data, 0, sizeof(arg->data));
    os_memmove(arg->data, tmp, sizeof(tmp));
}

void parseHiveSetWithdrawVestingRoute(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "From Account", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "To Account", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseUint32Field(buffer, bufferLength, "Percent", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    printString(buffer[0] == 0x01 ? "true" : "false", "Autovest", arg);
}

void parseHiveClaimAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Creator", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, sizeof(asset_t), "Fee", arg, &read, &written);
}

void parseHiveCreateClaimedAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Creator", arg, &read, &written);
    buffer += read; bufferLength -= read;
    if (argNum == 0) return;

    parseStringField(buffer, bufferLength, "New Account Name", arg, &read, &written);
    buffer += read; bufferLength -= read;
    if (argNum == 1) return;

    char tmp[128];

    parseUint32Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numOwnerAccountAuths = 0;
    os_memmove(&numOwnerAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numOwnerAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numOwnerKeyAuths = 0;
    os_memmove(&numOwnerKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numOwnerKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 2) { // owner auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(tmp, 0, sizeof(tmp));

    parseUint32Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numActiveAccountAuths = 0;
    os_memmove(&numActiveAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numActiveAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numActiveKeyAuths = 0;
    os_memmove(&numActiveKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numActiveKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Active Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 3) { // active auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(tmp, 0, sizeof(tmp));
    os_memset(arg->data, 0, sizeof(arg->data));

    parseUint32Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numPostingAccountAuths = 0;
    os_memmove(&numPostingAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numPostingAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numPostingKeyAuths = 0;
    os_memmove(&numPostingKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numPostingKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Posting Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if( argNum == 4) { // posting auth
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    os_memset(arg->data, 0, sizeof(arg->data));

    parsePublicKeyField(buffer, bufferLength, "Memo Key", arg, &read, &written);
    buffer += read; bufferLength -= read;

    if( argNum == 5) return;

    os_memset(arg->data, 0, sizeof(arg->data));

    parseStringField(buffer, bufferLength, "JSON Metadata", arg, &read, &written);
}

void parseHiveRequestAccountRecovery(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Recovery Account", arg, &read, &written);
    buffer += read; bufferLength -= read;

    if (argNum == 0) return;

    parseStringField(buffer, bufferLength, "Account To Recover", arg, &read, &written);
    buffer += read; bufferLength -= read;

    if (argNum == 1) return;

    char tmp[128];

    parseUint32Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numOwnerAccountAuths = 0;
    os_memmove(&numOwnerAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numOwnerAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numOwnerKeyAuths = 0;
    os_memmove(&numOwnerKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numOwnerKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    os_memset(arg->data, 0, sizeof(arg->data));
    os_memmove(arg->data, tmp, sizeof(tmp));
    return;
}

void parseHiveRecoverAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Account To Recover", arg, &read, &written);
    buffer += read; bufferLength -= read;

    if (argNum == 0) return;

    char tmp[128];

    parseUint32Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numNewOwnerAccountAuths = 0;
    os_memmove(&numNewOwnerAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numNewOwnerAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numNewOwnerKeyAuths = 0;
    os_memmove(&numNewOwnerKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numNewOwnerKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    if (argNum == 1) {
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    };

    parseUint32Field(buffer, bufferLength, "Recent Owner Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numRecentOwnerAccountAuths = 0;
    os_memmove(&numRecentOwnerAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numRecentOwnerAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "Recent Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Recent Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numRecentOwnerKeyAuths = 0;
    os_memmove(&numRecentOwnerKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numRecentOwnerKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "Recent Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "Recent Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    os_memset(arg->data, 0, sizeof(arg->data));
    os_memmove(arg->data, tmp, sizeof(tmp));
}

void parseHiveChangeRecoveryAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Account To Recover", arg, &read, &written);
    if (argNum == 0) return;
    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "New Recovery Account", arg, &read, &written);
}

void parseHiveTransferToSavings(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "From", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "To", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Amount", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Memo", arg, &read, &written);
}

void parseHiveTransferFromSavings(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "From", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseUint32Field(buffer, bufferLength, "Request ID", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "To", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Amount", arg, &read, &written);
    if (argNum == 3) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Memo", arg, &read, &written);
}

void parseHiveCancelTransferFromSavings(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "From", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseUint32Field(buffer, bufferLength, "Request ID", arg, &read, &written);
}

void parseHiveDeclineVotingRights(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Account", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    printString(buffer[0] == 0x01 ? "true" : "false", "Decline", arg);
}

void parseHiveResetAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Reset Account", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Account To Reset", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;

    char tmp[128];

    parseUint32Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "Weight: %s - ", arg->data);
    os_memset(arg->data, 0, sizeof(arg->data));

    uint8_t numNewOwnerAccountAuths = 0;
    os_memmove(&numNewOwnerAccountAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numNewOwnerAccountAuths; ++i) {
        parseStringField(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "A%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    uint8_t numNewOwnerKeyAuths = 0;
    os_memmove(&numNewOwnerKeyAuths, buffer, sizeof(uint8_t));
    buffer += sizeof(uint8_t); bufferLength -= sizeof(uint8_t);

    for (uint8_t i = 0; i < numNewOwnerKeyAuths; ++i) {
        parsePublicKeyField(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "K%d - %s:", i+1, arg->data);
        parseUint16Field(buffer, bufferLength, "New Owner Auth", arg, &read, &written);
        buffer += read; bufferLength -= read;
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "%s || ", arg->data);
    }

    os_memset(arg->data, 0, sizeof(arg->data));
    os_memmove(arg->data, tmp, sizeof(tmp));
}

void parseHiveSetResetAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Account", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Cur Reset Account", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "New Reset Account", arg, &read, &written);
}

void parseHiveClaimRewardBalance(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Account", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Reward Hive", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Reward HBD", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Reward VESTS", arg, &read, &written);
}

void parseHiveDelegateVestingShares(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Delegator", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Delegatee", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Vesting Shares", arg, &read, &written);
}

void parseHiveCreateProposal(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Creator", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Receiver", arg, &read, &written);
    if (argNum == 1) return;

    buffer += read; bufferLength -= read;
    parseUint32Field(buffer, bufferLength, "Start Date", arg, &read, &written);
    if (argNum == 2) return;

    buffer += read; bufferLength -= read;
    parseUint32Field(buffer, bufferLength, "End Date", arg, &read, &written);
    if (argNum == 3) return;

    buffer += read; bufferLength -= read;
    parseAssetField(buffer, bufferLength, "Daily Pay", arg, &read, &written);
    if (argNum == 4) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Subject", arg, &read, &written);
    if (argNum == 5) return;

    buffer += read; bufferLength -= read;
    parseStringField(buffer, bufferLength, "Permlink", arg, &read, &written);
}

void parseHiveUpdateProposalVotes(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    uint32_t numProposals = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Voter", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;

    char tmp[128];

    read = unpack_variant32(buffer, sizeof(uint32_t), &numProposals);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "[ ");

    for(uint32_t i = 0; i < numProposals; ++i) {
        parseInt64Field(buffer, bufferLength, "Proposal IDs", arg, &read, &written);
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), i == numProposals-1 ? "%s" : "%s, ", arg->data);
        buffer += read; bufferLength -= read;
    }

    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), " ]");

    if (argNum == 1) {
        os_memset(arg->data, 0, sizeof(arg->data));
        os_memmove(arg->data, tmp, sizeof(tmp));
        return;
    }

    printString(buffer[0] == 0x01 ? "true" : "false", "Approve", arg);
}

void parseHiveRemoveProposal(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg) {
    uint32_t read = 0;
    uint32_t written = 0;

    uint32_t numProposals = 0;

    buffer++; // opType byte

    parseStringField(buffer, bufferLength, "Proposal Owner", arg, &read, &written);
    if (argNum == 0) return;

    buffer += read; bufferLength -= read;

    char tmp[128];

    read = unpack_variant32(buffer, sizeof(uint32_t), &numProposals);
    buffer += read; bufferLength -= read;

    snprintf(tmp, sizeof(tmp), "[ ");

    for(uint32_t i = 0; i < numProposals; ++i) {
        parseInt64Field(buffer, bufferLength, "Proposal IDs", arg, &read, &written);
        snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), i == numProposals-1 ? "%s" : "%s, ", arg->data);
        buffer += read; bufferLength -= read;
    }

    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), " ]");

    os_memset(arg->data, 0, sizeof(arg->data));
    os_memmove(arg->data, tmp, sizeof(tmp));
}
