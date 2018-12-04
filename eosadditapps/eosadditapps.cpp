#include "eosadditapps.hpp"

ACTION eosadditapps::signup( string nickname, string avatar, string comment ) {
    print_f("signup");
}
ACTION eosadditapps::update( string nickname, string avatar, string comment ) {
    print_f("update");
}
ACTION eosadditapps::add( string url, string comment ) {
    print_f("add");
}
ACTION eosadditapps::edit( uint64_t index, string comment ) {
    print_f("edit");
}
ACTION eosadditapps::del( uint64_t index ) {
    print_f("del");
}
ACTION eosadditapps::vote( uint64_t index, int8_t vote ) {
    print_f("vote");
}
ACTION eosadditapps::create( asset maximum_supply ) {
    print_f("create");
}
ACTION eosadditapps::issue( asset quantity, string memo ) {
    print_f("issue");
}
ACTION eosadditapps::transfer( name from, name to, asset quantity, string memo ) {
    print_f("transfer");
}

EOSIO_DISPATCH( eosadditapps, (signup)(update)(add)(edit)(del)(vote)(create)(issue)(transfer) )
