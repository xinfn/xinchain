/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/system.h>
#include <string>
#include <list>
#include "config_manager.hpp"

using namespace std;
using namespace eosio;
using eosio::indexed_by;
using eosio::const_mem_fun;

#define XINCHAIN N(xinchain111)
#define OFGP N(xinchain111)

namespace ringleai {

    enum class AccountState {
        Normal = 0,
        Locked = 1,
    };

    enum class VideoState {
        Normal = 0,
        Locked = 1,
        Deleted = 2,
    };

    enum class OrderType {
        Video = 0,
        Deposit = 1,
        Withdraw = 2,
        BandwidthExchange = 3,
    };

    enum class DepositType {
        Normal = 0,
        RegistrationReward = 1,
    };

    class xinchain : public eosio::contract {
    public:
        using contract::contract;

        xinchain(account_name n)
        : eosio::contract(n)
        , config(n) {}

        void clear(string table, uint32_t count) {
            print("cleaning table '", table, "'");
            require_auth(XINCHAIN);

            if (table == "config") {
                config.clear(count);
            }
            if (table == "account") {
                account_table accounts(_self, _self);
                for(auto iter = accounts.begin(); iter != accounts.end() && count > 0; count--)
                    iter = accounts.erase(iter);
            }
            if (table == "video") {
                video_table videos(_self, _self);
                for(auto iter = videos.begin(); iter != videos.end() && count > 0; count--)
                    iter = videos.erase(iter);
            }
            if (table == "order") {
                order_table orders(_self, _self);
                for(auto iter = orders.begin(); iter != orders.end() && count > 0; count--)
                    iter = orders.erase(iter);
            }

            print("done");
        }

        void issue(uint64_t amount) {
            print("issue ", amount, " tokens");
            require_auth(XINCHAIN);   // transaction signed by xinchain account
            eosio_assert(amount > 0, "illegle amount");
            config.checkContractState();

            account_table accounts(_self, _self);
            auto xinchain_lookup = accounts.find(XINCHAIN);
            if (xinchain_lookup == accounts.end()) {
                newaccountInternal(XINCHAIN, XINCHAIN);
                xinchain_lookup = accounts.find(XINCHAIN);
            }

            // check for integer overflow
            uint64_t newBalance = (*xinchain_lookup).balance + amount;
            eosio_assert(newBalance > (*xinchain_lookup).balance, "integer overflow");

            accounts.modify(xinchain_lookup, XINCHAIN, [&](auto& account) {
                account.balance = newBalance;
            });

            print("issue token successfully");
        }

        void createtoken(account_name user, uint32_t amount) {
            print("create ", amount, " tokens for '", name{user}, "'");
            require_auth(OFGP);   // transaction signed by OFGP account
            eosio_assert(amount > 0, "illegle amount");
            config.checkContractState();

            // token transfer from xinchain to user
            account_table accounts(_self, _self);
            auto xinchain_lookup = accounts.find(XINCHAIN);
            eosio_assert(xinchain_lookup != accounts.end(), "account not found");
            eosio_assert((*xinchain_lookup).balance >= amount, "insufficient balance");
            accounts.modify(xinchain_lookup, OFGP, [&](auto& account) {
                account.balance -= amount;
            });

            auto account_lookup = accounts.find(user);
            eosio_assert(account_lookup == accounts.end() || (*account_lookup).state == (uint8_t)AccountState::Normal, "account locked");

            depositInternal(OFGP, user, amount, DepositType::Normal);

            print("create token successfully");
        }
        
