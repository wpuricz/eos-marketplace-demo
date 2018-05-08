
namespace products {
    
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

} 
    
