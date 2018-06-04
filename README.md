

Basic EOS CRUD example using Vue.js and EOS Blockchain

Credit to https://github.com/nsjames/Scatter-Tutorials

### Setup
Requires EOSIO 1.0

``` bash
# install dependencies
git clone git@github.com:wpuricz/eos-commerce.git
cd eos-commerce
npm install
```

#### create a secrets file
this is for keeping the endpoint and the keys needed for signing transactions

*Keys are only stored for ease of demo purposes! Not for production*

    touch config/secrets.js

```
module.exports = {
    endpoint: "http://xxx.xxx.xxx.xxx:8888",
    keys: [
        'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx',
        'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx',
    ]
  };
```

Update the code in Hello.vue to reflect your account, scope, and user

```
let appcode = 'commerce.app';
let currentUser = 'user1';
```

    npm run dev  # Start local vue application

#### Server Deploy

- Create an account for the application (i.e. commerce.app)
- Create an account for the currentuser (i.e user1)
- Deploy the contract


#### serve with hot reload
npm run dev


