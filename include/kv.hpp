#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>

using namespace eosio;
using std::string;

CONTRACT kv : public contract
{
public:
   using contract::contract;

   struct [[eosio::table, eosio::contract("kv")]] KV
   {
      uint64_t id;
      name owner;
      name key;
      string value;
      string notes;
      uint64_t primary_key() const { return id; }

      uint64_t by_owner() const { return owner.value; }
      uint64_t by_key() const { return key.value; }

      // timestamps
      time_point created_date = current_time_point();
      time_point updated_date = current_time_point();
      uint64_t by_created() const { return created_date.sec_since_epoch(); }
      uint64_t by_updated() const { return updated_date.sec_since_epoch(); }
   };

   typedef multi_index<"kvs"_n, KV,
                       indexed_by<"bykey"_n, const_mem_fun<KV, uint64_t, &KV::by_key>>,
                       indexed_by<"byowner"_n, const_mem_fun<KV, uint64_t, &KV::by_owner>>,
                       indexed_by<"bycreated"_n, const_mem_fun<KV, uint64_t, &KV::by_created>>,
                       indexed_by<"byupdated"_n, const_mem_fun<KV, uint64_t, &KV::by_updated>>>
       kv_table;

   ACTION set(const name &owner, const name &key, const string &value, const string &notes);
   ACTION erase(const name &owner, const name &key);

   uint64_t get_id(const name &owner, const name &key)
   {
      kv_table kv_t(get_self(), get_self().value);
      auto owner_index = kv_t.get_index<"byowner"_n>();
      auto kv_itr = owner_index.find(owner.value);

      while (kv_itr != owner_index.end() && kv_itr->owner == owner)
      {
         if (kv_itr->key == key)
         {
            return kv_itr->id;
         }
         kv_itr++;
      }
      check(false, "Owner [" + owner.to_string() + "] does not have a pair with key [" + key.to_string() + "]");
      return -1;
   }

   bool exists(const name &owner, const name &key)
   {
      kv_table kv_t(get_self(), get_self().value);
      auto owner_index = kv_t.get_index<"byowner"_n>();
      auto kv_itr = owner_index.find(owner.value);

      while (kv_itr != owner_index.end() && kv_itr->owner == owner)
      {
         if (kv_itr->key == key)
         {
            return true;
         }
         kv_itr++;
      }
      return false;
   }
};