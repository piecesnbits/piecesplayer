
#pragma once
#include <eosio/eosio.hpp>
//#include <eosio/action.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/time.hpp>
#include <eosio/singleton.hpp>
#include <math.h>


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




double get_ratio(eosio::symbol_code smart_token, eosio::symbol_code sym){

    double ratio;
    
    reserves_table _reserves(eosio::name(_BANCOR_CONTRACT_), smart_token.raw() );
    auto other = _reserves.get(sym.raw() );
    auto bnt = _reserves.get( eosio::symbol_code("BNT").raw() );
    ratio = (bnt.balance.amount/pow(10, bnt.balance.symbol.precision() ) ) / (other.balance.amount/pow(10, other.balance.symbol.precision() ) );

    eosio::print("\n"+ to_string(ratio) );
    return ratio;

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
eosio::asset get_cron_reward(eosio::asset gas_fee){
    double gas_in_eos;
    if(gas_fee.symbol.code() == eosio::symbol_code("EOS")){
        gas_in_eos = gas_fee.amount/pow(10, gas_fee.symbol.precision() );
    }
    else{
        //check if cached price is still valid 
        double token = bancor::get_ratio(eosio::symbol_code("BNTBOD"), eosio::symbol_code("BOID"));
        double eos = bancor::get_ratio(eosio::symbol_code("EOSBNT"), eosio::symbol_code("EOS") );
        gas_in_eos = token/eos;
        //modify price table  
    }
    //staked*inflation_ptc*job_gas_fee_eos*(0/1+(Math.exp(-t*decay_rate) ) );
    eosio::asset sch_stake = eosio::asset(10000000000, symbol(symbol_code("CRON"), 4) ); //1M
    double staked = sch_stake.amount/pow(10, sch_stake.symbol.precision() );
    double decay_rate = 0.04;
    double inflation_pct = 0.002;
    double t = 50;

    double t_component = 0/1+(exp(-t*decay_rate) );

    eosio::asset cron_reward = eosio::asset(10000, symbol(symbol_code("CRON"), 4) )*(staked*decay_rate*gas_in_eos*t_component);
    return cron_reward;

}


}