        // memo: the destination chain and address of asset go to
        // eg: memo="{\"address\":\"mveBnLP1bcgsq......Vhyagsk\",\"chain\":\"btc\"}"
        void destroytoken(account_name user, uint32_t amount, string memo) {
            print("destroy ", amount, " tokens for '", name{user}, "'");
            require_auth(user);
            config.checkContractState();

            eosio_assert(user != XINCHAIN, "Operation deny");
            eosio_assert(amount > 0, "illegle amount");
            eosio_assert(memo.size() <= 256, "memo has more than 256 bytes");

            // token transfer from user to xinchain
            account_table accounts(_self, _self);
            auto account_lookup = accounts.find(user);
            if (account_lookup == accounts.end()) {
                newaccountInternal(user, user);
                account_lookup = accounts.find(user);
            }

            eosio_assert((*account_lookup).state == (uint8_t)AccountState::Normal, "account locked");
            eosio_assert((*account_lookup).balance >= amount, "insufficient balance");

            auto xinchain_lookup = accounts.find(XINCHAIN);
            eosio_assert(xinchain_lookup != accounts.end(), "account not found");

            accounts.modify(xinchain_lookup, user, [&](auto& account) {
                account.balance += amount;
            });
            accounts.modify(account_lookup, user, [&](auto& account) {
                account.balance -= amount;
            });

            order_table orders(_self, _self);
            orders.emplace(user, [&](auto& order) {
                order.id = orders.available_primary_key();
                order.user = user;
                order.date = now();
                order.type = (uint8_t)OrderType::Withdraw;
                order.data1 = amount;
                order.data2 = 0;
            });

            print("destroy token successfully");
        }

        // set bandwidth rate
        void setbwrate(uint32_t bandwidthRate) {
            print("setting bandwidth rate to ", bandwidthRate);
            require_auth(XINCHAIN);
            eosio_assert(bandwidthRate > 0, "illegle bandwidth rate");
            config.checkContractState();

            config.setBandwidthRate(XINCHAIN, bandwidthRate);

            print("done");
        }

        // set registration reward
        void setregreward(uint32_t reward) {
            print("setting registration reward to ", reward);
            require_auth(XINCHAIN);
            config.checkContractState();

            config.setRegistrationReward(XINCHAIN, reward);

            print("done");
        }

        void lockaccount(account_name user, uint8_t lock) {
            AccountState state = AccountState::Normal;

            if (lock) {
                state = AccountState::Locked;
                print("locking account '", name{user}, "'");
            } else {
                print("unlocking account '", name{user}, "'");
            }

            require_auth(XINCHAIN);
            config.checkContractState();

            account_table accounts(_self, _self);
            auto account_lookup = accounts.find(user);
            eosio_assert(account_lookup != accounts.end(), "account not found");

            accounts.modify(account_lookup, XINCHAIN, [&](auto& account) {
                account.state = (uint8_t)state;
            });

            if (lock) {
                print("lock account successfully");
            } else {
                print("unlock account successfully");
            }
        }

        void lockvideo(const string ipfs_hash, uint8_t lock) {
            VideoState state = VideoState::Normal;

            if (lock) {
                state = VideoState::Locked;
                print("locking video '", ipfs_hash, "'");
            } else {
                print("unlocking video '", ipfs_hash, "'");
            }

            require_auth(XINCHAIN);
            config.checkContractState();

            uint64_t id = gen_id(ipfs_hash);
            video_table videos(_self, _self);
            auto video_lookup = videos.find(id);
            eosio_assert(video_lookup != videos.end(), "video not found");

            videos.modify(video_lookup, XINCHAIN, [&](auto& video) {
                video.state = (uint8_t)state;
            });

            if (lock) {
                print("lock video successfully");
            } else {
                print("unlock video successfully");
            }
        }

        void newaccount(account_name user) {
            print("creating account '", name{user}, "'");
            require_auth(user);
            config.checkContractState();
            
            account_table accounts(_self, _self);
            auto account_lookup = accounts.find(user);
            eosio_assert(account_lookup == accounts.end(), "account already exist");

            newaccountInternal(user, user);

            uint32_t registrationReward = config.getRegistrationReward();
            if (registrationReward > 0) {
                depositInternal(user, user, registrationReward, DepositType::RegistrationReward);
            }

            print("account create successfully with reward ", registrationReward);
        }

