### Potential use cases
Use case #1: Private attestation
1. Alice and Bob are coordinating online for some collaboration, coordination, or otherwise some exchange of information, tokens, goods, services, etc.
2. Each party wants confidence that they are communicating with person in control of a specific Telos account
3. Neither party wants to share or leak any information publicly
4. Contact information (multiaddr format) is shared via side channel

Use case #2: Public attestation
Public attestation of any public information, perhaps linking a Telos account to a bitcoin or ethereum address

### Usage Types
1. Vanilla
2. Plaintext [Multiformat](https://multiformats.io/)
3. Hashed Multiformat

#### Vanilla
Vanilla usage is simply exposing a pair with a key and string value pair.

For example: 
- Owner: alice.tk
- Key: bitcoin
- Value: bc1q6wtpe7r8hf8gsmqnhxkpqtd3tmwgjvfchy7jzg

#### Plaintext Multiformat
Multiformat is simply a more portable, self-describing protocol for unique identifiers. It is clearly duplicative to the owner and key, so not really adding value in these specific examples.

For example: 
- Owner: bob.tk
- Key: bitcoin
- Value: /bob.tk/ethereum/0x039653B16b2A5162aEd5BA8B6c379c31d791f6a7

#### Hashed Multiformat
If you want the information to remain private, except for parties you communicate with, you should hash the multiformat value.

If the value is not already a cryptographic hash or address, such as a bitcoin address, you should use a salt calculated from a secret phrase.

The first step is to construct the string that will represent the contact information or attestation details that will be shared via the side channel. This string should contain the Telos account, channel or network, the global identifier, and optionally a salt for additional security. The table below provides more details with examples.
```
/<account>/<channel|network>/<identifier|address>/<salt>
```

| Component             | Description                                                               | Examples                                              |
| --------------------- | ------------------------------------------------------------------------- | ----------------------------------------------------- | 
| account               | Telos account name                                                        |                                                       |
| channel               | Communications channel or network                                         | email, telegram, matrix, ethereum, eos, bitcoin       |
| contact-information   | Specific contact details, globally unique across the specified channel    | <account>@gmail.com, +12125559999, @luv-tk:matrix.org |
| salt (optional)       | Random salt to obfuscate ability to brute force exposure                  |                                                       |

Recommendations:
- use all lowercase letters
- create a salt using the below command with a secret phrase you can remember

Creating a salt from the command line:
```
$ echo "something that I can remember" | sha256sum

6aeb4849509d5bbbfd4a6fe3408a5209f0003af03ce29221c5888aba665c546f -
```
There are also hashing calculators [online](https://emn178.github.io/online-tools/sha256.html), although we do not attest to the validity of any of these.

#### Multiformat examples
alice.tk's email address, using "something that I can remember" as their secret phrase 
```
/alice.tk/email/alice@telos.kitchen/6aeb4849509d5bbbfd4a6fe3408a5209f0003af03ce29221c5888aba665c546f
```

bob.tk's bitcoin and Ethereum addresses, with no secret phrase
```
/bob.tk/bitcoin/bc1q6wtpe7r8hf8gsmqnhxkpqtd3tmwgjvfchy7jzg
/bob.tk/ethereum/0x039653B16b2A5162aEd5BA8B6c379c31d791f6a7
```

charlie.tk's matrix handle and telephone account, using "secret" as their secret phrase
```
/charlie.tk/matrix/@charlie:telos.kitchen/b37e50cedcd3e3f1ff64f4afc0422084ae694253cf399326868e07a35f4a45fb
/charlie.tk/phone/+12125559999/b37e50cedcd3e3f1ff64f4afc0422084ae694253cf399326868e07a35f4a45fb
```

Finally, the multiformat value above should be hashed and the resulting string should be provided as the value to the contract.

``` sh
$ echo "/alice.tk/email/alice@telos.kitchen/6aeb4849509d5bbbfd4a6fe3408a5209f0003af03ce29221c5888aba665c546f" | sha256sum

f8388594d8264fe96bd21b82cd0aeca0eff9c5060f6dd871648915c559499317
```

Alice would invoke the ```set``` action on the contract with the following parameters:
- owner: alice.tk
- key: contact
- value: f8388594d8264fe96bd21b82cd0aeca0eff9c5060f6dd871648915c559499317

Then, when communicating to bob via a side channel (e.g. telegram), she would provide the multiformat string with the salt to bob and he would verify that the hash of that string matches the ```kvs``` table of the ```kv.tk``` contract.  If it matches, he can be sure that owner of the ```alice.tk``` account has attested to ```alice@telos.kitchen``` as her email address.

### Contract details
How the contract works:
- The value of key must be unique for each owner. 
- When calling ```set```, the owner-key-value pair, plus notes, will be added to the kvs table.
- If the owner-key combination exists, the value and notes will be overwritten. 
- The permissions have been removed from the ```kv.tk``` so it is impossible for the code to be updated, ensuring the integrity of the data.

#### 
How to set a KV pair:
##### eosc
``` sh
eosc -u https://api.telos.kitchen --vault-file ~/dev/teloskitchen/m.tk.json tx create kv.tk set '{     
    "owner":"<myTelosAccount>",
    "key":"<any eosio::name value, [a-z][1-5][.](1-12)>",    
    "value":"<any string>",
    "notes":"<any string>"}' -p kv.tk
```

##### cleos
``` sh

```

##### telos.bloks.io





 cleos -u https://api.telos.kitchen push action eosio updateauth '{
    "account": "kv.tk",
    "permission": "active",
    "parent": "owner",
    "auth": {
        "keys": [
            {
                "key": "EOS61XWginSWjbe7B357mXLwQaEnh3VGXFf4BVyDiUSZmennrGPDv",
                "weight": 1
            }
        ],
        "threshold": 1,
        "accounts": [
            {
                "permission": {
                    "actor": "kv.tk",
                    "permission": "eosio.code"
                },
                "weight": 1
            }
        ],
        "waits": []
    }
}' -p kv.tk@owner