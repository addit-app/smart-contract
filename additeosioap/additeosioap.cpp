#include "additeosioap.hpp"

ACTION additeosioap::signup( string nickname, string avatar, string comment ) {
    print_f("signup");
}
ACTION additeosioap::update( string nickname, string avatar, string comment ) {
    print_f("update");
}
ACTION additeosioap::add( string url, string comment ) {
    print_f("add");
}
ACTION additeosioap::edit( uint64_t index, string comment ) {
    print_f("edit");
}
ACTION additeosioap::del( uint64_t index ) {
    print_f("del");
}
ACTION additeosioap::upvote( uint64_t index ) {
    print_f("upvote");
}
ACTION additeosioap::downvote( uint64_t index ) {
    print_f("downvote");
}
ACTION additeosioap::create( asset maximum_supply ) {
    print_f("create");
}
ACTION additeosioap::issue( asset quantity, string memo ) {
    print_f("issue");
}
ACTION additeosioap::transfer( name from, name to, asset quantity, string memo ) {
    print_f("transfer");
}

EOSIO_DISPATCH( additeosioap, (signup)(update)(add)(edit)(del)(upvote)(downvote)(create)(issue)(transfer) )
