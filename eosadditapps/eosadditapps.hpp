#include <eosiolib/eosio.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/asset.hpp>
#include <eosiolib/time.hpp>
#include "multiindex/addit.hpp"
#include "multiindex/token.hpp"

using namespace eosio;
using namespace std;

CONTRACT eosadditapps : public eosio::contract {
    private:
        const char* tokensymbol = "ADDIT";
    public:
        using contract::contract;
        eosadditapps( name receiver, name code, datastream<const char*> ds )
            : contract(receiver, code, ds)
            , opinion(receiver, receiver.value)
            , user(receiver, receiver.value) {}

        // account management
        ACTION signup( string nickname, string avatar, string comment );
        ACTION update( string nickname, string avatar, string comment );

        // addit
        ACTION add( string url, string comment );
        ACTION edit( uint64_t index, string comment );
        ACTION del( uint64_t index );
        ACTION vote( uint64_t index, int8_t vote );

        // token economy
        ACTION create( asset maximum_supply );
        ACTION issue( asset quantity, string memo );
        ACTION transfer( name from, name to, asset quantity, string memo );

        inline asset get_supply( symbol_code sym ) const;
        inline asset get_balance( name owner, symbol_code sym ) const;

        using add_action = action_wrapper<"add"_n, &eosadditapps::add>;
        using edit_action = action_wrapper<"edit"_n, &eosadditapps::edit>;
        using del_action = action_wrapper<"del"_n, &eosadditapps::del>;
        using vote_action = action_wrapper<"vote"_n, &eosadditapps::vote>;
        using create_action = action_wrapper<"create"_n, &eosadditapps::create>;
        using issue_action = action_wrapper<"issue"_n, &eosadditapps::issue>;
        using transfer_action = action_wrapper<"transfer"_n, &eosadditapps::transfer>;

        TABLE user {
            name account;
            string nickname;
            string comment;
            string avatar;
            uint64_t primary_key()const { return account.value; }
        };

        TABLE votelog {
            name account;
            int8_t vote;
            block_timestamp blocktime;
            uint64_t primary_key()const { return account.value; }
        };
        TABLE comments {
            uint64_t index;
            name account;
            string comment;
            uint32_t upvote;
            uint32_t downvote;
            votelog voting;
            block_timestamp blocktime;
            uint64_t primary_key()const { return index; }
        };
        TABLE opinion {
            uint64_t index;
            string url;
            capi_checksum512 url_hash;
            comments comment;
            block_timestamp blocktime;
            uint64_t primary_key()const { return index; }
        };

        typedef eosio::multi_index<"user"_n, user> users;
        typedef eosio::multi_index<"opinion"_n, opinion> opinions;

    private:
        opinions opinion;
        users user;
};
