#include "eosadditapps.hpp"

ACTION eosadditapps::signup(name account, string nickname, string avatar, string memo) {
    require_auth( account );
    
    print_f("eosadditapps::signup");
}

ACTION eosadditapps::update(name account, string nickname, string avatar, string memo) {
    require_auth( account );
    print_f("eosadditapps::update");
}

ACTION eosadditapps::add(name account, string url, string comment) {
    require_auth( _self );
    print_f("eosadditapps::add");
}

ACTION eosadditapps::edit(name account, uint64_t opidx, uint64_t coidx, string comment) {
    print_f("eosadditapps::edit");
}

ACTION eosadditapps::del(name account, uint64_t opidx, uint64_t coidx) {
    print_f("eosadditapps::del");
}

ACTION eosadditapps::vote(name account, uint64_t index, int8_t vote) {
    print_f("eosadditapps::vote");
}

ACTION eosadditapps::repute(name account, string url, int8_t repute) {
    print_f("eosadditapps::repute");
}

EOSIO_DISPATCH( eosadditapps, (signup)(update)(add)(edit)(del)(vote)(repute) )
