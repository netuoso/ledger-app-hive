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

from asn1 import Encoder, Numbers
from datetime import datetime
import time
import datetime
from calendar import timegm
import struct
from binascii import unhexlify, hexlify
from base58 import b58decode
import hashlib

class Operation:
    def __init__(self):
        pass

    @staticmethod
    def types():
        operations = {}
        operations["vote"] = 0
        operations["comment"] = 1
        operations["transfer"] = 2
        operations["transfer_to_vesting"] = 3
        operations["withdraw_vesting"] = 4
        operations["limit_order_create"] = 5
        operations["limit_order_cancel"] = 6
        operations["feed_publish"] = 7
        operations["convert"] = 8
        operations["account_create"] = 9
        operations["account_update"] = 10
        operations["witness_update"] = 11
        operations["account_witness_vote"] = 12
        operations["account_witness_proxy"] = 13
        operations["delete_comment"] = 17
        operations["custom_json"] = 18
        operations["comment_options"] = 19
        operations["set_withdraw_vesting_route"] = 20
        operations["claim_account"] = 22
        operations["create_claimed_account"] = 23
        operations["request_account_recovery"] = 24
        operations["recover_account"] = 25
        operations["change_recovery_account"] = 26
        operations["escrow_transfer"] = 27
        operations["escrow_dispute"] = 28
        operations["escrow_release"] = 29
        operations["escrow_approve"] = 31
        operations["transfer_to_savings"] = 32
        operations["transfer_from_savings"] = 33
        operations["cancel_transfer_from_savings"] = 34
        operations["custom_binary"] = 35
        operations["decline_voting_rights"] = 36
        operations["reset_account"] = 37
        operations["set_reset_account"] = 38
        operations["claim_reward_balance"] = 39
        operations["delegate_vesting_shares"] = 40
        operations["witness_set_properties"] = 42
        operations["account_update2"] = 43
        operations["create_proposal"] = 44
        operations["update_proposal_votes"] = 45
        operations["remove_proposal"] = 46
        return operations

