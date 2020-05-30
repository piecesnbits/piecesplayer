#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/time.hpp>


using namespace std;
using namespace eosio;

CONTRACT bancortest : public contract {
  public:
    using contract::contract;

    ACTION getcron(asset gas_fee, double t);
    ACTION setgasvalue(symbol_code symbol, symbol_code smart_symbol, double init_value );
    //ACTION clear();

  private:
  /*
    TABLE messages {
      name    user;
      string  text;
      auto primary_key() const { return user.value; }
    };
    typedef multi_index<name("messages"), messages> messages_table;
    */
};
