/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/system.h>
#include <string>
#include <list>

using namespace std;
using namespace eosio;
using eosio::indexed_by;
using eosio::const_mem_fun;

#define CONTRACT_STATE_CONFIG_ID        0
#define BANDWIDTH_RATE_CONFIG_ID        1
#define REGISTRATION_REWARD_CONFIG_ID   2

#define DEFAULT_BANDWIDTH_RATE          1024   // 1024KB per token
#define DEFAULT_REGISTRATION_REWARD     0

namespace ringleai {

    enum class ContractState {
        Normal = 0,
        Maintaining = 1,
    };

    class config_manager {
    public:
        config_manager(account_name n) : _self(n) {}
        
        void clear(uint32_t count) {
            config_table configs(_self, _self);
            for(auto iter = configs.begin(); iter != configs.end() && count > 0; count--)
                iter = configs.erase(iter);
        }

        void checkContractState() {
            uint64_t contractState = getUint64Config(CONTRACT_STATE_CONFIG_ID, (uint64_t)ContractState::Normal);
            eosio_assert(contractState == (uint64_t)ContractState::Normal, "contract maintaining");
        }

        void setBandwidthRate(account_name payer, uint32_t bandwidthRate) {
            setUint64Config(payer, BANDWIDTH_RATE_CONFIG_ID, bandwidthRate);
        }

        uint32_t getBandwidthRate() {
            return (uint32_t)getUint64Config(BANDWIDTH_RATE_CONFIG_ID, DEFAULT_BANDWIDTH_RATE);
        }

        void setRegistrationReward(account_name payer, uint32_t reward) {
            setUint64Config(payer, REGISTRATION_REWARD_CONFIG_ID, reward);
        }

        uint32_t getRegistrationReward() {
            return (uint32_t)getUint64Config(REGISTRATION_REWARD_CONFIG_ID, DEFAULT_REGISTRATION_REWARD);
        }

    private:
        account_name _self;

        void setUint64Config(account_name payer, uint64_t id, uint64_t value) {
            config_table configs(_self, _self);
            auto config_lookup = configs.find(id);

            if (config_lookup == configs.end()) {
                configs.emplace(payer, [&](auto& config) {
                    config.id = id;
                    config.iData = value;
                });
            } else {
                configs.modify(config_lookup, payer, [&](auto& config) {
                    config.iData = value;
                });
            }
        }

        uint64_t getUint64Config(uint64_t id, uint64_t defaultValue) {
            config_table configs(_self, _self);
            auto config_lookup = configs.find(id);

            if (config_lookup == configs.end()) {
                return defaultValue;
            }

            return (*config_lookup).iData;
        }

        // @abi table configStruct i64
        struct configStruct {
            uint64_t id;
            uint64_t iData;
            string  sData;

            uint64_t primary_key() const {return id;}

            EOSLIB_SERIALIZE(configStruct, (id)(iData)(sData))
        };
        typedef multi_index<N(config), configStruct> config_table;
    };
}