class Transaction:
    def __init__(self):
        pass

    @staticmethod
    def symbol_precision(sym):
        mappings = {
            "HIVE": 3,
            "HBD": 3,
            "STEEM": 3,
            "SBD": 3,
            "TBD": 3,
            "TESTS": 3,
            "VESTS": 6
        }
        return mappings[sym];

    @staticmethod
    def parse_asset(asset):
        amount, symbol = asset.split(' ')
        precision = Transaction.symbol_precision(symbol)
        amount = round(float(amount) * pow(10, precision))

        result = hexlify(struct.pack("<q", amount))
        result += hexlify(struct.pack("<b", precision))

        result += hexlify(symbol)

        padlength = 7 - len(symbol)
        if padlength > 0:
            result += "00" * padlength

        return unhexlify(result)


    @staticmethod
    def pack_fc_uint(value):
        out = ''
        val = value
        while True:
            b = val & 0x7f
            val >>= 7
            b |= ((val > 0) << 7)
            out += chr(b)

            if val == 0:
                break

        return out

    @staticmethod
    def parse_public_key(data):
        data = str(data[3:])
        decoded = b58decode(data)
        decoded = decoded[:-4]
        return decoded

    @staticmethod
    def parse_vote(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["vote"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['voter'])) + data['voter'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['author'])) + data['author'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['permlink'])) + data['permlink'])
        parameters += hexlify(struct.pack("<h", int(data["weight"])))

        return unhexlify(parameters)

    @staticmethod
    def parse_comment(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["comment"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['parent_author'])) + data['parent_author'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['parent_permlink'])) + data['parent_permlink'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['author'])) + data['author'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['permlink'])) + data['permlink'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['title'])) + data['title'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['body'])) + data['body'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['json_metadata'])) + data['json_metadata'])

        return unhexlify(parameters)

    @staticmethod
    def parse_transfer(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["transfer"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['from'])) + data['from'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['to'])) + data['to'])
        parameters += hexlify(Transaction.parse_asset(data["amount"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['memo'])))
        if len(data['memo']) > 0:
            parameters += hexlify(data['memo'])

        return unhexlify(parameters)

    @staticmethod
    def parse_transfer_to_vesting(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["transfer_to_vesting"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['from'])) + data['from'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['to'])) + data['to'])
        parameters += hexlify(Transaction.parse_asset(data["amount"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_withdraw_vesting(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["withdraw_vesting"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account'])) + data['account'])
        parameters += hexlify(Transaction.parse_asset(data["vesting_shares"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_limit_order_create(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["limit_order_create"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['owner'])) + data['owner'])
        parameters += hexlify(struct.pack("<I", data['orderid']))
        parameters += hexlify(Transaction.parse_asset(data["amount_to_sell"]))
        parameters += hexlify(Transaction.parse_asset(data["min_to_receive"]))
        parameters += "01" if data['fill_or_kill'] else "00"
        timestamp = int(datetime.datetime.strptime(data['expiration']+'Z', "%Y-%m-%dT%H:%M:%SZ").strftime("%s"))
        parameters += hexlify(struct.pack("<I", timestamp))

        return unhexlify(parameters)

    @staticmethod
    def parse_limit_order_cancel(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["limit_order_cancel"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['owner'])) + data['owner'])
        parameters += hexlify(struct.pack("<I", data['orderid']))

        return unhexlify(parameters)

    @staticmethod
    def parse_feed_publish(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["feed_publish"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['publisher'])) + data['publisher'])
        for item in data['exchange_rate'].items():
            parameters += hexlify(Transaction.parse_asset(item[1]))

        return unhexlify(parameters)

    @staticmethod
    def parse_convert(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["convert"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['owner'])) + data['owner'])
        parameters += hexlify(struct.pack("<I", data['requestid']))
        parameters += hexlify(Transaction.parse_asset(data["amount"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_account_create(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["account_create"]))
        parameters += hexlify(Transaction.parse_asset(data["fee"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['creator'])) + data['creator'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['new_account_name'])) + data['new_account_name'])
        parameters += hexlify(struct.pack("<L", data['owner']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['owner']['account_auths'])))
        for item in data['owner']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['owner']['key_auths'])))
        for item in data['owner']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", data['active']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['active']['account_auths'])))
        for item in data['active']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['active']['key_auths'])))
        for item in data['active']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", data['posting']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['posting']['account_auths'])))
        for item in data['posting']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['posting']['key_auths'])))
        for item in data['posting']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(Transaction.parse_public_key(data["memo_key"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_account_update(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["account_update"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account'])) + data['account'])
        parameters += hexlify(struct.pack("<L", data['owner']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['owner']['account_auths'])))
        for item in data['owner']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['owner']['key_auths'])))
        for item in data['owner']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", data['active']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['active']['account_auths'])))
        for item in data['active']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['active']['key_auths'])))
        for item in data['active']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", data['posting']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['posting']['account_auths'])))
        for item in data['posting']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['posting']['key_auths'])))
        for item in data['posting']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(Transaction.parse_public_key(data["memo_key"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_witness_update(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["witness_update"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['owner'])) + data['owner'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['url'])) + data['url'])
        parameters += hexlify(Transaction.parse_public_key(data['block_signing_key']))
        for item in data['props'].items():
            if item[0] == "account_creation_fee":
                parameters += hexlify(Transaction.parse_asset(item[1]))
            elif item[0] == "maximum_block_size":
                parameters += hexlify(struct.pack("<L", item[1]))
            elif item[0] == "sbd_interest_rate":
                parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(Transaction.parse_asset(data["fee"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_account_witness_vote(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["account_witness_vote"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account'])) + data['account'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['witness'])) + data['witness'])
        parameters += "01" if data['approve'] else "00"

        return unhexlify(parameters)

    @staticmethod
    def parse_account_witness_proxy(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["account_witness_proxy"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account'])) + data['account'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['proxy'])) + data['proxy'])

        return unhexlify(parameters)

    @staticmethod
    def parse_delete_comment(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["delete_comment"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['author'])) + data['author'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['permlink'])) + data['permlink'])

        return unhexlify(parameters)

    @staticmethod
    def parse_custom_json(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["custom_json"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['required_auths'])))
        for item in data["required_auths"]:
            parameters += hexlify(Transaction.pack_fc_uint(len(item)) + item)
        parameters += hexlify(Transaction.pack_fc_uint(len(data['required_posting_auths'])))
        for item in data["required_posting_auths"]:
            parameters += hexlify(Transaction.pack_fc_uint(len(item)) + item)
        parameters += hexlify(Transaction.pack_fc_uint(len(data['id'])) + data['id'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['json'])) + data['json'])

        return unhexlify(parameters)

    @staticmethod
    def parse_comment_options(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["comment_options"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['author'])) + data['author'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['permlink'])) + data['permlink'])
        parameters += hexlify(Transaction.parse_asset(data["max_accepted_payout"]))
        parameters += hexlify(struct.pack("<h", int(data["percent_steem_dollars"])))
        parameters += "01" if data['allow_votes'] else "00"
        parameters += "01" if data['allow_curation_rewards'] else "00"
        parameters += hexlify(struct.pack("<h", len(data.get("extensions", []))))
        for item in data['extensions']:
            if item[0] != 0:
                raise "extension type not implemented"
            parameters += hexlify(Transaction.pack_fc_uint(len(item[1]['beneficiaries'])))
            for beneficiary in item[1]['beneficiaries']:
                parameters += hexlify(Transaction.pack_fc_uint(len(beneficiary['account'])) + beneficiary['account'])
                parameters += hexlify(struct.pack("<h", int(beneficiary["weight"])))

        return unhexlify(parameters)

    @staticmethod
    def parse_set_withdraw_vesting_route(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["set_withdraw_vesting_route"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['from_account'])) + data['from_account'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['to_account'])) + data['to_account'])
        parameters += hexlify(struct.pack("<h", int(data["percent"])))
        parameters += "01" if data['autovest'] else "00"

        return unhexlify(parameters)

    @staticmethod
    def parse_claim_account(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["claim_account"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['creator'])) + data['creator'])
        parameters += hexlify(Transaction.parse_asset(data["fee"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_create_claimed_account(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["create_claimed_account"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['creator'])) + data['creator'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['new_account_name'])) + data['new_account_name'])
        parameters += hexlify(struct.pack("<L", data['owner']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['owner']['account_auths'])))
        for item in data['owner']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['owner']['key_auths'])))
        for item in data['owner']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", data['active']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['active']['account_auths'])))
        for item in data['active']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['active']['key_auths'])))
        for item in data['active']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", data['posting']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['posting']['account_auths'])))
        for item in data['posting']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['posting']['key_auths'])))
        for item in data['posting']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(Transaction.parse_public_key(data["memo_key"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['json_metadata'])) + data['json_metadata'])
        return unhexlify(parameters)

    @staticmethod
    def parse_request_account_recovery(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["request_account_recovery"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['recovery_account'])) + data['recovery_account'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account_to_recover'])) + data['account_to_recover'])
        parameters += hexlify(struct.pack("<L", data['new_owner_authority']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['new_owner_authority']['account_auths'])))
        for item in data['new_owner_authority']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['new_owner_authority']['key_auths'])))
        for item in data['new_owner_authority']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", len(data.get('extensions', []))))
        return unhexlify(parameters)

    @staticmethod
    def parse_recover_account(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["recover_account"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account_to_recover'])) + data['account_to_recover'])
        parameters += hexlify(struct.pack("<L", data['new_owner_authority']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['new_owner_authority']['account_auths'])))
        for item in data['new_owner_authority']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['new_owner_authority']['key_auths'])))
        for item in data['new_owner_authority']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", data['recent_owner_authority']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['recent_owner_authority']['account_auths'])))
        for item in data['recent_owner_authority']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['recent_owner_authority']['key_auths'])))
        for item in data['recent_owner_authority']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<L", len(data.get('extensions', []))))
        return unhexlify(parameters)

    @staticmethod
    def parse_change_recovery_account(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["change_recovery_account"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account_to_recover'])) + data['account_to_recover'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['new_recovery_account'])) + data['new_recovery_account'])
        parameters += hexlify(struct.pack("<L", len(data.get('extensions', []))))
        return unhexlify(parameters)

    @staticmethod
    def parse_transfer_to_savings(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["transfer_to_savings"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['from'])) + data['from'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['to'])) + data['to'])
        parameters += hexlify(Transaction.parse_asset(data["amount"]))

        parameters += hexlify(Transaction.pack_fc_uint(len(data['memo'])))
        if len(data['memo']) > 0:
            parameters += hexlify(data['memo'])

        return unhexlify(parameters)

    @staticmethod
    def parse_transfer_from_savings(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["transfer_from_savings"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['from'])) + data['from'])
        parameters += hexlify(struct.pack("<L", data['request_id']))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['to'])) + data['to'])
        parameters += hexlify(Transaction.parse_asset(data["amount"]))

        parameters += hexlify(Transaction.pack_fc_uint(len(data['memo'])))
        if len(data['memo']) > 0:
            parameters += hexlify(data['memo'])

        return unhexlify(parameters)

    @staticmethod
    def parse_cancel_transfer_from_savings(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["cancel_transfer_from_savings"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['from'])) + data['from'])
        parameters += hexlify(struct.pack("<L", data['request_id']))

        return unhexlify(parameters)

    @staticmethod
    def parse_decline_voting_rights(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["decline_voting_rights"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account'])) + data['account'])
        parameters += "01" if data['decline'] else "00"

        return unhexlify(parameters)

    @staticmethod
    def parse_reset_account(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["reset_account"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['reset_account'])) + data['reset_account'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account_to_reset_account'])) + data['account_to_reset_account'])
        parameters += hexlify(struct.pack("<L", data['new_owner_authority']['weight_threshold']))
        parameters += hexlify(struct.pack("<b", len(data['new_owner_authority']['account_auths'])))
        for item in data['new_owner_authority']['account_auths']:
            parameters += hexlify(Transaction.pack_fc_uint(len(item[0])) + item[0])
            parameters += hexlify(struct.pack("<H", item[1]))
        parameters += hexlify(struct.pack("<b", len(data['new_owner_authority']['key_auths'])))
        for item in data['new_owner_authority']['key_auths']:
            parameters += hexlify(Transaction.parse_public_key(item[0]))
            parameters += hexlify(struct.pack("<H", item[1]))

        return unhexlify(parameters)

    @staticmethod
    def parse_set_reset_account(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["set_reset_account"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account'])) + data['account'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['current_reset_account'])) + data['current_reset_account']) 
        parameters += hexlify(Transaction.pack_fc_uint(len(data['reset_account'])) + data['reset_account']) 

        return unhexlify(parameters)

    @staticmethod
    def parse_claim_reward_balance(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["claim_reward_balance"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['account'])) + data['account'])
        parameters += hexlify(Transaction.parse_asset(data["reward_steem"]))
        parameters += hexlify(Transaction.parse_asset(data["reward_sbd"]))
        parameters += hexlify(Transaction.parse_asset(data["reward_vests"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_delegate_vesting_shares(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["delegate_vesting_shares"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['delegator'])) + data['delegator'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['delegatee'])) + data['delegatee'])
        parameters += hexlify(Transaction.parse_asset(data["vesting_shares"]))

        return unhexlify(parameters)

    @staticmethod
    def parse_create_proposal(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["create_proposal"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['creator'])) + data['creator'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['receiver'])) + data['receiver'])
        startdate = int(datetime.datetime.strptime(data['start_date']+'Z', "%Y-%m-%dT%H:%M:%SZ").strftime("%s"))
        parameters += hexlify(struct.pack("<I", startdate))
        enddate = int(datetime.datetime.strptime(data['end_date']+'Z', "%Y-%m-%dT%H:%M:%SZ").strftime("%s"))
        parameters += hexlify(struct.pack("<I", enddate))
        parameters += hexlify(Transaction.parse_asset(data["daily_pay"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['subject'])) + data['subject'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['permlink'])) + data['permlink'])
        parameters += hexlify(struct.pack("<L", len(data.get('extensions', []))))

        return unhexlify(parameters)

    @staticmethod
    def parse_update_proposal_votes(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["update_proposal_votes"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['voter'])) + data['voter'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['proposal_ids'])))
        for item in data["proposal_ids"]:
            parameters += hexlify(struct.pack("<q", item))
        parameters += "01" if data['approve'] else "00"
        parameters += hexlify(struct.pack("<L", len(data.get('extensions', []))))

        return unhexlify(parameters)

    @staticmethod
    def parse_remove_proposal(data):
        parameters = hexlify(Transaction.pack_fc_uint(Operation.types()["remove_proposal"]))
        parameters += hexlify(Transaction.pack_fc_uint(len(data['proposal_owner'])) + data['proposal_owner'])
        parameters += hexlify(Transaction.pack_fc_uint(len(data['proposal_ids'])))
        for item in data["proposal_ids"]:
            parameters += hexlify(struct.pack("<q", item))
        parameters += hexlify(struct.pack("<L", len(data.get('extensions', []))))

        return unhexlify(parameters)

    @staticmethod
    def parse(json):
        tx = Transaction()
        tx.json = json

        # https://peakd.com/steem/@xeroc/steem-transaction-signing-in-a-nutshell

        tx.chain_id = unhexlify("0000000000000000000000000000000000000000000000000000000000000000")

        body = json

        tx.ref_block_num = struct.pack('<H', body['ref_block_num'])
        tx.ref_block_prefix = struct.pack('<I', body['ref_block_prefix'])

        timeformat = "%Y-%m-%dT%H:%M:%S%Z"
        tx.expiration = struct.pack("<I", timegm(time.strptime((body["expiration"] + "UTC"), timeformat)))

        tx.operations_count = Transaction.pack_fc_uint(len(body['operations']))

        tx.operations = []
        for operation in body['operations']:
            op = Operation()

            data = operation[1]
            if operation[0] == 'vote':
                parameters = Transaction.parse_vote(data)
            elif operation[0] == 'comment':
                parameters = Transaction.parse_comment(data)
            elif operation[0] == 'transfer':
                parameters = Transaction.parse_transfer(data)
            elif operation[0] == 'transfer_to_vesting':
                parameters = Transaction.parse_transfer_to_vesting(data)
            elif operation[0] == 'withdraw_vesting':
                parameters = Transaction.parse_withdraw_vesting(data)
            elif operation[0] == 'limit_order_create':
                parameters = Transaction.parse_limit_order_create(data)
            elif operation[0] == 'limit_order_cancel':
                parameters = Transaction.parse_limit_order_cancel(data)
            elif operation[0] == 'feed_publish':
                parameters = Transaction.parse_feed_publish(data)
            elif operation[0] == 'convert':
                parameters = Transaction.parse_convert(data)
            elif operation[0] == 'account_create':
                parameters = Transaction.parse_account_create(data)
            elif operation[0] == 'account_update':
                parameters = Transaction.parse_account_update(data)
            elif operation[0] == 'witness_update':
                parameters = Transaction.parse_witness_update(data)
            elif operation[0] == 'account_witness_vote':
                parameters = Transaction.parse_account_witness_vote(data)
            elif operation[0] == 'account_witness_proxy':
                parameters = Transaction.parse_account_witness_proxy(data)
            elif operation[0] == 'delete_comment':
                parameters = Transaction.parse_delete_comment(data)
            elif operation[0] == 'custom_json':
                parameters = Transaction.parse_custom_json(data)
            elif operation[0] == 'comment_options':
                parameters = Transaction.parse_comment_options(data)
            elif operation[0] == 'set_withdraw_vesting_route':
                parameters = Transaction.parse_set_withdraw_vesting_route(data)
            elif operation[0] == 'claim_account':
                parameters = Transaction.parse_claim_account(data)
            elif operation[0] == 'create_claimed_account':
                parameters = Transaction.parse_create_claimed_account(data)
            elif operation[0] == 'request_account_recovery':
                parameters = Transaction.parse_request_account_recovery(data)
            elif operation[0] == 'recover_account':
                parameters = Transaction.parse_recover_account(data)
            elif operation[0] == 'change_recovery_account':
                parameters = Transaction.parse_change_recovery_account(data)
            elif operation[0] == 'transfer_to_savings':
                parameters = Transaction.parse_transfer_to_savings(data)
            elif operation[0] == 'transfer_from_savings':
                parameters = Transaction.parse_transfer_from_savings(data)
            elif operation[0] == 'cancel_transfer_from_savings':
                parameters = Transaction.parse_cancel_transfer_from_savings(data)
            elif operation[0] == 'decline_voting_rights':
                parameters = Transaction.parse_decline_voting_rights(data)
            elif operation[0] == 'reset_account':
                parameters = Transaction.parse_reset_account(data)
            elif operation[0] == 'set_reset_account':
                parameters = Transaction.parse_set_reset_account(data)
            elif operation[0] == 'claim_reward_balance':
                parameters = Transaction.parse_claim_reward_balance(data)
            elif operation[0] == 'delegate_vesting_shares':
                parameters = Transaction.parse_delegate_vesting_shares(data)
            elif operation[0] == 'create_proposal':
                parameters = Transaction.parse_create_proposal(data)
            elif operation[0] == 'update_proposal_votes':
                parameters = Transaction.parse_update_proposal_votes(data)
            elif operation[0] == 'remove_proposal':
                parameters = Transaction.parse_remove_proposal(data)
            else:
                print("operation type not implemented")
                exit()

            op.data = parameters

            tx.operations.append(op)

        for operation in tx.operations:
            sha = hashlib.sha256()
            sha.update(operation.data)
            print 'Argument checksum ' + sha.hexdigest()


        tx.extensions_size = Transaction.pack_fc_uint(len(body.get('extensions', [])))

        return tx

    def encode(self):
        encoder = Encoder()
        sha = hashlib.sha256()

        sha.update(self.chain_id)
        sha.update(self.ref_block_num)
        sha.update(self.ref_block_prefix)
        sha.update(self.expiration)
        sha.update(self.operations_count)
        for operation in self.operations:
            sha.update(operation.data)
        sha.update(self.extensions_size)

        print 'Signing digest ' + sha.hexdigest()

        encoder.start()
        encoder.write(self.chain_id, Numbers.OctetString)
        encoder.write(self.ref_block_num, Numbers.OctetString)
        encoder.write(self.ref_block_prefix, Numbers.OctetString)
        encoder.write(self.expiration, Numbers.OctetString)

        encoder.write(self.operations_count, Numbers.OctetString)
        for operation in self.operations:
            encoder.write(operation.data, Numbers.OctetString)
        encoder.write(self.extensions_size, Numbers.OctetString)

        return encoder.output()
