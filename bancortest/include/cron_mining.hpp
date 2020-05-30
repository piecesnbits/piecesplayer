
#pragma once
#include <eosio/eosio.hpp>
//#include <eosio/action.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/time.hpp>
#include <eosio/singleton.hpp>
#include <math.h>


#define _CONTRACT_NAME_ "bancortest"
#define _BANCOR_CONTRACT_ "bancorcnvrtr"
#define _SELF_ "piecesplayer"



struct reserve_t {
    name contract;
    uint64_t ratio;
    asset balance;
    uint64_t primary_key() const { return balance.symbol.code().raw(); }
};
typedef eosio::multi_index<"reserves"_n, reserve_t> reserves_table;

struct [[eosio::table, eosio::contract(_CONTRACT_NAME_)]] gasvalues {
    eosio::symbol_code symbol;
    eosio::time_point_sec last;
    double value = 0;
    eosio::symbol_code smart_symbol;
    uint64_t primary_key() const { return symbol.raw(); }
};
typedef eosio::multi_index<"gasvalues"_n, gasvalues> gasvalues_table;



namespace bancor{

    eosio::name self = name(_SELF_);


    double get_ratio(eosio::symbol_code smart_token, eosio::symbol_code sym){

        double ratio;
        
        reserves_table _reserves(eosio::name(_BANCOR_CONTRACT_), smart_token.raw() );
        auto other = _reserves.get(sym.raw() );
        auto bnt = _reserves.get( eosio::symbol_code("BNT").raw() );
        ratio = (bnt.balance.amount/pow(10, bnt.balance.symbol.precision() ) ) / (other.balance.amount/pow(10, other.balance.symbol.precision() ) );
        eosio::print("ratio " + smart_token.to_string() + " " + to_string(ratio) + "\n" );
        return ratio;
    }

    double get_gas_in_eos_value(eosio::asset gas_fee){
        
        double gas_in_eos = 0;
        eosio::time_point_sec now = eosio::time_point_sec(current_time_point());

        //eos
        if(gas_fee.symbol.code() == eosio::symbol_code("EOS")){
            gas_in_eos = gas_fee.amount/pow(10, 4 );
        }
        //other gas tokens
        else{
            gasvalues_table _gasvalues(self, self.value);
            auto itr = _gasvalues.find(gas_fee.symbol.code().raw() );
            if(itr != _gasvalues.end() ){
                if( now < time_point_sec(itr->last.sec_since_epoch() + 60 ) || itr->smart_symbol.length() == 0 ){
                    //if smart_symbol is not supplied return the default value
                    gas_in_eos = (gas_fee.amount/pow(10, gas_fee.symbol.precision() ) )*itr->value;
                }
                else{

                    double eos = get_ratio(eosio::symbol_code("EOSBNT"), eosio::symbol_code("EOS") );
                    double token = get_ratio(itr->smart_symbol, gas_fee.symbol.code() );
                    double value = token/eos;
                    gas_in_eos = (gas_fee.amount/pow(10, gas_fee.symbol.precision() ) )*value;
                    _gasvalues.modify( itr, same_payer, [&]( auto& n) {
                        n.last = now;
                        n.value = value;
                    });
                }
            }
        }
        return gas_in_eos;
    }

    std::pair<eosio::asset, eosio::asset>get_cron_reward(eosio::asset gas_fee, double t_mining){
        double gas_in_eos = get_gas_in_eos_value(gas_fee);

        //if gas in eos is 0 return results immediate todo

        eosio::asset sch_stake = eosio::asset(200000000000, eosio::symbol(eosio::symbol_code("CRON"), 4) ); //20M
        double staked = sch_stake.amount/pow(10, sch_stake.symbol.precision() );
        
        double decay_rate = 0.04;
        double inflation_pct = 0.002;
        double t_component = exp(-t_mining*decay_rate);

        //staked*inflation_ptc*job_gas_fee_eos*Math.exp(-t*decay_rate );
        double base = staked*inflation_pct*gas_in_eos;
        double t_base = base*t_component;
        double rest = base - t_base;


        eosio::asset miner_reward = eosio::asset(t_base*pow(10,4), eosio::symbol(eosio::symbol_code("CRON"), 4) );
        eosio::asset rest_reward = eosio::asset(rest*pow(10,4), eosio::symbol(eosio::symbol_code("CRON"), 4) );
        
        eosio::print("t_component: "+to_string(t_component)+"\n" );
        eosio::print("t_base: "+to_string(t_base)+"\n" );
        eosio::print("rest: "+to_string(rest)+"\n" );
        eosio::print("miner_reward: "+miner_reward.to_string()+"\n" );
        eosio::print("rest_reward: "+rest_reward.to_string()+"\n" );
        return make_pair(miner_reward, rest_reward);
        


    }


}