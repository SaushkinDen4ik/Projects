### **Физическая модель**
-----
#### Таблица Order

| Название | Описание | Тип данных | Ограничение |
|----------|----------|------------|-------------|
|`order_id`| Идентификатор | `INTEGER` | `PRIMARY KEY` |
| `client_id`| Идентификатор клиента |`INTEGER` | `FOREIGN KEY` |
| `order_dt` | Дата заказа | `TIMESTAMP` | `NOT NULL` |
| `order_cost` | Идентификатор клиента | `INTEGER` | `NOT NULL` |

#### Таблица OrderList

| Название | Описание | Тип данных | Ограничение |
|----------|----------|------------|-------------|
|`order_id`| Идентификатор | `INTEGER` | `PRIMARY KEY` |
| `items_amount`| Число товаров в заказе |`INTEGER` | `NOT NULL` |
| `order_id` | Идентификатор заказа | `INTEGER` | `FOREIGN KEY` |

#### Таблица Client

| Название | Описание | Тип данных | Ограничение |
|----------|----------|------------|-------------|
|`client_id`| Идентификатор | `INTEGER` | `PRIMARY KEY` |
| `fisrtname`| Имя клиента |`VARCHAR(50)` | `NOT NULL` |
| `surname` | Фамилия клиента | `VARCHAR(50)` | `NOT NULL` |
| `orders_amount` | Число заказов | `INTEGER` | `NOT NULL` |
| `orders_sum` | Суммарная стоимость заказов | `INTEGER` | `NOT NULL` |

#### Таблица ClientHistory (SCD)

| Название | Описание | Тип данных | Ограничение |
|----------|----------|------------|-------------|
|`client_id`| Идентификатор | `INTEGER` | `PRIMARY KEY` |
| `fisrtname`| Имя клиента |`VARCHAR(50)` | `NOT NULL` |
| `surname` | Фамилия клиента | `VARCHAR(50)` | `NOT NULL` |
| `orders_amount` | Число заказов | `INTEGER` | `NOT NULL` |
| `orders_sum` | Суммарная стоимость заказов | `INTEGER` | `NOT NULL` |
| `actual_dt` | Дата записи | `TIMESTAMP` | `NOT NULL` |

#### Таблица Category

| Название | Описание | Тип данных | Ограничение |
|----------|----------|------------|-------------|
|`category_id`| Идентификатор | `INTEGER` | `PRIMARY KEY` |
| `title`| Название категории |`VARCHAR(50)` | `NOT NULL` |
| `items_amount` | Число товаров в категории | `INTEGER` | `NOT NULL` |
| `offers_amount` | Число офферов в категории | `INTEGER` | `NOT NULL` |

#### Таблица Item

| Название | Описание | Тип данных | Ограничение |
|----------|----------|------------|-------------|
|`item_id`| Идентификатор | `INTEGER` | `PRIMARY KEY` |
| `category_id`| Идентификатор категории |`INTEGER` | `FOREIGN KEY` |
| `title` | Название товара | `VARCHAR(50)` | `NOT NULL` |

#### Таблица Offer

| Название | Описание | Тип данных | Ограничение |
|----------|----------|------------|-------------|
|`offer_id`| Идентификатор | `INTEGER` | `PRIMARY KEY` |
| `publish_dt`| Дата публикации |`TIMESTAMP` | `NOT NULL` |
| `item_id` | Идентификатор товара | `INTEGER` | `FOREIGN KEY` |
| `shop_id` | Идентификатор магазина | `INTEGER` | `FOREIGN KEY` |
| `rating` | Оценка товара | `NUMERIC` | `NOT NULL` |
| `amount` | Количество данного товара в магазине | `INTEGER` | `NOT NULL` |
| `price` | Цена товара | `INTEGER` | `NOT NULL` |
| `status` | Актуален ли оффер сейчас | `BOOLEAN` | `NOT NULL` |

#### Таблица Shop

| Название | Описание | Тип данных | Ограничение |
|----------|----------|------------|-------------|
|`shop_id`| Идентификатор | `INTEGER` | `PRIMARY KEY` |
| `rating`| Оценка магазина |`NUMERIC` | `NOT NULL` |
| `street` | Название улицы, где магазин | `VARCHAR(50)` | `NOT NULL` |
| `house_num` | Номер дома, где магазин | `VARCHAR(50)` | `NOT NULL` |

