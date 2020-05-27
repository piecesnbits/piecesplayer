#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace std;
using namespace eosio;

CONTRACT bancortest : public contract {
  public:
    using contract::contract;

    ACTION getprice(symbol_code smart_token, symbol_code sym);
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
