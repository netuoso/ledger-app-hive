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

#include <string.h>
#include "hive_stream.h"
#include "os.h"
#include "cx.h"
#include "hive_types.h"
#include "hive_utils.h"
#include "hive_parse.h"
#include "hive_parse_operations.h"
#include "hive_parse_unknown.h"

void initTxContext(txProcessingContext_t *context, 
                   cx_sha256_t *sha256, 
                   cx_sha256_t *dataSha256, 
                   txProcessingContent_t *processingContent,
                   uint8_t dataAllowed) {
    os_memset(context, 0, sizeof(txProcessingContext_t));
    context->sha256 = sha256;
    context->dataSha256 = dataSha256;
    context->content = processingContent;
    context->state = TLV_CHAIN_ID;
    context->dataAllowed = dataAllowed;
    cx_sha256_init(context->sha256);
    cx_sha256_init(context->dataSha256);
}

uint8_t readTxByte(txProcessingContext_t *context) {
    uint8_t data;
    if (context->commandLength < 1) {
        PRINTF("readTxByte Underflow\n");
        THROW(EXCEPTION);
    }
    data = *context->workBuffer;
    context->workBuffer++;
    context->commandLength--;
    return data;
}

static void processHiveVote(txProcessingContext_t *context) {
    context->content->argumentCount = 4;
    strcpy(context->content->opName, "vote");
}

static void processHiveComment(txProcessingContext_t *context) {
    context->content->argumentCount = 7;
    strcpy(context->content->opName, "comment");
}

static void processHiveTransfer(txProcessingContext_t *context) {
    context->content->argumentCount = 4;
    strcpy(context->content->opName, "transfer");
}

static void processHiveTransferToVesting(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "transfer_to_vesting");
}

static void processHiveWithdrawVesting(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "withdraw_vesting");
}

static void processHiveLimitOrderCreate(txProcessingContext_t *context) {
    context->content->argumentCount = 6;
    strcpy(context->content->opName, "limit_order_create");
}

static void processHiveLimitOrderCancel(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "limit_order_cancel");
}

static void processHiveFeedPublish(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "feed_publish");
}

static void processHiveConvert(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "convert");
}

static void processHiveAccountCreate(txProcessingContext_t *context) {
    context->content->argumentCount = 8;
    strcpy(context->content->opName, "account_create");
}

static void processHiveAccountUpdate(txProcessingContext_t *context) {
    context->content->argumentCount = 6;
    strcpy(context->content->opName, "account_update");
}

static void processHiveWitnessUpdate(txProcessingContext_t *context) {
    context->content->argumentCount = 4;
    strcpy(context->content->opName, "witness_update");
}

static void processHiveAccountWitnessVote(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "account_witness_vote");
}

static void processHiveAccountWitnessProxy(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "account_witness_proxy");
}

static void processHiveDeleteComment(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "delete_comment");
}

static void processHiveCustomJson(txProcessingContext_t *context) {
    context->content->argumentCount = 4;
    strcpy(context->content->opName, "custom_json");
}

static void processHiveCommentOptions(txProcessingContext_t *context) {
    context->content->argumentCount = 7;
    strcpy(context->content->opName, "comment_options");
}

static void processHiveSetWithdrawVestingRoute(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "set_withdraw_vesting_route");
}

static void processHiveClaimAccount(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "claim_account");
}

static void processHiveCreateClaimedAccount(txProcessingContext_t *context) {
    context->content->argumentCount = 7;
    strcpy(context->content->opName, "create_claimed_account");
}

static void processHiveRequestAccountRecovery(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "request_account_recovery");
}

static void processHiveRecoverAccount(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "recover_account");
}

static void processHiveChangeRecoveryAccount(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "change_recovery_account");
}

static void processHiveTransferToSavings(txProcessingContext_t *context) {
    context->content->argumentCount = 4;
    strcpy(context->content->opName, "transfer_to_savings");
}

static void processHiveTransferFromSavings(txProcessingContext_t *context) {
    context->content->argumentCount = 5;
    strcpy(context->content->opName, "transfer_from_savings");
}

static void processHiveCancelTransferFromSavings(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "cancel_transfer_from_savings");
}

static void processHiveDeclineVotingRights(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "decline_voting_rights");
}

static void processHiveResetAccount(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "reset_account");
}

static void processHiveSetResetAccount(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "set_reset_account");
}

static void processHiveClaimRewardBalance(txProcessingContext_t *context) {
    context->content->argumentCount = 4;
    strcpy(context->content->opName, "claim_reward_balance");
}

