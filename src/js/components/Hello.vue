<template>
  <div class="Hello">
    {{msg}}

     <div class="product-form">
      <!-- <label for="rowid">ID</label>
      <input name="rowid" v-model="rowid" /><br/> -->
      <label for="name">Name</label>
      <input name="name" v-model="name"/><br/>
      <label for="description">Description</label>
      <input name="description" v-model="description"/><br/>
      <label for="price">Price</label>
      <input name="price" v-model="price"/><br/>
    </div>
      
    <div v-if="editMode">
      <b-button @click="editProduct" size="" variant="primary">Edit Product</b-button>  
      <b-button @click="cancelEdit" size="" variant="danger">Cancel</b-button>  
    </div>
    <div v-else>
      <b-button @click="addProduct" size="" variant="primary">Add Product</b-button>
    </div>

    <div v-if="processing" class="alert alert-light processing" role="alert">
      Processing...
    </div>


    <table  class="table table-bordred table-striped product-table">
      <tr>
        <th>ID</th><th>Seller</th><th>Product</th> <th>Description</th><th>Price</th> <th>Edit</th><th>Delete</th>
      </tr>
      <tr v-for="(row,index) in rows" :key="row.id">
        <td>{{ row.id }}</td>
        <td>{{ row.owner }}</td>
        <td>{{ row.name }}</td>
        <td>{{ row.description }}</td>
        <td>{{ row.price }}</td>
        <td><b-button @click="edit(index)" size="sm" variant="primary">Edit</b-button></td>
        <td><b-button @click="deleteProduct(row.id)" size="sm" variant="danger">X</b-button></td>
      </tr>
    </table>

  </div>
</template>

<script>

import * as Eos from 'eosjs';
import secrets from '../../../config/secrets'

let config = {
  keyProvider:secrets.keys,
  httpEndpoint: secrets.endpoint
};
let eos = Eos(config); // 127.0.0.1:8888
let appcode = 'commerce.app';
let currentUser = 'user1';

export default {
  name: 'hello',
  data() {
    return {
      msg: 'Basic EOS CRUD Example',
      rows:[],
      rowid:0,
      name:'',
      description:'',
      price:'',
      processing:false,
      editMode:false
    };
  },
  created: async function() {
    //this.getProducts();
    //this.getBalance();
    this.checkForScatter();
  },
  methods: {
   checkForScatter: async function() {
      
      document.addEventListener('scatterLoaded', scatterExtension => {
        const scatter = window.scatter;
        console.log("scatter found");

        scatter.getIdentity().then(identity => {
          console.log(identity,"identity found");
          window.identity = identity;
          alert("welcome to eos: " + identity.name);
          const network = {
            blockchain:'eos',
            host:'178.128.144.189', // ( or null if endorsed chainId )
            port:8888, // ( or null if defaulting to 80 )
            chainId:null, // Or null to fetch automatically ( takes longer )
          }
          let eos = scatter.eos(network,Eos.Localnet, {});
          let tableQuery = {
            "json":true,
            "table_key":"uint64_t",
            "scope":appcode,
            "code":appcode,
            "table":"product"
          }
          
          eos.getTableRows(tableQuery)
          .then(res => {
            this.rows = res.rows;
          })
          .catch(err => {
            console.log("error fetching rows: " + err);
          })
          //this.rows = result.rows;
          //let eos = eosscatter;
          //this.getProducts();
        }).catch(error => {
          alert("error getting identity: " + error);
          console.log(error);
        })
      })

    },
    getProducts: async function() {
      let tableQuery = {
        "json":true,
        "table_key":"uint64_t",
        "scope":appcode,
        "code":appcode,
        "table":"product"
      }
      
      let result = await eos.getTableRows(tableQuery);
      this.rows = result.rows;

    },
    addProduct: async function() {
      
      try {
        this.processing = true;
        let response = await eos.transaction({
          actions: [
            {
              account: appcode,
              name: 'addproduct',
              authorization: [{
                actor: currentUser,
                permission: 'active'
              }],
              data: {
                //id: this.rowid,
                owner: currentUser,
                name: this.name,
                description: this.description,
                price: this.price + ' SYS'
                
              }
            }
          ]
        })
        console.log(response)
        let result = await this.getProducts();
        this.processing = false;

      }catch(e) {
        alert(JSON.parse(e).error.what)
        
      }

    },
    deleteProduct: async function(rowid) {
      this.processing = true
      let result = await eos.transaction({
        actions: [
          {
            account: appcode,
            name: 'delproduct',
            authorization: [{
              actor: currentUser,
              permission: 'active'
            }],
            data: {
              id: rowid,
              owner:currentUser
            }

          }
        ]
      })
      let response = await this.getProducts();
      this.processing = false
    },
    editProduct: async function() {
      
      this.processing = true
      let result = await eos.transaction({
        actions: [
          {
            account: appcode,
            name: 'modproduct',
            authorization: [{
              actor: currentUser,
              permission: 'active'
            }],
            data: {
              id: this.rowid,
              owner: currentUser,
              name: this.name,
              description: this.description,
              price: this.price

            }

          }
        ]
      })
      let response = await this.getProducts()
      this.processing = false
      this.resetForm()
      this.editMode = false
    },
    edit: function(index) {
      let currentRow = this.rows[index]
      this.rowid = currentRow.id
      this.name = currentRow.name;
      this.description = currentRow.description
      this.price = currentRow.price
      this.editMode = true;
    },
    resetForm: function() {
      this.rowid = 0
      this.name = "";
      this.description = ""
      this.price = ""
    },
    cancelEdit: function() {
      this.resetForm()
      this.editMode = false
    },
    getBalance: async function() {
      let result = await eos.getCurrencyBalance("eosio.token",currentUser);
      console.log(result);
    }
  }

  
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b983;
}

.product-form {
  margin-top:10px;
}
.product-table {
  margin-top: 50px;
}

.processing {
  margin:auto;
  width:150px
}



</style>
