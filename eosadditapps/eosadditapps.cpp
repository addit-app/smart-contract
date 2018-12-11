#include "eosadditapps.hpp"

namespace addit {
    ACTION eosadditapps::signup(name account, string nickname, string avatar, string memo) {
        require_auth( account );
        
        eosio_assert( nickname.size() <= 32, "nickname has more than 32 bytes" );
        eosio_assert( avatar.size() <= 256, "avatar link size has more than 256 bytes" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        users_index uidx( _self, account.value );
        auto itr = uidx.find(account.value);
        if ( itr == uidx.end() ) {
            // insert user account information
            uidx.emplace( _self, [&]( auto& u ) {
                u.account = account;
                u.nickname = nickname;
                u.memo = memo;
                u.avatar = avatar;
                u.repute = 1000;
            });
        } else {
            // update user account information
            uidx.modify( itr, _self, [&]( auto& u ) {
                u.account = account;
                u.nickname = nickname;
                u.memo = memo;
                u.avatar = avatar;
            });
        }

        // initial reward 100.0000 ADDIT
        asset balance(1000000, symbol( symbol_code( tokensymbol ), 4) );
        add_supply( balance );
        add_balance( account, balance, account );
    }

    ACTION eosadditapps::addit(name account, int64_t iopinion, string url, string comment) {
        require_auth( account );

        users_index uidx( _self, account.value );
        auto uitr = uidx.find(account.value);
        eosio_assert( uitr != uidx.end(), "failed find user, signup first" );

        // payment 1 ADDIT
        asset balance(10000, symbol( symbol_code( tokensymbol ), 4) );
        account_index accounts( _self, account.value );
        auto aitr = accounts.find( balance.symbol.code().raw() );
        eosio_assert( aitr->balance.amount > balance.amount, "not enough balance" );

        sub_balance( account, balance );
        sub_supply( balance );

        eosio_assert( url.size() <= 256, "url size has more than 256 bytes" );
        eosio_assert( comment.size() <= 256, "comment has more than 256 bytes" );

        uint64_t blocktime = publication_time();
        
        if (iopinion < 0) {
            // insert url to domains_index
            domains_index didx( _self, _self.value );
            auto urlindex = didx.available_primary_key();
            iopinion = urlindex;
            didx.emplace( _self, [&]( auto& d ) {
                d.index = urlindex;
                d.url = url;
            });

            // insert url to opinions_index
            opinions_index oidx( _self, iopinion );
            oidx.emplace( _self, [&]( auto& o ) {
                o.index = urlindex;
                o.url = url;
                o.repvalue = 0;
                o.blocktime = blocktime;
            });
        }

        // insert data to comments_index
        comments_index cidx( _self, iopinion );
        cidx.emplace( _self, [&]( auto& c ) {
            c.index = cidx.available_primary_key();
            c.account = account;
            c.comment = comment;
            c.blocktime = blocktime;
            c.upvote = 0;
            c.downvote = 0;
        });
    }

    ACTION eosadditapps::del(name account, uint64_t iopinion, uint64_t icomment) {
        require_auth( account );
        
        // payment 5 ADDIT
        asset balance(50000, symbol( symbol_code( tokensymbol ), 4) );
        account_index accounts( _self, account.value );
        auto uitr = accounts.find( balance.symbol.code().raw() );
        eosio_assert( uitr->balance.amount > balance.amount, "not enough balance" );
        
        sub_balance( account, balance );
        sub_supply( balance );

        // remove comments data from comments_index
        comments_index cidx( _self, iopinion );
        auto citr = cidx.find( icomment );

        eosio_assert( citr != cidx.end(), "comment index does not exist." );
        cidx.erase( citr );
    }

    ACTION eosadditapps::create( asset max_supply ) {
        require_auth( _self );

        asset supply(0, symbol( symbol_code( tokensymbol ), 4) );
        auto symbol = supply.symbol;
        max_supply.symbol = symbol;
        
        auto symbol_name = symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto existing_currency = currency_table.find( symbol_name );
        eosio_assert( existing_currency == currency_table.end(), "token with symbol already exists" );

        currency_table.emplace( _self, [&]( auto& currency ) {
            currency.supply = supply;
            currency.max_supply = max_supply;
            currency.issuer = _self;
        });
    }

    ACTION eosadditapps::issue( asset quantity, string memo ) {
        auto symbol = quantity.symbol;
        eosio_assert( symbol.is_valid(), "invalid symbol name" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        auto symbol_name = symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto existing_currency = currency_table.find( symbol_name );
        eosio_assert( existing_currency != currency_table.end(), "token with symbol does not exist. create token before issue" );
        const auto& st = *existing_currency;

        require_auth( st.issuer );
        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must issue positive quantity" );
        eosio_assert( quantity.amount <= st.max_supply.amount, "quantity exceeds available supply" );
        eosio_assert( symbol == st.supply.symbol, "symbol precision mismatch" );

        add_supply( quantity );
        add_balance( _self, quantity, st.issuer );
    }

    ACTION eosadditapps::transfer( name from, name to, asset quantity, string memo ) {
        eosio_assert( from != to, "cannot transfer to self" );
        require_auth( from );

        eosio_assert( is_account( to ), "to account does not exist");
        auto sym = quantity.symbol.code().raw();
        currency_index currency_table( _self, sym );
        const auto& st = currency_table.get( sym );

        require_recipient( from );
        require_recipient( to );

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount > 0, "must transfer positive quantity" );
        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        sub_balance( from, quantity );
        add_balance( to, quantity, from );
    }

    ACTION eosadditapps::burn( name account, asset quantity, string memo ) {
        require_auth( _self );

        auto sym = quantity.symbol;
        eosio_assert( sym.is_valid(), "invalid symbol name" );
        eosio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

        auto sym_name = sym.code().raw();
        currency_index currency_table( _self, sym_name );
        auto existing = currency_table.find( sym_name );
        eosio_assert( existing != currency_table.end(), "token with symbol does not exist, create token before burn" );
        const auto& st = *existing;

        eosio_assert( quantity.is_valid(), "invalid quantity" );
        eosio_assert( quantity.amount >= 0, "must burn positive or zero quantity" );

        eosio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
        eosio_assert( quantity.amount <= st.supply.amount, "quantity exceeds available supply");

        sub_supply( quantity );
        sub_balance( account, quantity );
    }

    void eosadditapps::sub_balance( name owner, asset value ) {
        account_index from_acnts( _self, owner.value );
        const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );
        eosio_assert( from.balance.amount >= value.amount, "overdrawn balance" );

        if( from.balance.amount == value.amount ) {
            from_acnts.erase( from );
        } else {
            from_acnts.modify( from, owner, [&]( auto& a ) {
                a.balance -= value;
            });
        }
    }

