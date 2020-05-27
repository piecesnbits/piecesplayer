#include <bancortest.hpp>
#include <cron_mining.hpp>

ACTION bancortest::getprice(symbol_code smart_token, symbol_code sym) {
  //require_auth(get_self() );

  // Init the _message table
  bancor::get_eos_value_of(smart_token, sym);

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
