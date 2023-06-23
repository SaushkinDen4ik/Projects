-- Обновляем число товаров в категории при добавлении товара
CREATE TRIGGER update_items_in_category
ON market.items
AFTER INSERT
AS
UPDATE market.categories
SET items = items + 1
WHERE category_id = NEW.category_id;

GO

-- Обновляем число товаров в категории при удалении товара
CREATE TRIGGER update_items_in_category
ON market.items
AFTER DELETE
AS
UPDATE market.categories
SET items = items - 1
WHERE category_id = OLD.category_id;

GO
