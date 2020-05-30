#include <bancortest.hpp>
#include <cron_mining.hpp>


ACTION bancortest::getcron(asset gas_fee, double t, asset cron_stake ) {
  //require_auth(get_self() );
  //get_cron_reward(eosio::asset gas_fee, double t_mining)
  pair<double,double> reward = cron_mining::calc_cron_reward(gas_fee, t, cron_stake);

  asset miner = eosio::asset(reward.first*pow(10,4), eosio::symbol(eosio::symbol_code("CRON"), 4) );
  asset rest = eosio::asset(reward.second*pow(10,4), eosio::symbol(eosio::symbol_code("CRON"), 4) );
  eosio::print("miner_reward: "+miner.to_string()+"\n" );
  eosio::print("rest_reward: "+rest.to_string()+"\n" );

}
ACTION bancortest::setgasvalue(symbol_code symbol, symbol_code smart_symbol, double init_value, bool remove) {
  require_auth(get_self() );
  check(symbol.length() != 0, "Required gas symbol" );
  gasvalues_table _gasvalues(get_self(), get_self().value);
  auto itr = _gasvalues.find(symbol.raw() );

  if(itr == _gasvalues.end() ){
    _gasvalues.emplace(get_self(), [&](auto& n) {
      n.symbol = symbol;
      n.smart_symbol = smart_symbol;
      n.value = init_value;
    });
  }
  else{
    if(remove){
      _gasvalues.erase(itr);
      return;
    }
    _gasvalues.modify( itr, same_payer, [&]( auto& n) {
      n.symbol = symbol;
      n.smart_symbol = smart_symbol;
      n.value = init_value;
      n.last = time_point_sec(0);
    });
  
  }
}
/*
ACTION bancortest::clear() {
  require_auth(get_self());

  messages_table _messages(get_self(), get_self().value);

  // Delete all records in _messages table
  auto msg_itr = _messages.begin();
  while (msg_itr != _messages.end()) {
    msg_itr = _messages.erase(msg_itr);
  }
}
*/