        void publishvideo2(account_name publisher, const string ipfs_hash, const uint32_t price, const uint32_t reward,
                const uint64_t totalReward, const uint32_t outDate, const uint32_t promotionDate) {
            print("publishing video '", ipfs_hash, "' by '", name{publisher}, "', price=", price, ", reward=", reward , "/", totalReward,
                    ", outDate=", outDate, ", promotionDate=", promotionDate);
            require_auth(publisher);
            config.checkContractState();
            
            eosio_assert(publisher != XINCHAIN, "Operation deny");
            eosio_assert(ipfs_hash.size() > 0 && ipfs_hash.size() < 64, "illegle video hash");
            eosio_assert(totalReward >= reward, "totalReward should not less than reward");
            eosio_assert(outDate >= promotionDate, "outDate should not before of promotionDate");

            uint64_t currentTime = now();
            eosio_assert(outDate > currentTime, "outDate should not before of current time");

            account_table accounts(_self, _self);
            auto account_lookup = accounts.find(publisher);
            if (account_lookup == accounts.end()) {
                newaccountInternal(publisher, publisher);
                account_lookup = accounts.find(publisher);
            }

            eosio_assert((*account_lookup).state == (uint8_t)AccountState::Normal, "account locked");

            uint64_t id = gen_id(ipfs_hash);
            video_table videos(_self, _self);
            auto video_lookup = videos.find(id);
            eosio_assert(video_lookup == videos.end(), "video already exist");

            videos.emplace(publisher, [&](auto& video) {
                video.id = id;
                video.user = publisher;
                video.ipfs_hash = ipfs_hash;
                video.price = price;
                video.reward = reward;
                video.totalReward = totalReward;
                video.orders = 0;
                video.promotions = 0;
                video.date = currentTime;
                video.outDate = outDate;
                video.promotionDate = promotionDate;
                video.state = (uint8_t)VideoState::Normal;
            });

            print("publish successfully, id=", id);
        }

        void deletevideo(const string ipfs_hash) {
            print("deleting video '", ipfs_hash, "'");
            config.checkContractState();

            uint64_t id = gen_id(ipfs_hash);
            video_table videos(_self, _self);
            auto video_lookup = videos.find(id);
            eosio_assert(video_lookup != videos.end(), "video not found");
            eosio_assert((*video_lookup).state != (uint8_t)VideoState::Locked, "video locked");
            eosio_assert((*video_lookup).state == (uint8_t)VideoState::Normal, "video deleted");

            auto publisher = (*video_lookup).user;
            require_auth(publisher);

            videos.modify(video_lookup, publisher, [&](auto& video) {
                video.state = (uint8_t)VideoState::Deleted;
            });

            print("delete video successfully");
        }

        void getrewards(account_name publisher, account_name watcher, const string ipfs_hash){
            print(name{publisher}, " send reward to ", name{watcher}, " and video hash is ", ipfs_hash);
            require_auth(publisher);

            config.checkContractState();

            eosio_assert(publisher != XINCHAIN, "Operation deny");

            uint64_t video_id = gen_id(ipfs_hash);
            uint128_t user_video_id = order::make_int128(watcher, video_id);

            video_table videos(_self, _self);
            auto video_lookup = videos.find(video_id);

            eosio_assert(video_lookup != videos.end(), "video not found");
            eosio_assert((*video_lookup).state != (uint8_t)VideoState::Locked, "video locked");
            eosio_assert((*video_lookup).state == (uint8_t)VideoState::Normal, "video deleted");
            eosio_assert((*video_lookup).outDate >= now(), "video out of date");

            bool promotionEnded = false;
            if (((*video_lookup).promotions + 1) * ((*video_lookup).reward) > (*video_lookup).totalReward
                    || (*video_lookup).promotionDate < now()) {
                promotionEnded = true;
            }

            order_table orders(_self, _self);
            auto idx = orders.template get_index<N(uservideoid)>();
            auto order_lookup = idx.find(user_video_id);

            // Check get reward before or not
            eosio_assert(order_lookup == idx.end() || (*order_lookup).data2 == 0, "you already get rewarded");

            uint64_t data2 = now();
            if (!promotionEnded) {
                data2 |= (1LLU << 32);
            }

            if (order_lookup == idx.end()) {
                orders.emplace(publisher, [&](auto& order) {
                    order.id = orders.available_primary_key();
                    order.user = watcher;
                    order.date = 0;
                    order.type = (uint8_t)OrderType::Video;
                    order.data1 = video_id;
                    order.data2 = data2;
                });
            } else {
                orders.modify(*order_lookup, publisher, [&](auto& order) {
                    order.data2 = data2;
                });
            }

            videos.modify(video_lookup, publisher, [&](auto& video) {
                video.promotions += 1;
            });

            // promotion ended, no more reward will be pay to watcher
            if (promotionEnded) {
                print("promotion ended");
                return;
            }

            account_table accounts(_self, _self);
            auto watcher_account = accounts.find(watcher);
            if (watcher_account == accounts.end()) {
                newaccountInternal(publisher, watcher);
                watcher_account = accounts.find(watcher);
            }
            eosio_assert((*watcher_account).state == (uint8_t)AccountState::Normal, "account locked");

            auto publisher_account = accounts.find(publisher);
            if (publisher_account == accounts.end()) {
                newaccountInternal(publisher, publisher);
                publisher_account = accounts.find(publisher);
            }
            
            if ((*video_lookup).reward > 0) {
                // transfer token from buyer to publisher
                accounts.modify(watcher_account, publisher, [&](auto& account) {
                    account.balance += (*video_lookup).reward;
                    account.profit  += (*video_lookup).reward;

                });
                accounts.modify(publisher_account, publisher, [&](auto& account) {
                    // balance limit
                    eosio_assert(account.balance >= (*video_lookup).reward, "insufficient balance");
                    account.balance -= (*video_lookup).reward;
                    account.expense += (*video_lookup).reward;
                });
            }
            
            print("get traile raward successfully, id=", user_video_id);
        }