    void eosadditapps::add_balance( name owner, asset value, name ram_payer ) {
        account_index to_accounts( _self, owner.value );
        auto to = to_accounts.find( value.symbol.code().raw() );
        if( to == to_accounts.end() ) {
            to_accounts.emplace( ram_payer, [&]( auto& a ){
                a.balance = value;
            });
        } else {
            to_accounts.modify( to, _self, [&]( auto& a ) {
                a.balance += value;
            });
        }
    }

    void eosadditapps::sub_supply( asset quantity ) {
        auto symbol_name = quantity.symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto current_currency = currency_table.find( symbol_name );

        currency_table.modify( current_currency, _self, [&]( auto& currency ) {
            currency.supply -= quantity;
        });
    }

    void eosadditapps::add_supply( asset quantity ) {
        auto symbol_name = quantity.symbol.code().raw();
        currency_index currency_table( _self, symbol_name );
        auto current_currency = currency_table.find( symbol_name );

        currency_table.modify( current_currency, name(0), [&]( auto& currency ) {
            currency.supply += quantity;
        });
    }

    ACTION eosadditapps::repute(name account, uint64_t iopinion, int8_t repute) {
        require_auth( account );
        uint64_t blocktime = publication_time();
        bool flag = true;

        if (repute != UPVOTE && repute != DOWNVOTE) {
            eosio_assert( false, "fail to repute value" );
        }
        // cli get table eosadditapps eosadditapps domain 의 인덱스가 iopinion 임
        // iopinion(url index)가 있는지 체크 (없으면 에러)
        domains_index didx( _self, _self.value );
        auto ditr = didx.find( iopinion );
        eosio_assert( ditr != didx.end(), "failed find opinion url" );

        // repute에서 account의 기록이 있는지 체크
        opinions_index oidx( _self, iopinion );
        auto oitr = oidx.find( iopinion );
        eosio_assert( oitr != oidx.end(), "failed find opinions" );

        // opinions_index에 repute 업데이트
        oidx.modify( oitr, _self, [&]( auto& o ) {
            for (int index = 0; index < o.repute.size(); index++) {
                if (o.repute[index].voter.value == account.value) {
                    flag = false;
                    int8_t slc = select_vote(o.repute[index].vote, repute);
                    switch ( slc ) {  
                        case 1:
                            // up, up
                            o.repvalue = oitr->repvalue - 1;
                            o.repute[index].vote = 0;
                            break;
                        case 2:  
                            // up, down
                            o.repvalue = oitr->repvalue - 2;
                            o.repute[index].vote = DOWNVOTE;
                            break;
                        case 3:
                            // down, down
                            o.repvalue = oitr->repvalue + 1;
                            o.repute[index].vote = 0;
                            break;
                        case 4:
                            // down, up
                            o.repvalue = oitr->repvalue + 2;
                            o.repute[index].vote = UPVOTE;
                            break;
                        default:
                            o.repvalue = oitr->repvalue + repute;
                            o.repute[index].vote = repute;
                            o.repute[index].voter = account;
                            break;
                    }

                    o.repute[index].blocktime = blocktime;
                    break;
                }
            }
        });

        if (flag) {
            oidx.modify( oitr, _self, [&]( auto& o ) {
                o.repvalue = oitr->repvalue + repute;

                rlog log;
                log.voter = account;
                log.vote = repute;
                log.blocktime = blocktime;

                o.repute.push_back(log);
            });
        }
    }

