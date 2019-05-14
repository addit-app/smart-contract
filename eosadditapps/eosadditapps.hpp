#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

using namespace eosio;
using namespace std;

namespace addit {
    CONTRACT eosadditapps : public eosio::contract {
        private:
            const char *tokensymbol = "ADDIT";
            const int8_t UPVOTE = 1;
            const int8_t DOWNVOTE = -1;
            const int8_t DEFAULTVOTE = 0;

        public:
            using contract::contract;
            eosadditapps(name receiver, name code, datastream<const char *> ds)
                : contract(receiver, code, ds) {}

            // addit Action
            ACTION signup(name account, string nickname, string avatar, string memo);
            ACTION addit(name account, int64_t iopinion, string url, string comment);
            ACTION del(name account, uint64_t iopinion, uint64_t icomment);
            ACTION vote(name account, uint64_t iopinion, uint64_t icomment, int8_t vote);
            ACTION repute(name account, uint64_t iopinion, int8_t repute);
            
            // token Action
            ACTION create(asset max_supply);
            ACTION issue(asset quantity, string memo);
            ACTION transfer(name from, name to, asset quantity, string memo);
            ACTION burn(name account, asset quantity, string memo);

            using signup_action = action_wrapper<"signup"_n, &eosadditapps::signup>;
            using addit_action = action_wrapper<"addit"_n, &eosadditapps::addit>;
            using del_action = action_wrapper<"del"_n, &eosadditapps::del>;
            using vote_action = action_wrapper<"vote"_n, &eosadditapps::vote>;
            using repute_action = action_wrapper<"repute"_n, &eosadditapps::repute>;
            
            using create_action = action_wrapper<"create"_n, &eosadditapps::create>;
            using issue_action = action_wrapper<"issue"_n, &eosadditapps::issue>;
            using transfer_action = action_wrapper<"transfer"_n, &eosadditapps::transfer>;
            using burn_action = action_wrapper<"burn"_n, &eosadditapps::burn>;

            // struct
            // url별 repute 기록
            struct rlog {
                name     voter;
                int8_t   vote;
                uint64_t blocktime;
            };

            // 코멘트별 vote 기록
            struct voting {
                name     voter;
                int8_t   vote;
                asset    balance;
                uint64_t blocktime;
            };

            // Table
            // 사용자 정보
            TABLE user {
                name    account;
                string  nickname;
                string  memo;
                string  avatar;
                int64_t repute;

                uint64_t primary_key()const { return account.value; }
            };
            using users_index = multi_index<"user"_n, user>;

            // url에 대한 코멘트(의견)
            TABLE comments {
                uint64_t       index;
                name           account;
                string         comment;
                vector<voting> vote;
                uint64_t       blocktime;
                uint64_t       upvote;
                uint64_t       downvote;

                uint64_t primary_key() const { return index; }
            };
            using comments_index = multi_index<"comments"_n, comments>;

            // url에 대한 평판  
            TABLE opinion {
                uint64_t     index;
                string       url;
                int64_t      repvalue;
                vector<rlog> repute;
                uint64_t     blocktime;

                uint64_t primary_key() const { return index; }
            };
            using opinions_index = multi_index<"opinion"_n, opinion>;

            // url의 index 정보조회용 테이블
            TABLE domain {
                uint64_t index;
                string   url;

                uint64_t primary_key() const { return index; }
            };
            using domains_index = multi_index<"domain"_n, domain>;

            TABLE account {
                asset balance;

                uint64_t primary_key() const { return balance.symbol.code().raw(); }
            };
            using account_index = eosio::multi_index<"accounts"_n, account>;

            TABLE currency_stats {
                asset supply;
                asset max_supply;
                name  issuer;

                uint64_t primary_key() const { return supply.symbol.code().raw(); }
                uint64_t get_issuer() const { return issuer.value; }
            };
            using currency_index = eosio::multi_index<"stat"_n, currency_stats, indexed_by< "byissuer"_n, const_mem_fun< currency_stats, uint64_t, &currency_stats::get_issuer> > >;

        private:
            void sub_balance(name owner, asset value);
            void add_balance(name owner, asset value, name ram_payer);
            void sub_supply(asset quantity);
            void add_supply(asset quantity);
            asset calc_reward(name account);
            void user_repute(name account, int64_t repute);
            int8_t select_vote(int8_t old_vote, int8_t new_vote);
    };
} // addit