#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>


using namespace eosio;
using namespace std;

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

    // @abi action
    void addorder(account_name buyer, account_name seller, asset total, string desc) {
        // cleos push action commerce.app addorder '["user2","user1","10.99 EOS","magic mouse"]' -p user2
        // cleos push action commerce.app addorder '["user2","user1","10.99 EOS","magic mouse"]' -p user2
        // cleos get table commerce.app commerce.app order
        require_auth(buyer);
        uint64_t p0 = 0;
        uint64_t p1 = 2;


        orderline ol1 = {1,p0,1};
        orderline ol2 = {2,p1,1};
        vector<orderline> olines;
        olines.push_back(ol1);
        olines.push_back(ol2);

        auto iter_product = _product.find(p0);
        eosio_assert(iter_product != _product.end(), "Product 1 does not exist");
        iter_product = _product.find(p1);
        eosio_assert(iter_product != _product.end(), "Product 2 does not exist");
        
        _order.emplace(buyer, [&] (auto& row) {
            row.id = _order.available_primary_key();
            row.buyer = buyer;
            row.seller = seller;
            row.total = total;
            row.desc = desc;
            row.lines = olines;
            row.orderdate = now();
            
        });
        
        eosio::action(
            permission_level{ N(user2), N(active) },
            N(eosio.token), N(transfer),
            std::make_tuple(buyer, seller, "7.0000 EOS", "memo")
         ).send();
    }

    void delorder(uint64_t id) {
        // cleos push action commerce.app delorder '[1]' -p commerce.app
        
        auto iter = _order.find(id);
        print("found id");
        _order.erase(iter);
    }
    
    
    
private:
    // @abi table
    struct product {
        uint64_t id;
        name owner;
        string name;
        string description;
        asset price;
        
        //uint64_t primary_key() const { return id; }
        auto primary_key()const { return id; }
        
        EOSLIB_SERIALIZE(product, (id)(owner)(name)(description)(price));
    };
    
    struct orderline {
        uint64_t id;
        uint64_t product_id;
        uint64_t quantity;
    };
    
    // @abi table order i64
    struct order {
        uint64_t id;
        account_name buyer;
        account_name seller;
        asset total;
        string desc;
        vector<orderline> lines;
        uint64_t orderdate;

        auto primary_key()const { return id;}

        EOSLIB_SERIALIZE(order, (id)(buyer)(seller)(total)(desc)(lines)(orderdate));

    };
    
    

    multi_index<N(product), product> _product;
    multi_index<N(order), order> _order;
    
    
    
};

EOSIO_ABI( commerce, (addproduct)(modproduct)(delproduct)(addorder)(delorder) );
