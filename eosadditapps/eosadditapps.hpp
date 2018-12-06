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

        // Action
        ACTION signup(name account, vector<string> nickname, vector<string> avatar, vector<string> memo);
        ACTION addit(name account, uint64_t iopinion, vector<string> url, vector<string> comment);
        ACTION del(name account, uint64_t iopinion, uint64_t icomment);
        ACTION vote(name account, uint64_t iopinion, uint64_t icomment, int8_t vote);
        ACTION repute(name account, uint64_t iopinion, int8_t repute);

        using signup_action = action_wrapper<"signup"_n, &eosadditapps::signup>;
        using addit_action = action_wrapper<"addit"_n, &eosadditapps::addit>;
        using del_action = action_wrapper<"del"_n, &eosadditapps::del>;
        using vote_action = action_wrapper<"vote"_n, &eosadditapps::vote>;
        using repute_action = action_wrapper<"repute"_n, &eosadditapps::repute>;

        // Table
        TABLE user {
            name account;
            vector<string> nickname;
            vector<string> memo;
            vector<string> avatar;

            uint64_t primary_key()const { return account.value; }
        };
        using users_index = multi_index<"user"_n, user>;

        TABLE voting {
            uint64_t index;
            name account;
            int8_t vote;
            block_timestamp blocktime;

            uint64_t primary_key() const { return account.value; }
        };
        using votings_index = multi_index<"voting"_n, voting>;

        TABLE reputation {
            uint64_t index;
            name account;
            int8_t vote;
            block_timestamp blocktime;

            uint64_t primary_key() const { return account.value; }
        };
        using reputations_index = multi_index<"reputation"_n, reputation>;

        TABLE comments {
            uint64_t index;
            name account;
            vector<string> comment;
            uint32_t upvote;
            uint32_t downvote;
            block_timestamp blocktime;

            uint64_t primary_key() const { return index; }
        };
        using comments_index = multi_index<"comments"_n, comments>;

        TABLE opinion {
            uint64_t index;
            vector<string> url;
            int64_t repvalue;
            block_timestamp blocktime;

            vector<string> primary_key() const { return url; }
        };
        using opinions_index = multi_index<"opinion"_n, opinion>;

        TABLE domain {
            uint64_t index;
            uint64_t urls;
            vector<string> url;

            uint64_t primary_key() const { return index; }
        };
        using domains_index = multi_index<"domain"_n, domain>;
};
