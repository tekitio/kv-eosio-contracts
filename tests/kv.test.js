const assert = require('assert');
const eoslime = require('eoslime').init('local');
// const eos = eoslime.Provider.eos;

const KV_WASM = '../kv/kv.wasm';
const KV_ABI = '../kv/kv.abi';

describe('KV Pairs', function () {
    // Increase mocha(testing framework) time, otherwise tests fails
    this.timeout(15000);

    let kvContract;
    let kvUser;
    
    before(async () => {
        let accounts = await eoslime.Account.createRandoms(2);
        kvContract = accounts[0];
        kvUser = accounts[1];
        console.log ("KV Contract: ", kvContract.name)
        kvContract = await eoslime.Contract.deploy(KV_WASM, KV_ABI);
        pairsTable = kvContract.kvs;
    });

    beforeEach(async () => {
      
    });

    it('Set a new KV', async () => {
        await kvContract.set(kvUser.name, "mykey", "myvalue", "a bunch of notes", {from: kvUser});
        let pairs = await pairsTable.limit(10).find();
        const pair = pairs[0];

        assert.equal (pair.key, "mykey", "key does not match");
        assert.equal (pair.value, "myvalue", "value does not match");
        assert.equal (pair.notes, "a bunch of notes", "notes do not match");
    });

    it('Should update a value', async () => {
        await kvContract.set(kvUser.name, "mykey", "my second value", "a bunch of notes", {from: kvUser});
        let pairs = await pairsTable.limit(10).find();
        const pair = pairs[0];

        assert.equal (pair.key, "mykey", "key does not match");
        assert.equal (pair.value, "my second value", "replaced value does not match");
        assert.equal (pair.notes, "a bunch of notes", "notes do not match");
    });


    it('Should delete a key', async () => {
        await kvContract.erase (kvUser.name, "mykey", {from: kvUser});
        let pairs = await pairsTable.limit(10).find();
        assert.equal (pairs.length, 0, "key was not removed from the table");
    });
});