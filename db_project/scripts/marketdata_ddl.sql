CREATE SCHEMA market;

CREATE TABLE market.categories (
    category_id INTEGER NOT NULL PRIMARY KEY,
    title VARCHAR(50) NOT NULL,
    items_amount INTEGER CHECK (items_amount >= 0),
    offers_amount INTEGER NOT NULL CHECK (offer_amount >= 0)
);

CREATE TABLE market.shops (
    shop_id INTEGER NOT NULL PRIMARY KEY,
    rating NUMERIC CHECK (rating >= 0 and rating <= 5),
    street VARCHAR(50) NOT NULL,
    house_number INTEGER NOT NULL,
    insurance_number INTEGER
);

CREATE TABLE market.items (
    item_id INTEGER NOT NULL PRIMARY KEY,
    category_id INTEGER NOT NULL,
    title VARCHAR(50) NOT NULL,

    CONSTRAINT fk_item_category_id FOREIGN KEY (category_id)
        REFERENCES market.categories(category_id)
            ON DELETE RESTRICT
            ON UPDATE RESTRICT
);

CREATE TABLE market.offers (
    offer_id INTEGER NOT NULL PRIMARY KEY,
    publish_dt TIMESTAMP WITHOUT TIME ZONE NOT NULL,
    item_id INTEGER NOT NULL,
    shop_id INTEGER NOT NULL,
    rating NUMERIC CHECK (rating >= 0 and rating <= 5),
    amount INTEGER CHECK (amount >= 0),
    price INTEGER CHECK (price > 0),
    status BIT NOT NULL,

    CONSTRAINT fk_offer_item_id FOREIGN KEY (item_id)
        REFERENCES market.items(item_id)
            ON DELETE RESTRICT
            ON UPDATE RESTRICT,

    CONSTRAINT fk_offer_shop_id FOREIGN KEY (shop_id)
        REFERENCES market.shops(shop_id)
            ON DELETE RESTRICT
            ON UPDATE RESTRICT
);

CREATE TABLE market.clients (
    client_id INTEGER NOT NULL PRIMARY KEY,
    firstname VARCHAR(50) NOT NULL,
    surname VARCHAR(50) NOT NULL,
    orders_amount INTEGER CHECK (orders_amount >= 0),
    orders_sum INTEGER CHECK (orders_sum > 0)
);

CREATE TABLE market.clientshistory (
    client_id INTEGER NOT NULL,
    firstname VARCHAR(50) NOT NULL,
    surname VARCHAR(50) NOT NULL,
    orders_amount INTEGER CHECK (orders_amount >= 0),
    orders_sum INTEGER CHECK (orders_sum > 0),
    actual_dt TIMESTAMP WITHOUT TIME ZONE NOT NULL

    CONSTRAINT fk_clients_history_client_id FOREIGN KEY (client_id)
        REFERENCES market.clients(client_id)
            ON DELETE SET NULL
            ON UPDATE CASCADE
);


CREATE TABLE market.orders (
    order_id INTEGER NOT NULL PRIMARY KEY,
    client_id INTEGER NOT NULL,
    order_dt TIMESTAMP WITHOUT TIME ZONE NOT NULL,
    order_cost INTEGER CHECK (order_cost > 0),

    CONSTRAINT fk_orders_client_id FOREIGN KEY (client_id)
        REFERENCES market.clients(client_id)
            ON DELETE RESTRICT
            ON UPDATE RESTRICT
);

CREATE TABLE market.orderlists (
    order_id INTEGER NOT NULL,
    items_amount INTEGER CHECK (items_amount >= 0),
    offer_id INTEGER NOT NULL,

    CONSTRAINT fk_orderlist_order_id FOREIGN KEY (order_id)
        REFERENCES market.orders(order_id)
            ON DELETE RESTRICT
            ON UPDATE RESTRICT,
    
    CONSTRAINT fk_orderlist_offer_id FOREIGN KEY (offer_id)
        REFERENCES market.offers(offer_id)
            ON DELETE RESTRICT
            ON UPDATE RESTRICT
);