    ACTION eosadditapps::vote(name account, uint64_t iopinion, uint64_t icomment, int8_t vote) {
        require_auth( account );
        uint64_t blocktime = publication_time();
        uint32_t comment_upvote;
        uint32_t comment_downvote;
        int64_t user_repute;
        asset balance(0, symbol( symbol_code( tokensymbol ), 4) );

        bool flag = true;

        if (vote != UPVOTE && vote != DOWNVOTE) {
            eosio_assert( false, "fail to vote value" );
        }

        // comments_index에서 icomment 찾기
        comments_index cidx( _self, iopinion );
        auto citr = cidx.find( icomment );
        eosio_assert( citr->account.value != account.value, "can't vote your self" );


        cidx.modify( citr, _self, [&]( auto& c ) {
            for (int index = 0; index < c.vote.size(); index++) {
                if (c.vote[index].voter.value == account.value) {
                    // 기존에 voting 내역이 있음
                    int8_t slc = select_vote(c.vote[index].vote, vote);
                    asset reward = calc_reward(c.account);
                    switch ( slc ) {
                        case 1:
                            // up, up
                            c.upvote -= 1;
                            c.vote[index].balance = balance;
                            c.vote[index].vote = 0;

                            sub_supply( reward );
                            sub_balance( c.account, reward );
                            break;
                        case 2:
                            // up, down
                            c.upvote -= 1;
                            c.downvote += 1;
                            c.vote[index].balance = balance;
                            c.vote[index].vote = DOWNVOTE;

                            sub_supply( reward );
                            sub_balance( c.account, reward );
                            break;
                        case 3:
                            // down, down
                            c.downvote -= 1;
                            c.vote[index].balance = balance;
                            c.vote[index].vote = 0;
                            break;
                        case 4:
                            // down, up
                            c.upvote += 1;
                            c.downvote -= 1;
                            c.vote[index].balance = reward;
                            c.vote[index].vote = UPVOTE;

                            add_supply( reward );
                            add_balance( c.account, reward, _self );
                            break;
                        default:
                            if (vote == UPVOTE) {
                                c.upvote += 1;
                                c.vote[index].balance = reward;
                                c.vote[index].vote = vote;

                                add_supply( reward );
                                add_balance( c.account, reward, _self );
                            } else {
                                c.downvote += 1;
                                c.vote[index].balance = balance;
                            }
                            break;
                    }

                    
                    c.vote[index].blocktime = blocktime;
                    flag = false;
                    break;
                }
            }

            if (flag) {
                // 기존에 voting 내역이 없음
                voting log;
                log.voter = account;
                log.vote = vote;
                log.blocktime = blocktime;

                if (vote == UPVOTE) {
                    asset reward = calc_reward(c.account);
                    c.upvote += 1;

                    log.balance = reward;
                    add_supply( reward );
                    add_balance( c.account, reward, _self );
                } else {
                    log.balance = balance;
                    c.downvote += 1;
                }

                c.vote.push_back(log);
            }
        });
    }

    asset eosadditapps::calc_reward( name account ) {
        users_index uidx( _self, account.value );
        auto uitr = uidx.find(account.value);

        int64_t b = uitr->repute * 10;
        if ( b < 1 ) {
            b = 0;
        } else if (b > 9999) {
            b = 10000;
        }

        asset balance(b, symbol( symbol_code( tokensymbol ), 4) );

        return balance;
    }

    int8_t eosadditapps::select_vote( uint8_t old_vote, uint8_t new_vote ) {
        if (old_vote == DEFAULTVOTE) {
            return 0;
        } else if (old_vote == UPVOTE && new_vote == UPVOTE ) {
            return 1;
        } else if (old_vote == UPVOTE && new_vote == DOWNVOTE ) {
            return 2;
        } else if (old_vote == DOWNVOTE && new_vote == DOWNVOTE ) {
            return 3;
        } else if (old_vote == DOWNVOTE && new_vote == UPVOTE ) {
            return 4;
        }
        return 0;
    }
} // addit

EOSIO_DISPATCH( addit::eosadditapps, (signup)(addit)(del)(create)(issue)(transfer)(burn)(repute)(vote) )