static void processHiveDelegateVestingShares(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "delegate_vesting_shares");
}

static void processHiveCreateProposal(txProcessingContext_t *context) {
    context->content->argumentCount = 7;
    strcpy(context->content->opName, "create_proposal");
}

static void processHiveUpdateProposalVotes(txProcessingContext_t *context) {
    context->content->argumentCount = 3;
    strcpy(context->content->opName, "update_proposal_votes");
}

static void processHiveRemoveProposal(txProcessingContext_t *context) {
    context->content->argumentCount = 2;
    strcpy(context->content->opName, "remove_proposal");
}

void printArgument(uint8_t argNum, txProcessingContext_t *context) {
    uint8_t opType = context->content->opType;
    uint8_t *buffer = context->actionDataBuffer;
    uint32_t bufferLength = context->currentActionDataBufferLength;
    actionArgument_t *arg =  &context->content->arg;

    switch (opType) {
    case 0:
        parseHiveVote(buffer, bufferLength, argNum, arg);
        break;
    case 1:
        parseHiveComment(buffer, bufferLength, argNum, arg);
        break;
    case 2:
        parseHiveTransfer(buffer, bufferLength, argNum, arg);
        break;
    case 3:
        parseHiveTransferToVesting(buffer, bufferLength, argNum, arg);
        break;
    case 4:
        parseHiveWithdrawVesting(buffer, bufferLength, argNum, arg);
        break;
    case 5:
        parseHiveLimitOrderCreate(buffer, bufferLength, argNum, arg);
        break;
    case 6:
        parseHiveLimitOrderCancel(buffer, bufferLength, argNum, arg);
        break;
    case 7:
        parseHiveFeedPublish(buffer, bufferLength, argNum, arg);
        break;
    case 8:
        parseHiveConvert(buffer, bufferLength, argNum, arg);
        break;
    case 9:
        parseHiveAccountCreate(buffer, bufferLength, argNum, arg);
        break;
    case 10:
        parseHiveAccountUpdate(buffer, bufferLength, argNum, arg);
        break;
    case 11:
        parseHiveWitnessUpdate(buffer, bufferLength, argNum, arg);
        break;
    case 12:
        parseHiveAccountWitnessVote(buffer, bufferLength, argNum, arg);
        break;
    case 13:
        parseHiveAccountWitnessProxy(buffer, bufferLength, argNum, arg);
        break;
    case 17:
        parseHiveDeleteComment(buffer, bufferLength, argNum, arg);
        break;
    case 18:
        parseHiveCustomJson(buffer, bufferLength, argNum, arg);
        break;
    case 19:
        parseHiveCommentOptions(buffer, bufferLength, argNum, arg);
        break;
    case 20:
        parseHiveSetWithdrawVestingRoute(buffer, bufferLength, argNum, arg);
        break;
    case 22:
        parseHiveClaimAccount(buffer, bufferLength, argNum, arg);
        break;
    case 23:
        parseHiveCreateClaimedAccount(buffer, bufferLength, argNum, arg);
        break;
    case 24:
        parseHiveRequestAccountRecovery(buffer, bufferLength, argNum, arg);
        break;
    case 25:
        parseHiveRecoverAccount(buffer, bufferLength, argNum, arg);
        break;
    case 26:
        parseHiveChangeRecoveryAccount(buffer, bufferLength, argNum, arg);
        break;
    case 32:
        parseHiveTransferToSavings(buffer, bufferLength, argNum, arg);
        break;
    case 33:
        parseHiveTransferFromSavings(buffer, bufferLength, argNum, arg);
        break;
    case 34:
        parseHiveCancelTransferFromSavings(buffer, bufferLength, argNum, arg);
        break;
    case 36:
        parseHiveDeclineVotingRights(buffer, bufferLength, argNum, arg);
        break;
    case 37:
        parseHiveResetAccount(buffer, bufferLength, argNum, arg);
        break;
    case 38:
        parseHiveSetResetAccount(buffer, bufferLength, argNum, arg);
        break;
    case 39:
        parseHiveClaimRewardBalance(buffer, bufferLength, argNum, arg);
        break;
    case 40:
        parseHiveDelegateVestingShares(buffer, bufferLength, argNum, arg);
        break;
    case 44:
        parseHiveCreateProposal(buffer, bufferLength, argNum, arg);
        break;
    case 45:
        parseHiveUpdateProposalVotes(buffer, bufferLength, argNum, arg);
        break;
    case 46:
        parseHiveRemoveProposal(buffer, bufferLength, argNum, arg);
        break;
    default:
        THROW(EXCEPTION);
    }

    return;
}

