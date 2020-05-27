#include <bancortest.hpp>
#include <cron_mining.hpp>

ACTION bancortest::getprice(eosio::symbol_code smart_token, eosio::symbol_code sym) {
  //require_auth(get_self() );

  // Init the _message table
  double token = bancor::get_ratio(smart_token, sym);
  double eos = bancor::get_ratio(symbol_code("EOSBNT"), symbol_code("EOS") );

  double v = token/eos;


  print("\n");
  print(v);

}
ACTION bancortest::getcron(asset gas_fee) {
  //require_auth(get_self() );
  bancor::get_cron_reward(gas_fee);


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
