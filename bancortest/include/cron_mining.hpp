
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
}

eosio::asset get_cron_reward(eosio::asset gas_fee){
    double gas_in_eos;
    if(gas_fee.symbol.code() == eosio::symbol_code("EOS")){
        gas_in_eos = gas_fee.amount/pow(10, gas_fee.symbol.precision() );
    }
    else{
        //check if cached price is still valid 
        double token = get_ratio(eosio::symbol_code("BNTBOD"), eosio::symbol_code("BOID"));
        double eos = get_ratio(eosio::symbol_code("EOSBNT"), eosio::symbol_code("EOS") );
        gas_in_eos = (token/eos)*(gas_fee.amount/pow(10, gas_fee.symbol.precision() ) );
        //modify price table  
    }
    //staked*inflation_ptc*job_gas_fee_eos*(0/1+(Math.exp(-t*decay_rate) ) );
    eosio::asset sch_stake = eosio::asset(200000000000, eosio::symbol(eosio::symbol_code("CRON"), 4) ); //1M
    double staked = sch_stake.amount/pow(10, sch_stake.symbol.precision() );
    double decay_rate = 0.04;
    double inflation_pct = 0.002;
    double t = 50;

    double t_component = 0/1+(exp(-t*decay_rate) );

    eosio::print("t_component");
    eosio::print(t_component);

    eosio::asset cron_reward = eosio::asset(10000, eosio::symbol(eosio::symbol_code("CRON"), 4) )*(staked*decay_rate*gas_in_eos*t_component);
    return cron_reward;

}


}