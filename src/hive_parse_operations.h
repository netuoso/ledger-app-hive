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

#ifndef __HIVE_PARSE_OPERATIONS_H__
#define __HIVE_PARSE_OPERATIONS_H__

#include "hive_parse.h"

void parseHiveVote(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveComment(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveTransfer(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveTransferToVesting(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveWithdrawVesting(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveLimitOrderCreate(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveLimitOrderCancel(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveFeedPublish(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveConvert(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveAccountCreate(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveAccountUpdate(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveWitnessUpdate(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveAccountWitnessVote(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveAccountWitnessProxy(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveDeleteComment(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveCustomJson(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveCommentOptions(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveSetWithdrawVestingRoute(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveClaimAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveCreateClaimedAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveRequestAccountRecovery(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveRecoverAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveChangeRecoveryAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveTransferToSavings(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveTransferFromSavings(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveCancelTransferFromSavings(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveDeclineVotingRights(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveResetAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveSetResetAccount(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveClaimRewardBalance(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveDelegateVestingShares(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveCreateProposal(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveUpdateProposalVotes(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);
void parseHiveRemoveProposal(uint8_t *buffer, uint32_t bufferLength, uint8_t argNum, actionArgument_t *arg);

#endif