/**
 * Sequentially hash an incoming data.
 * Hash functionality is moved out here in order to reduce 
 * dependencies on specific hash implementation.
*/
static void hashTxData(txProcessingContext_t *context, uint8_t *buffer, uint32_t length) {
    cx_hash(&context->sha256->header, 0, buffer, length, NULL, 0);
}

static void hashActionData(txProcessingContext_t *context, uint8_t *buffer, uint32_t length) {
    cx_hash(&context->dataSha256->header, 0, buffer, length, NULL, 0);
}

/**
 * Process all fields that do not requre any processing except hashing.
 * The data comes in by chucks, so it may happen that buffer may contain 
 * incomplete data for particular field. Function designed to process 
 * everything until it receives all data for a particular field 
 * and after that will move to next field.
*/
static void processField(txProcessingContext_t *context) {
    if (context->currentFieldPos < context->currentFieldLength) {
        uint32_t length = 
            (context->commandLength <
                     ((context->currentFieldLength - context->currentFieldPos))
                ? context->commandLength
                : context->currentFieldLength - context->currentFieldPos);

        hashTxData(context, context->workBuffer, length);

        context->workBuffer += length;
        context->commandLength -= length;
        context->currentFieldPos += length;
    }

    if (context->currentFieldPos == context->currentFieldLength) {
        context->state++;
        context->processingField = false;
    }
}

/**
 * Process Size fields that are expected to have Zero value. Except hashing the data, function
 * caches an incomming data. So, when all bytes for particulat field are received
 * do additional processing: Read actual number of actions encoded in buffer.
 * Throw exception if number is not '0'.
*/
static void processZeroSizeField(txProcessingContext_t *context) {
    if (context->currentFieldPos < context->currentFieldLength) {
        uint32_t length = 
            (context->commandLength <
                     ((context->currentFieldLength - context->currentFieldPos))
                ? context->commandLength
                : context->currentFieldLength - context->currentFieldPos);

        hashTxData(context, context->workBuffer, length);

        // Store data into a buffer
        os_memmove(context->sizeBuffer + context->currentFieldPos, context->workBuffer, length);

        context->workBuffer += length;
        context->commandLength -= length;
        context->currentFieldPos += length;
    }

    if (context->currentFieldPos == context->currentFieldLength) {
        uint32_t sizeValue = 0;
        unpack_variant32(context->sizeBuffer, context->currentFieldPos + 1, &sizeValue);
        if (sizeValue != 0) {
            PRINTF("zeroSizeField must be 0\n");
            THROW(EXCEPTION);
        }
        // Reset size buffer
        os_memset(context->sizeBuffer, 0, sizeof(context->sizeBuffer));

        // Move to next state
        context->state++;
        context->processingField = false;
    }
}

/**
 * Process Action Number Field. Except hashing the data, function
 * caches an incomming data. So, when all bytes for particulat field are received
 * do additional processing: Read actual number of actions encoded in buffer.
*/
static void processActionListSizeField(txProcessingContext_t *context) {
    if (context->currentFieldPos < context->currentFieldLength) {
        uint32_t length = 
            (context->commandLength <
                     ((context->currentFieldLength - context->currentFieldPos))
                ? context->commandLength
                : context->currentFieldLength - context->currentFieldPos);

        hashTxData(context, context->workBuffer, length);

        // Store data into a buffer
        os_memmove(context->sizeBuffer + context->currentFieldPos, context->workBuffer, length);

        context->workBuffer += length;
        context->commandLength -= length;
        context->currentFieldPos += length;
    }

    if (context->currentFieldPos == context->currentFieldLength) {
        unpack_variant32(context->sizeBuffer, context->currentFieldPos + 1, &context->numOperations);
        context->currentOpIndex = 0;
        
        // Reset size buffer
        os_memset(context->sizeBuffer, 0, sizeof(context->sizeBuffer));

        context->state++;
        context->processingField = false;
        // if (context->numOperations > 1) {
        //     context->confirmProcessing = true;
        // }
    }
}

