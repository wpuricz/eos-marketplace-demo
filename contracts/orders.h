namespace orders {
    
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
    
    struct cart {
        uint64_t product_id;
        uint64_t quantity;
    };
}
