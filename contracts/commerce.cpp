#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>

using namespace eosio;
using namespace std;

#include "products.h"
#include "orders.h"

using namespace products;
using namespace orders;

class commerce : public contract {
    using contract::contract;



public:
    commerce( account_name self ) :
    contract(self),
    _order(_self,_self),
    _product(_self,_self){}
    
    // @abi action
    void addproduct( name owner, string name, string description, asset price) {
        // cleos push action commerce.code addproduct '[2,"user1","wireless mouse","mac magic mouse","71.99 EOS"]' -p user1
        // cleos get table commerce.code commerce.code product
        require_auth(owner);
        
        _product.emplace(owner, [&] (auto& row) {
            row.id = _product.available_primary_key();
            row.owner = owner;
            row.name = name;
            row.description = description;
            row.price = price;
            
        });
    }

    // @abi action
    void modproduct(uint64_t id, name owner, string name, string description, asset price) {
        //cleos push action commerce.code modproduct '[2,"user1","wireless mouse","mac magic Mouse","71.99 EOS"]' -p user1
        
        require_auth(owner);
        auto iter = _product.find(id);
        _product.modify( iter, 0, [&]( auto& row) {
            row.id = id;
            row.owner = owner;
            row.name = name;
            row.description = description;
            row.price = price;
        });
    }
    
    // @abi action
    void delproduct(uint64_t id, name owner) {
        // cleos push action commerce.code dropproduct '[1]' -p user1
        // 2018-01-08T01:02:11
        
        require_auth(owner);
        auto iter = _product.find(id);
        _product.erase(iter);
    }

    asset getPrice(uint64_t product_id) {
        auto iter_product = _product.find(product_id);
        eosio_assert(iter_product != _product.end(), "Product ID does not exist");
        return iter_product->price;
    }

    // @abi action
    void addorder(account_name buyer, account_name seller, string desc, vector<cart> shoppingcart) {
        // cleos push action commerce.app addorder '["user2","user1","10.99 EOS","magic mouse",[{"product_id":1,"quantity":2},{"product_id":2,"quantity":1}]]' -p user2
        // cleos push action commerce.app addorder '["user2","user1","10.99 EOS","magic mouse"]' -p user2
        // cleos get table commerce.app commerce.app order
        require_auth(buyer);

        vector<orderline> olines;
        orderline o;
        asset order_total;

        for(int i =0; i < shoppingcart.size();i++) {
            cart c = shoppingcart[i];
            order_total+= getPrice(c.product_id);
            o.id = i+1;
            o.product_id = c.product_id;
            o.quantity = c.quantity;
            olines.push_back(o);
        }
        
        _order.emplace(buyer, [&] (auto& row) {
            row.id = _order.available_primary_key();
            row.buyer = buyer;
            row.seller = seller;
            row.total = order_total;
            row.desc = desc;
            row.lines = olines;
            row.orderdate = now();
            
        });
        // Make Payment
        /*eosio::action(
            permission_level{ N(user2), N(active) },
            N(eosio.token), N(transfer),
            std::make_tuple(buyer, seller, "7.0000 EOS", "memo")
         ).send();*/
    }

    void delorder(uint64_t id) {
        // cleos push action commerce.app delorder '[1]' -p commerce.app
        auto iter = _order.find(id);
        _order.erase(iter);
    }
    
    
    
private:

    multi_index<N(product), product> _product;
    multi_index<N(order), order> _order;
    
};

EOSIO_ABI( commerce, (addproduct)(modproduct)(delproduct)(addorder)(delorder) );