        void buyvideo(account_name buyer, const string ipfs_hash) {
            print("ordering video '", ipfs_hash, "' by '", name{buyer}, "'");
            require_auth(buyer);
            config.checkContractState();

            eosio_assert(buyer != XINCHAIN, "Operation deny");

            uint64_t video_id = gen_id(ipfs_hash);
            uint128_t user_video_id = order::make_int128(buyer, video_id);

            video_table videos(_self, _self);
            auto video_lookup = videos.find(video_id);
            eosio_assert(video_lookup != videos.end(), "video not found");
            eosio_assert((*video_lookup).state != (uint8_t)VideoState::Locked, "video locked");
            eosio_assert((*video_lookup).state == (uint8_t)VideoState::Normal, "video deleted");
            eosio_assert((*video_lookup).outDate >= now(), "video out of date");

            order_table orders(_self, _self);
            auto idx = orders.template get_index<N(uservideoid)>();
            auto order_lookup = idx.find(user_video_id);

            // date == 0 indicate that the user watched the trailer but have not buy the video
            eosio_assert(order_lookup == idx.end() || (*order_lookup).date == 0, "video already ordered");

            if (order_lookup == idx.end()) {
                orders.emplace(buyer, [&](auto& order) {
                    order.id = orders.available_primary_key();
                    order.user = buyer;
                    order.date = now();
                    order.type = (uint8_t)OrderType::Video;
                    order.data1 = video_id;
                    order.data2 = 0;
                });
            } else {
                // 
                orders.modify(*order_lookup, buyer, [&](auto& order) {
                    order.date = now();
                });
            }

            account_table accounts(_self, _self);
            auto buyer_account = accounts.find(buyer);
            if (buyer_account == accounts.end()) {
                newaccountInternal(buyer, buyer);
                buyer_account = accounts.find(buyer);
            }

            eosio_assert((*buyer_account).state == (uint8_t)AccountState::Normal, "account locked");
            eosio_assert((*buyer_account).balance >= (*video_lookup).price, "insufficient balance");
            account_name publisher = (*video_lookup).user;
            auto publisher_account = accounts.find(publisher);
            if (publisher_account == accounts.end()) {
                newaccountInternal(buyer, publisher);
                publisher_account = accounts.find(publisher);
            }

            // update video order stat
            videos.modify(video_lookup, buyer, [&](auto& video) {
                video.orders += 1;
            });

            if ((*video_lookup).price > 0) {
                // transfer token from buyer to publisher
                accounts.modify(buyer_account, buyer, [&](auto& account) {
                    account.balance -= (*video_lookup).price;
                    account.expense += (*video_lookup).price;
                });
                accounts.modify(publisher_account, buyer, [&](auto& account) {
                    account.balance += (*video_lookup).price;
                    account.profit += (*video_lookup).price;
                });
            }

            print("public successfully, id=", user_video_id);
        }

        // TODO: remove this method, use createtoken() instead
        void deposit(account_name user, uint32_t amount) {
            print("deposit '", name{user}, "' with ", amount);
            require_auth(user);

            eosio_assert(user != XINCHAIN, "Operation deny");

            depositInternal(user, user, amount, DepositType::Normal);

            print("deposit successfully");
        }

