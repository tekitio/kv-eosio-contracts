--- kv Project ---

 - How to Build -
   - cd to 'build' directory
   - run the command 'cmake ..'
   - run the command 'make'

 - After build -
   - The built smart contract is under the 'kv' directory in the 'build' directory
   - You can then do a 'set contract' action with 'cleos' and point in to the './build/kv' directory

 - Additions to CMake should be done to the CMakeLists.txt in the './src' directory and not in the top level CMakeLists.txt

 cleos -u https://api.telos.kitchen push action eosio updateauth '{
    "account": "kv.hypha",
    "permission": "active",
    "parent": "owner",
    "auth": {
        "keys": [
            {
                "key": "EOS56kr5Zfqto7d6cpdy2rA1ThVGkedSXsfzVDcpotJ3KSC4BnL16",
                "weight": 1
            }
        ],
        "threshold": 1,
        "accounts": [
            {
                "permission": {
                    "actor": "kv.hypha",
                    "permission": "eosio.code"
                },
                "weight": 1
            }
        ],
        "waits": []
    }
}' -p kv.hypha@owner