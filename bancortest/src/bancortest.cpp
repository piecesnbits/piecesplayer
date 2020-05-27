#include <bancortest.hpp>
#include <cron_mining.hpp>

ACTION bancortest::getprice(eosio::name pair, eosio::extended_asset token) {
  //require_auth(get_self() );

  // Init the _message table
  bancor::get_eos_value_of(pair, token);

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