/**
 * Process current action data field and store in into data buffer.
*/
static void processActionData(txProcessingContext_t *context) {
    if (context->currentFieldLength > sizeof(context->actionDataBuffer) - 1) {
        PRINTF("processActionData data overflow\n");
        THROW(EXCEPTION);
    }

    if (context->currentFieldPos < context->currentFieldLength) {
        uint32_t length = 
            (context->commandLength <
                     ((context->currentFieldLength - context->currentFieldPos))
                ? context->commandLength
                : context->currentFieldLength - context->currentFieldPos);

        hashTxData(context, context->workBuffer, length);
        hashActionData(context, context->workBuffer, length);
        os_memmove(context->actionDataBuffer + context->currentFieldPos, context->workBuffer, length);
        if(context->currentFieldPos == 0) {
            os_memmove(&context->content->opType, context->workBuffer, sizeof(uint8_t));
        }

        context->workBuffer += length;
        context->commandLength -= length;
        context->currentFieldPos += length;
    }

    if (context->currentFieldPos == context->currentFieldLength) {
        context->currentActionDataBufferLength = context->currentFieldLength;

        switch(context->content->opType) {
            case 0:
                processHiveVote(context);
                break;
            case 1:
                processHiveComment(context);
                break;
            case 2:
                processHiveTransfer(context);
                break;
            case 3:
                processHiveTransferToVesting(context);
                break;
            case 4:
                processHiveWithdrawVesting(context);
                break;
            case 5:
                processHiveLimitOrderCreate(context);
                break;
            case 6:
                processHiveLimitOrderCancel(context);
                break;
            case 7:
                processHiveFeedPublish(context);
                break;
            case 8:
                processHiveConvert(context);
                break;
            case 9:
                processHiveAccountCreate(context);
                break;
            case 10:
                processHiveAccountUpdate(context);
                break;
            case 11:
                processHiveWitnessUpdate(context);
                break;
            case 12:
                processHiveAccountWitnessVote(context);
                break;
            case 13:
                processHiveAccountWitnessProxy(context);
                break;
            case 17:
                processHiveDeleteComment(context);
                break;
            case 18:
                processHiveCustomJson(context);
                break;
            case 19:
                processHiveCommentOptions(context);
                break;
            case 20:
                processHiveSetWithdrawVestingRoute(context);
                break;
            case 22:
                processHiveClaimAccount(context);
                break;
            case 23:
                processHiveCreateClaimedAccount(context);
                break;
            case 24:
                processHiveRequestAccountRecovery(context);
                break;
            case 25:
                processHiveRecoverAccount(context);
                break;
            case 26:
                processHiveChangeRecoveryAccount(context);
                break;
            case 32:
                processHiveTransferToSavings(context);
                break;
            case 33:
                processHiveTransferFromSavings(context);
                break;
            case 34:
                processHiveCancelTransferFromSavings(context);
                break;
            case 36:
                processHiveDeclineVotingRights(context);
                break;
            case 37:
                processHiveResetAccount(context);
                break;
            case 38:
                processHiveSetResetAccount(context);
                break;
            case 39:
                processHiveClaimRewardBalance(context);
                break;
            case 40:
                processHiveDelegateVestingShares(context);
                break;
            case 44:
                processHiveCreateProposal(context);
                break;
            case 45:
                processHiveUpdateProposalVotes(context);
                break;
            case 46:
                processHiveRemoveProposal(context);
                break;
            default:
                PRINTF("unknown action");
                THROW(EXCEPTION);
        }

        if (++context->currentOpIndex >= context->numOperations) {
            context->state = TLV_TX_EXTENSION_LIST_SIZE;
        }
        
        context->processingField = false;
        context->actionReady = true;
    }
}

static parserStatus_e processTxInternal(txProcessingContext_t *context) {
    for(;;) {
        if (context->confirmProcessing) {
            context->confirmProcessing = false;
            return STREAM_CONFIRM_PROCESSING;
        }
        if (context->actionReady) {
            context->actionReady = false;
            return STREAM_ACTION_READY;
        }
        if (context->state == TLV_DONE) {
            return STREAM_FINISHED;
        }
        if (context->commandLength == 0) {
            return STREAM_PROCESSING;
        }
        if (!context->processingField) {
            // While we are not processing a field, we should TLV parameters
            bool decoded = false;
            while (context->commandLength != 0) {
                bool valid;
                // Feed the TLV buffer until the length can be decoded
                context->tlvBuffer[context->tlvBufferPos++] =
                    readTxByte(context);

                decoded = tlvTryDecode(context->tlvBuffer, context->tlvBufferPos, 
                    &context->currentFieldLength, &valid);

                if (!valid) {
                    PRINTF("TLV decoding error\n");
                    return STREAM_FAULT;
                }
                if (decoded) {
                    break;
                }

                // Cannot decode yet
                // Sanity check
                if (context->tlvBufferPos == sizeof(context->tlvBuffer)) {
                    PRINTF("TLV pre-decode logic error\n");
                    return STREAM_FAULT;
                }
            }
            if (!decoded) {
                return STREAM_PROCESSING;
            }
            context->currentFieldPos = 0;
            context->tlvBufferPos = 0;
            context->processingField = true;
        }
        switch (context->state) {
        case TLV_CHAIN_ID:
        case TLV_HEADER_REF_BLOCK_NUM:
        case TLV_HEADER_REF_BLOCK_PREFIX:
        case TLV_HEADER_EXPITATION:
            processField(context);
            break;

        case TLV_OPERATION_LIST_SIZE:
            processActionListSizeField(context);
            break;

        case TLV_OPERATION_DATA:
            processActionData(context);
            break;

        // TODO: implement extensions processing
        // Throw an error if the extensions field is not zero length `[]`
        case TLV_TX_EXTENSION_LIST_SIZE:
            processZeroSizeField(context);
            break;

        default:
            PRINTF("Invalid TLV decoder context\n");
            return STREAM_FAULT;
        }
    }
}

