#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/time.hpp>

using namespace eosio;
using namespace std;

CONTRACT eosadditapps : public eosio::contract
{
    private:
        const char *tokensymbol = "ADDIT";
        const int8_t UPVOTE = 1;
        const int8_t DOWNVOTE = -1;

    public:
        using contract::contract;
        eosadditapps(name receiver, name code, datastream<const char *> ds)
            : contract(receiver, code, ds) {}

        TABLE user {
            name account;
            string nickname;
            string memo;
            string avatar;

            uint64_t primary_key()const { return account.value; }
        };

        TABLE voting {
            name account;
            int8_t vote;
            block_timestamp blocktime;

            uint64_t primary_key() const { return account.value; }
        };

        TABLE reputation {
            name account;
            int8_t vote;
            block_timestamp blocktime;

            uint64_t primary_key() const { return account.value; }
        };

        TABLE comments {
            uint64_t index;
            name account;
            string comment;
            uint32_t upvote;
            uint32_t downvote;
            voting vote;
            block_timestamp blocktime;

            uint64_t primary_key() const { return index; }
        };

        TABLE opinion {
            uint64_t index;
            string url;
            comments comment;
            reputation repute;
            int64_t repvalue;
            block_timestamp blocktime;

            uint64_t primary_key() const { return index; }
        };

        ACTION signup(name account, string nickname, string avatar, string memo);
        ACTION update(name account, string nickname, string avatar, string memo);
        ACTION add(name account, string url, string comment);
        ACTION edit(name account, uint64_t opidx, uint64_t coidx, string comment);
        ACTION del(name account, uint64_t opidx, uint64_t coidx);
        ACTION vote(name account, uint64_t index, int8_t vote);
        ACTION repute(name account, string url, int8_t repute);

        using signup_action = action_wrapper<"signup"_n, &eosadditapps::signup>;
        using update_action = action_wrapper<"update"_n, &eosadditapps::update>;
        using add_action = action_wrapper<"add"_n, &eosadditapps::add>;
        using edit_action = action_wrapper<"edit"_n, &eosadditapps::edit>;
        using del_action = action_wrapper<"del"_n, &eosadditapps::del>;
        using vote_action = action_wrapper<"vote"_n, &eosadditapps::vote>;
        using repute_action = action_wrapper<"repute"_n, &eosadditapps::repute>;

        using opinions_index = multi_index<"opinion"_n, opinion>;
        using users_index = multi_index<"user"_n, user>;
};
