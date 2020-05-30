#include <bancortest.hpp>
#include <cron_mining.hpp>


ACTION bancortest::getcron(asset gas_fee, double t) {
  //require_auth(get_self() );
  //get_cron_reward(eosio::asset gas_fee, double t_mining)
  pair<asset,asset> reward = bancor::get_cron_reward(gas_fee, t);

  eosio::print("miner_reward: "+reward.first.to_string()+"\n" );
  eosio::print("rest_reward: "+reward.second.to_string()+"\n" );

}
ACTION bancortest::setgasvalue(symbol_code symbol, symbol_code smart_symbol, double init_value ) {
  require_auth(get_self() );

  gasvalues_table _gasvalues(get_self(), get_self().value);
  auto itr = _gasvalues.find(symbol.raw() );
  check(itr == _gasvalues.end(), "already added");

  _gasvalues.emplace(get_self(), [&](auto& n) {
    n.symbol = symbol;
    n.smart_symbol = smart_symbol;
    n.value = init_value;
  });

  //bancor::get_gas_in_eos_value(eosio::asset gas_fee)

  


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
