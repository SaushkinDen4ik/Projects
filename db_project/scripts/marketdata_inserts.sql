INSERT INTO market.offers (offer_id, publish_dt, item_id, shop_id, rating, amount, price, status) VALUES
(0, '2012-02-12 11:30:00', 0, 0, 4.5, 5, 999, true),
(1, '2012-07-11 12:30:00', 0, 1, 4.7, 3, 1199, true),
(2, '2012-07-11 13:30:00', 1, 0, 5.0, 2, 199, true),
(3, '2012-04-12 12:30:00', 1, 1, 4.2, 1, 299, true),
(4, '2012-05-10 15:30:00', 2, 0, 3.9, 7, 499, true),
(5, '2012-09-12 19:30:00', 3, 1, 4.0, 4, 499, true);

INSERT INTO market.items (item_id, category_id, title) VALUES
(0, 0, 'IPhone X'),
(1, 1, 'Стул'),
(2, 0, 'Samsung Galaxy S2'),
(3, 1, 'Диван');

INSERT INTO market.categories (category_id, title, items_amount, offers_amount) VALUES
(0,'Техника', 2, 15),
(1, 'Мебель', 2, 7);

INSERT INTO market.shops (shop_id, rating, street, house_number, insurance_number) VALUES
(0, 4.5, 'Бестужевых', 3, 1234),
(1, 4.1, 'Первомайская', 1337);

INSERT INTO market.clients (client_id, firstname, surname, orders_amount, orders_sum) VALUES
(0, 'Денис', 'Саушкин', 1, 999),
(1, 'Онучин', 'Артём', 1, 1199),
(2, 'Василий', 'Пупкин', 2, 998);

INSERT INTO market.clientshistory (client_id, firstname, surname, orders_amount, orders_sum, actual_dt) VALUES
(0, 'Денис', 'Саушкин', 1, 999, '2012-09-12 19:30:00'),
(1, 'Онучин', 'Артём', 1, 1199, '2012-09-12 15:30:00'),
(2, 'Василий', 'Пупкин', 1, 499, '2012-09-12 18:30:00'),
(2, 'Василий', 'Пупкин', 2, 998, '2012-22-12 20:30:00');

INSERT INTO market.orders (order_id, client_id, order_dt, order_cost) VALUES
(0, 0, '2012-09-12 19:35:00', 999),
(1, 1, '2012-09-12 15:30:00', 1199),
(2, 2, '2012-09-12 18:50:00', 499),
(3, 2, '2012-23-12 17:30:00', 499);

INSERT INTO market.orderlist (order_id, items_amount, offer_id) VALUES
(0, 1, 0),
(1, 1, 1),
(2, 1, 4),
(3, 1, 4);
