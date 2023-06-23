-- Данный индекс разумно ввести т.к. в офферы часто будут приходить запросы поиска по товару
CREATE INDEX "offers.item_id"
    ON market.offers
    USING btree
    (item_id);

-- Данный индекс разумно ввести т.к. в офферы часто будут приходить запросы поиска по магазину
CREATE INDEX "offers.shop_id"
    ON market.offers
    USING btree
    (shop_id);

-- Данный индекс разумно ввести т.к. в офферы часто будут приходить запросы поиска по опр. товару из опр. магазина
CREATE INDEX "offers.shopid_itemid"
    ON market.offers
    USING btree
    (shop_id, item_id);

-- Данный индекс разумно ввести т.к. хотим поддержать возможность поиска магазинов на заданной улице
CREATE INDEX "shops.street"
    ON market.shops
    USING btree
    (street);
