#include <eosiolib/eosio.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/asset.hpp>
#include <eosiolib/time.hpp>
#include "multiindex/addit.hpp"
#include "multiindex/token.hpp"

using namespace eosio;
using namespace std;

CONTRACT additeosioap : public eosio::contract {
    private:
        const char* tokensymbol = "ADDIT";
    public:
        using contract::contract;
        additeosioap( name receiver, name code, datastream<const char*> ds )
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
        ACTION upvote( uint64_t index );
        ACTION downvote( uint64_t index );

        // token economy
        ACTION create( asset maximum_supply );
        ACTION issue( asset quantity, string memo );
        ACTION transfer( name from, name to, asset quantity, string memo );

        inline asset get_supply( symbol_code sym ) const;
        inline asset get_balance( name owner, symbol_code sym ) const;

        using add_action = action_wrapper<"add"_n, &additeosioap::add>;
        using edit_action = action_wrapper<"edit"_n, &additeosioap::edit>;
        using del_action = action_wrapper<"del"_n, &additeosioap::del>;
        using upvote_action = action_wrapper<"upvote"_n, &additeosioap::upvote>;
        using downvote_action = action_wrapper<"downvote"_n, &additeosioap::downvote>;
        using create_action = action_wrapper<"create"_n, &additeosioap::create>;
        using issue_action = action_wrapper<"issue"_n, &additeosioap::issue>;
        using transfer_action = action_wrapper<"transfer"_n, &additeosioap::transfer>;

        TABLE user {
            name account;
            string nickname;
            string comment;
            string avatar;
            uint64_t primary_key()const { return account.value; }
        };

        TABLE comments {
            uint64_t index;
            name account;
            string comment;
            uint32_t upvote;
            uint32_t downvote;
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
