INSERT INTO market.items (item_id, category_id, title) VALUES
(5, 1, 'Шкаф');

INSERT INTO market.items (item_id, category_id, title) VALUES
(4, 0, 'IPhone XR');

INSERT INTO market.categories (category_id, title, items_amount, offers_amount) VALUES
(2, 'Спортивное питание', 0, 0);

DELETE FROM market.categories WHERE category_id = 2;

INSERT INTO market.shops (shop_id, rating, street, house_number, insurance_number) VALUES
(2, NULL, 'Кропоткинская', 7, 4321);

UPDATE market.shops
    SET street = 'Ленинская',
        house_number = 5
WHERE shop_id = 2;

INSERT INTO market.offers (offer_id, publish_dt, item_id, shop_id, rating, amount, price, status) VALUES
(6, '2012-30-12 9:30:00', 0, 2, NULL, 2, 1299, TRUE);

UPDATE market.offers
    SET amount = 0,
        status = FALSE
WHERE offer_id = 6;

UPDATE market.offers
    SET status = FALSE
WHERE offer_id = 6;

DELETE FROM market.offers WHERE status = FALSE;
