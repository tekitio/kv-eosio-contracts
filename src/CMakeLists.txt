project(kv)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( kv kv kv.cpp )
target_include_directories( kv PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( kv ${CMAKE_SOURCE_DIR}/../ricardian )