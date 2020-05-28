#include <bancortest.hpp>
#include <cron_mining.hpp>


ACTION bancortest::getcron(asset gas_fee, double t) {
  //require_auth(get_self() );
  //get_cron_reward(eosio::asset gas_fee, double t_mining)
  pair<asset,asset> reward = bancor::get_cron_reward(gas_fee, t);

}
ACTION bancortest::setgasvalue(symbol_code symbol, symbol_code smart_symbol ) {
  require_auth(get_self() );

  gasvalues_table _gasvalues(get_self(), get_self().value);
  auto itr = _gasvalues.find(symbol.raw() );
  check(itr == _gasvalues.end(), "already added");

  _gasvalues.emplace(get_self(), [&](auto& n) {
    n.symbol = symbol;
    n.smart_symbol = smart_symbol;
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
