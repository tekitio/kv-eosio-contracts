#include <kv.hpp>

void kv::set ( const name& owner, 
               const name& key, 
               const string& value, 
               const string& notes ) {

   require_auth (owner);

   kv_table kv_t (get_self(), get_self().value);

   if (exists(owner, key)) {
      auto kv_itr = kv_t.find (get_id(owner, key));

      // paranoid programming
      require_auth (kv_itr->owner);
      check (owner == kv_itr->owner, "FATAL: Owner on key [" + kv_itr->owner.to_string() + 
         "] does not match parameterized owner [" + owner.to_string() + "]");

      kv_t.modify (kv_itr, get_self(), [&](auto& k) {
         k.value  = value;
         k.notes  = notes;
      });
   } else {
      kv_t.emplace (get_self(), [&](auto &k) {
         k.id     = kv_t.available_primary_key();
         k.owner  = owner;
         k.key    = key;
         k.value  = value;
         k.notes  = notes;
      });
   }
}

void kv::erase ( const name& owner, const name& key) {

   require_auth (owner);
   kv_table kv_t (get_self(), get_self().value);
   kv_t.erase (kv_t.find(get_id(owner, key)));
}

