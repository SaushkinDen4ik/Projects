-- Выводит число активных офферов, которые имеют оценку больше 4
SELECT COUNT(*)
FROM market.offers
WHERE rating >= 4 and status = TRUE;

-- Выводит имя/фамилию клиентов и отклонение их трат на маркете от среднего значения, а также сортирует по тратам
SELECT
    firstname,
    surname,
    orders_sum,
    orders_sum - AVG(orders_sum) AS deviation_from_average
FROM market.clients
ORDER BY orders_sum;

-- Выводит название категорий и число товаров в этой категории и сортирует их по количеству товаров
SELECT title, COUNT(*) AS items_in_category
FROM
    market.categories INNER JOIN market.items
    ON market.categories.category_id = market.items.category_id
GROUP BY title
ORDER BY items_in_category;

-- Агрегирующая
-- Для каждого оффера выводит отклонение его стоимости от средней стоимости офферов соответствующего товара
SELECT
    item_id,
    offer_id,
    price,
    AVG(price) OVER (PARTITION BY item_id) AS average
FROM market.offers;

-- Ранжирующая
-- Ранжируем строки с составом заказа по самому заказу
SELECT
    order_id,
    offer_id,
    amount,
    DENSE_RANK() OVER (ORDER BY order_id) AS order_dense_rank 
FROM market.offerlists;

-- Смещения
-- Смещение в количестве заказов и затраченной суммы на покупки
SELECT
    client_id,
    firstname,
    surname,
    actual_dt,
    orders_amount,
    LAG(orders_amount, 1, 0) OVER (PARTITION BY client_id ORDER BY actual_dt DESC) AS prev_amount,
    orders_sum,
    LAG(orders_sum, 1, 0) OVER (PARTITION BY client_id ORDER BY actual_dt DESC) AS prev_sum
FROM market.clientshistory;
