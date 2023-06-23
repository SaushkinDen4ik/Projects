-- скроем фамилию клиента
CREATE VIEW market.clients_surname_view AS
SELECT
	client_id,
	firstname,
	CONCAT(LEFT(surname, 1), REPEAT('*', LENGTH(surname)-1)) AS surname,
    orders_amount,
    orders_sum
FROM
	market.clients
ORDER BY
	orders_sum DESC;

-- скроем фамилию клиента в таблицы с историей
CREATE VIEW market.clientshistory_surname_view AS
SELECT
	client_id,
	firstname,
	CONCAT(LEFT(surname, 1), REPEAT('*', LENGTH(surname)-1)) AS surname,
    orders_amount,
    orders_sum,
    actual_dt
FROM
	market.clients
ORDER BY
	actual_dt ASC;

-- скроем номер страховки магазина
CREATE VIEW market.shops_insurance_view AS
SELECT
    shop_id,
    rating,
    street,
    house_number,
    CONCAT(LEFT(insurance_number, 1), '***') AS insurance_number,
FROM
    market.shops
ORDER BY
    rating DESC;

-- соединим офферы и магазины, чтобы получить оценку магазина, который выставил данный оффер
CREATE VIEW market.offer_shop_rating AS
SELECT
    market.offers.offer_id AS offer_id,
    market.shops.rating AS shop_rating
FROM
    market.offers AS offers INNER JOIN market.shops AS shops
    ON offers.shop_id = shops.shop_id
ORDER BY shop_rating;

-- по офферу получаем название соответствующего ему товара и категорию этого товара
CREATE VIEW market.offer_item_and_categ AS
SELECT
    market.offers.offer_id AS offer_id,
    market.categories.title AS category_title,
    market.items.title AS item_title
FROM
    market.offers AS offers INNER JOIN market.items AS items
    ON offers.item_id = items.item_id AS offers_items
    INNER JOIN market.categories AS categ
    ON categ.category_id = offers_items.category_id;

-- по заказу высчитываем его стоимость по таблицам состава заказа и офферу
CREATE VIEW market.name AS
SELECT
    market.orders.offer_id,
    SUM(market.offers.price * market.orderslist.amount) AS order_cost
FROM
    market.orders AS ords INNER JOIN market.orderslist AS ordl
    ON ordl.order_id = ords.offer_id AS ords
    INNER JOIN market.offer AS offers
    ON ords.offer_id = offers.offer_id
GROUP BY market.orders.order_id
ORDER BY market.offers.price;