        // bandwidth exchange
        void bwexchange(account_name user, const string ipfs_hash, const uint32_t kbytes) {
            print("'", name{user}, "' exchange bandwidth of '", ipfs_hash, "' with ", kbytes, "KB");
            require_auth(user);
            config.checkContractState();

            eosio_assert(user != XINCHAIN, "Operation deny");
            eosio_assert(kbytes > 0, "illegle bandwidth");

            uint32_t bandwidthRate = config.getBandwidthRate();
            if (bandwidthRate == 0) {
                print("bwexchange successfully, amount=0 !!");
                return;
            }

            uint64_t amount = kbytes / bandwidthRate / 10;
            amount *= 10;

            account_table accounts(_self, _self);
            auto account_lookup = accounts.find(user);
            if (account_lookup == accounts.end()) {
                newaccountInternal(user, user);
                account_lookup = accounts.find(user);
            }

            eosio_assert((*account_lookup).state == (uint8_t)AccountState::Normal, "account locked");

            uint64_t video_id = gen_id(ipfs_hash);
            video_table videos(_self, _self);
            auto video_lookup = videos.find(video_id);
            eosio_assert(video_lookup != videos.end(), "video not found");
            eosio_assert((*video_lookup).state == (uint8_t)VideoState::Normal, "video deleted");
            // bandwidth exchange procedure will go ahead even the video is locked

            account_name publisher = (*video_lookup).user;
            auto publisher_account = accounts.find(publisher);
            if (publisher_account == accounts.end()) {
                newaccountInternal(user, publisher);
                publisher_account = accounts.find(publisher);
            }

            eosio_assert((*publisher_account).balance >= amount, "insufficient balance");

            if (amount == 0) {
                print("bwexchange successfully, amount=", amount);
                return;
            }

            accounts.modify(account_lookup, user, [&](auto& account) {
                account.balance += amount;
                account.profit += amount;
            });
            accounts.modify(publisher_account, user, [&](auto& account) {
                account.balance -= amount;
                account.expense += amount;
            });

            order_table orders(_self, _self);
            orders.emplace(user, [&](auto& order) {
                order.id = orders.available_primary_key();
                order.user = user;
                order.date = now();
                order.type = (uint8_t)OrderType::BandwidthExchange;
                order.data1 = amount;
                order.data2 = video_id;
            });

            print("bwexchange successfully, amount=", amount);
        }

        void setnodeid(account_name user, string node_id) {
            print("setting '", name{user}, "' node id to '", node_id, "'");
            require_auth(user);
            config.checkContractState();

            eosio_assert(user != XINCHAIN, "Operation deny");
            eosio_assert(node_id.size() > 0 && node_id.size() < 64, "illegle node id");

            account_table accounts(_self, _self);
            auto account_lookup = accounts.find(user);
            if (account_lookup == accounts.end()) {
                newaccountInternal(user, user);
                account_lookup = accounts.find(user);
            }

            eosio_assert((*account_lookup).state == (uint8_t)AccountState::Normal, "account locked");

            accounts.modify(account_lookup, user, [&](auto& account) {
                account.node_id = node_id;
            });

            print("set nodeid successfully");
        }

        void setprofile(account_name user, string profile) {
            print("setting '", name{user}, "' profile to '", profile, "'");
            require_auth(user);
            config.checkContractState();

            eosio_assert(user != XINCHAIN, "Operation deny");
            eosio_assert(profile.size() > 0 && profile.size() < 64, "illegle profile");

            account_table accounts(_self, _self);
            auto account_lookup = accounts.find(user);
            if (account_lookup == accounts.end()) {
                newaccountInternal(user, user);
                account_lookup = accounts.find(user);
            }

            eosio_assert((*account_lookup).state == (uint8_t)AccountState::Normal, "account locked");

            accounts.modify(account_lookup, user, [&](auto& account) {
                account.profile = profile;
            });

            print("set profile successfully");
        }

    private:
        static uint64_t gen_id(const string data) {
            checksum256 result;
            sha256( (char *)data.c_str(), data.size(), &result);
            return *((uint64_t*)&result.hash[0]);
        }

