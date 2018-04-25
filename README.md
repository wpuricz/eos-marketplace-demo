# vue-starter

> A Vue.js starter kit that lets you focus on more programming and less configruation.

## Build Setup

### TODO: Finish documenting setup
``` bash
# install dependencies
npm install
```

### create a secrets file
this is for keeping the endpoint and the keys needed for signing transactions

*Keys are only stored for ease of demo purposes! Never in production*

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

### serve with hot reload at localhost:8080
npm run dev