/**
 * Transaction processing should be done in a most efficient
 * way as possible, as Hive transaction size isn't fixed
 * and depends on action size. 
 * Also, Ledger Nano S have limited RAM resource, so data caching
 * could be very expencive. Due to these features and limitations
 * only some fields are cached before processing. 
 * All data is encoded by DER.ASN1 rules in plain way and serialized as a flat map.
 * 
 * Flat map is used in order to avoid nesting complexity.
 * 
 * Buffer serialization example:
 * [chain id][header][action number (1)][action 0][...]
 * Each field is encoded by DER rules.
 * Chain id field will be encoded next way:
 *  [Tag][Length][Value]
 * [0x04][ 0x20 ][chain id as octet string]
 * 
 * More infomation about DER Tag Length Value encoding is here: http://luca.ntop.org/Teaching/Appunti/asn1.html.
 * Only octet tag number is allowed. 
 * Value is encoded as octet string.
 * The length of the string is stored in Length byte(s)
 * 
 * Detailed flat map representation of incoming data:
 * [CHAIN ID][HEADER][CTX_FREE_ACTION_NUMBER][ACTION_NUMBER][ACTION 0][TX_EXTENSION_NUMBER][CTX_FREE_ACTION_DATA_NUMBER]
 * 
 * CHAIN ID:
 * [32 BYTES]
 * 
 * HEADER size may vary due to MAX_NET_USAGE_WORDS and DELAY_SEC serialization:
 * [EXPIRATION][REF_BLOCK_NUM][REF_BLOCK_PREFIX][MAX_NET_USAGE_WORDS][MAX_CPU_USAGE_MS][DELAY_SEC]
 * 
 * CTX_FREE_ACTION_NUMBER theoretically is not fixed due to serialization. Ledger accepts only 0 as encoded value.
 * ACTION_NUMBER theoretically is not fixed due to serialization.
 * 
 * ACTION size may vary as authorization list and action data is dynamic:
 * [ACCOUNT][NAME][AUTHORIZATION_NUMBER][AUTHORIZATION 0][AUTHORIZATION 1]..[AUTHORIZATION N][ACTION_DATA]
 * ACCOUNT and NAME are 8 bytes long, both.
 * AUTHORIZATION_NUMBER theoretically is not fixed due to serialization.
 * ACTION_DATA is octet string of bytes.
 *  
 * AUTHORIZATION is 16 bytes long:
 * [ACTOR][PERMISSION]
 * ACTOR and PERMISSION are 8 bites long, both.
 * 
 * TX_EXTENSION_NUMBER theoretically is not fixed due to serialization. Ledger accepts only 0 as encoded value.
 * CTX_FREE_ACTION_DATA_NUMBER theoretically is not fixed due to serialization. Ledger accepts only 0 as encoded value.
*/
parserStatus_e parseTx(txProcessingContext_t *context, uint8_t *buffer, uint32_t length) {
    parserStatus_e result;
#ifdef DEBUG_APP
    // Do not catch exceptions.
    context->workBuffer = buffer;
    context->commandLength = length;
    result = processTxInternal(context);
#else
    BEGIN_TRY {
        TRY {
            if (context->commandLength == 0) {
                context->workBuffer = buffer;
                context->commandLength = length;
            }
            result = processTxInternal(context);
        }
        CATCH_OTHER(e) {
            result = STREAM_FAULT;
        }
        FINALLY {
        }
    }
    END_TRY;
#endif
    return result;
}
