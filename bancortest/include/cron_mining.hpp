
#pragma once
#include <eosio/eosio.hpp>
//#include <eosio/action.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/time.hpp>
#include <eosio/singleton.hpp>


#define _CONTRACT_NAME_ "cron.eos"
#define _BANCOR_CONTRACT_ "bancorcnvrtr"

struct reserve_t {
    name contract;
    uint64_t ratio;
    asset balance;
    uint64_t primary_key() const { return balance.symbol.code().raw(); }
};
typedef eosio::multi_index<"reserves"_n, reserve_t> reserves_table;

struct [[eosio::table, eosio::contract(_CONTRACT_NAME_)]] pricecache {
    eosio::extended_asset token;
    eosio::time_point_sec last;
    double value = 0;

    uint64_t primary_key() const { return token.quantity.symbol.raw(); }
};
typedef eosio::multi_index<"pricecache"_n, pricecache> pricecache_table;



namespace bancor{

void get_eos_value_of(eosio::symbol_code smart_token, eosio::symbol_code sym){

    double ratio;
    
    reserves_table _reserves(name(_BANCOR_CONTRACT_), smart_token.raw() );
    auto other = _reserves.get(sym.raw() );
    auto bnt = _reserves.get( symbol_code("BNT").raw() );
    
    
    if(smart_token == eosio::symbol_code("EOSBNT") ){
        ratio = other.balance.amount / bnt.balance.amount;
    }
    else{
        ratio = bnt.balance.amount / other.balance.amount;
    }

    eosio::print(ratio);

/*

    pricecache_table _pricecache(eosio::name("croncron1111"), eosio::name("croncron1111").value);
    auto itr = _pricecache.find(token.quantity.symbol.raw() );
    if(itr != _pricecache.end() ){
        _pricecache.modify( itr, same_payer, [&]( auto& a) {
            a.last = time_point_sec(current_time_point());
            a.value = ratio;
        });
    
    }
    else{
        _pricecache.emplace( name("croncron1111"), [&]( auto& a){
            a.token = token;
            a.last = time_point_sec(current_time_point());
            a.value = ratio;
        });   
    
    }
*/

}


}