        void newaccountInternal(account_name payer, account_name user) {
            account_table accounts(_self, _self);
            accounts.emplace(payer, [&](auto& account) {
                account.user = user;
                account.balance = 0;
                account.freeze = 0;
                account.profit = 0;
                account.expense = 0;
                account.date = now();
                account.state = (uint8_t)AccountState::Normal;
            });
        }

        void depositInternal(account_name payer, account_name user, uint32_t amount, DepositType depositType) {
            account_table accounts(_self, _self);
            auto account_lookup = accounts.find(user);
            if (account_lookup == accounts.end()) {
                newaccountInternal(payer, user);
                account_lookup = accounts.find(user);
            }

            // check for integer overflow
            uint64_t newBalance = (*account_lookup).balance + amount;
            eosio_assert(newBalance > (*account_lookup).balance, "integer overflow");

            accounts.modify(account_lookup, payer, [&](auto& account) {
                account.balance = newBalance;
            });

            order_table orders(_self, _self);
            orders.emplace(payer, [&](auto& order) {
                order.id = orders.available_primary_key();
                order.user = user;
                order.date = now();
                order.type = (uint8_t)OrderType::Deposit;
                order.data1 = amount;
                order.data2 = (uint64_t)depositType;
            });
        }

        config_manager config;

        // @abi table account i64
        struct account {
            account_name user;
            string node_id;
            string profile;
            uint64_t balance;
            uint64_t freeze;
            uint64_t profit;
            uint64_t expense;
            uint32_t date;      // UNIX timestamp, second
            uint8_t  state;

            uint64_t primary_key() const {return user;}
            uint64_t by_node_id() const {return gen_id(node_id);}

            EOSLIB_SERIALIZE(account, (user)(node_id)(profile)(balance)(freeze)(profit)(expense)(date)(state))
        };
        typedef multi_index<
            N(account), account,
            indexed_by<N(user), const_mem_fun<account, uint64_t, &account::by_node_id>>
        > account_table;

        // @abi table video i64
        struct video {
            uint64_t id;
            account_name user;
            string ipfs_hash;
            uint32_t price;
            uint32_t reward;
            uint64_t totalReward;
            uint32_t orders;
            uint32_t promotions;
            uint32_t date;              // Publish date. UNIX timestamp, second
            uint32_t outDate;           // The date to pull video out. UNIX timestamp, second
            uint32_t promotionDate;     // Promotion deadline. UNIX timestamp, second
            uint8_t  state;

            uint64_t primary_key() const {return id;}
            uint64_t by_user() const {return user;}

            EOSLIB_SERIALIZE(video, (id)(user)(ipfs_hash)(price)(reward)(totalReward)(orders)(promotions)(date)(outDate)(promotionDate)(state))
        };
        typedef multi_index<
            N(video), video,
            indexed_by<N(user), const_mem_fun<video, account_name, &video::by_user>>
        > video_table;

        // @abi table order i64
        struct order {
            uint64_t id;
            account_name user;
            uint32_t date;              // UNIX timestamp, second
            uint8_t type;               // Video, Deposit, Withdraw

            uint64_t data1;             // video_id if type == Video, otherwise the amount of deposit or withdraw
            uint64_t data2;             // the date user watch trailer if type == Video

            uint64_t primary_key() const {return id;}
            uint64_t by_user() const {return user;}
            uint128_t by_user_type() const {return make_int128(user, type);}
            uint64_t by_video_id() const {return type == (uint8_t)OrderType::Video ? data1 : 0;}
            uint128_t by_user_video_id() const {return type == (uint8_t)OrderType::Video ? make_int128(user, data1) : 0;}

            static uint128_t make_int128(account_name user, uint64_t val) {
                return ((uint128_t)val) << 64 | (uint128_t)user;
            }

            EOSLIB_SERIALIZE(order, (id)(user)(date)(type)(data1)(data2))
        };
        typedef multi_index<
            N(order), order,
            indexed_by<N(user), const_mem_fun<order, account_name, &order::by_user>>,
            indexed_by<N(videoid), const_mem_fun<order, uint64_t, &order::by_video_id>>,
            indexed_by<N(usertype), const_mem_fun<order, uint128_t, &order::by_user_type>>,
            indexed_by<N(uservideoid), const_mem_fun<order, uint128_t, &order::by_user_video_id>>
        > order_table;
    };
}