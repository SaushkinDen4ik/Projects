package db_test

import (
	"database/sql"
	"fmt"
	"log"
	"os"
	"testing"
	"time"

	mysql "github.com/go-sql-driver/mysql"
	assert "github.com/tj/assert"
)

var db *sql.DB

// TODO: в светлые времена вынести эти структуры в отдельный файл надо бы
type FirstTestRow struct {
	count int32
}

type SecondTestRow struct {
	firstname         string
	surname           string
	ordersSum         int32
	ordersSumMinusAvg float32
}

type ThirdTestRow struct {
	title           string
	itemsInCategory int32
}

type ForthTestRow struct {
	itemId   int32
	offerId  int32
	price    int32
	avgPrice float32
}

type FifthTestRow struct {
	orderId        int32
	offerId        int32
	amount         int32
	orderDenseRank int32
}

type SixthTestRow struct {
	clientId     int32
	firstname    string
	surname      string
	actualDate   time.Time
	ordersAmount int32
	prevAmount   int32
	ordersSum    int32
	prevSum      int32
}

func ConnectToDB() {
	cfg := mysql.Config{
		User:   os.Getenv("DBUSER"),
		Passwd: os.Getenv("DBPASS"),
		Net:    "tcp",
		Addr:   "127.0.0.1:3306",
		DBName: "market", // todo
	}

	db, err := sql.Open("mysql", cfg.FormatDSN())
	if err != nil {
		log.Fatal(err)
	}

	pingErr := db.Ping()
	if pingErr != nil {
		log.Fatal(pingErr)
	}
	fmt.Println("Connected!")
}

/*
+-------+
| count |
+-------+
|   4   |
+-------+
*/
func TestFirstQuery(t *testing.T) {
	ConnectToDB()

	rows, err := db.Query("SELECT COUNT(*) FROM market.offers WHERE rating >= 4 and status = TRUE;")
	if err != nil {
		t.Fatal("Couldn't complete query")
	}
	defer func() {
		_ = rows.Close()
	}()

	var result []FirstTestRow
	for rows.Next() {
		var row FirstTestRow
		if err := rows.Scan(&row.count); err != nil {
			t.Fatal("Couldn't parse row")
		}
		result = append(result, row)
	}
	if err := rows.Err(); err != nil {
		t.Fatal("Error occurred")
	}

	if len(result) != 1 {
		t.Fatalf("Wrong size: %d, but actual size is 1", len(result))
	}
	if result[0].count != 4 {
		t.Fatalf("Wrong answer: %d, but actual values is 4", result[0].count)
	}
}

/*
	+-----------+---------+-------------+------------------------+
	| firstname | surname | orders_sum  | deviation_from_average |
	+-----------+---------+-------------+------------------------+
	| Ivan      | Ivanov  | 1000        | -1000                  |
	| Denis     | Petrov  | 3000        |  1000                  |
	| Oleg 	    | Petin   | 2000        |  0                	 |
	+-----------+---------+-------------+------------------------+
*/

func TestSecondQuery(t *testing.T) {
	ConnectToDB()

	rows, err := db.Query("SELECT\n" +
		"firstname,\n" +
		"surname,\n" +
		"orders_sum,\n" +
		"orders_sum - AVG(orders_sum) AS deviation_from_average\n" +
		"FROM market.clients\n" +
		"ORDER BY orders_sum;")
	if err != nil {
		t.Fatal("Couldn't complete query")
	}
	defer func() {
		_ = rows.Close()
	}()

	var result []SecondTestRow
	for rows.Next() {
		var row SecondTestRow
		if err := rows.Scan(&row.firstname, &row.surname, &row.ordersSum, &row.ordersSumMinusAvg); err != nil {
			t.Fatal("Couldn't parse row")
		}
		result = append(result, row)
	}
	if err := rows.Err(); err != nil {
		t.Fatal("Error occurred")
	}

	if len(result) != 3 {
		t.Fatalf("Wrong size: %d, but actual size is 3", len(result))
	}

	firstnamesExpected := [3]string{"Ivan", "Denis", "Oleg"}
	surnameExpected := [3]string{"Ivanov", "Petrov", "Petin"}
	ordersSumExpected := [3]int32{1000, 3000, 2000}
	deviationFromAverageExpected := [3]float32{-1000, 1000, 0}
	for i, row := range result {
		assert.Equal(t, row.firstname, firstnamesExpected[i])
		assert.Equal(t, row.surname, surnameExpected[i])
		assert.Equal(t, row.ordersSum, ordersSumExpected[i])
		assert.Equal(t, row.ordersSumMinusAvg, deviationFromAverageExpected)
	}
}

/*
	+--------- +--------- ---------+
	| title    | items_in_category |
	+--------- +--------- +--------+
	| Мебель   |  1                |
	| Телефоны |  2                |
	| Одежда   |  4                |
	+---------+--------------------+
*/
func TestThirdQuery(t *testing.T) {
	ConnectToDB()

	rows, err := db.Query("SELECT title, COUNT(*) AS items_in_category\n" +
		"FROM\n" +
		"market.categories INNER JOIN market.items\n" +
		"ON market.categories.category_id = market.items.category_id\n" +
		"GROUP BY title\n" +
		"ORDER BY items_in_category;")
	if err != nil {
		t.Fatal("Couldn't complete query")
	}
	defer func() {
		_ = rows.Close()
	}()

	var result []ThirdTestRow
	for rows.Next() {
		var row ThirdTestRow
		if err := rows.Scan(&row.title, &row.itemsInCategory); err != nil {
			t.Fatal("Couldn't parse row")
		}
		result = append(result, row)
	}
	if err := rows.Err(); err != nil {
		t.Fatal("Error occurred")
	}

	if len(result) != 3 {
		t.Fatalf("Wrong size: %d, but actual size is 3", len(result))
	}

	titlesExpected := [3]string{"Мебель", "Телефоны", "Одежда"}
	itemsInCategoryExpected := [3]int32{1, 2, 4}
	for i, row := range result {
		assert.Equal(t, row.title, titlesExpected[i])
		assert.Equal(t, row.itemsInCategory, itemsInCategoryExpected[i])
	}
}

/*
	+---------+--------- +----------+---------+
	| item_id | order_id | price    | average |
	+---------+--------- +----------+---------+
	| 0       | 1        | 1000     |  1500   |
	| 0       | 2        | 2000     |  1500   |
	| 1 	  | 0        | 500      |  500    |
	+---------+----------+----------+---------+
*/
func TestForthQuery(t *testing.T) {
	ConnectToDB()

	rows, err := db.Query("SELECT\n" +
		"item_id,\n" +
		"offer_id,\n" +
		"price,\n" +
		"AVG(price) OVER (PARTITION BY item_id) AS average\n" +
		"FROM market.offers;")
	if err != nil {
		t.Fatal("Couldn't complete query")
	}
	defer func() {
		_ = rows.Close()
	}()

	var result []ForthTestRow
	for rows.Next() {
		var row ForthTestRow
		if err := rows.Scan(&row.itemId, &row.offerId, &row.price, &row.avgPrice); err != nil {
			t.Fatal("Couldn't parse row")
		}
		result = append(result, row)
	}
	if err := rows.Err(); err != nil {
		t.Fatal("Error occurred")
	}

	if len(result) != 3 {
		t.Fatalf("Wrong size: %d, but actual size is 3", len(result))
	}

	itemsIdExpected := [3]int32{0, 0, 1}
	offerIdExpected := [3]int32{1, 2, 0}
	pricesExpected := [3]int32{1000, 2000, 500}
	averagePricesExpected := [3]float32{1500, 1500, 500}
	for i, row := range result {
		assert.Equal(t, row.itemId, itemsIdExpected[i])
		assert.Equal(t, row.offerId, offerIdExpected[i])
		assert.Equal(t, row.price, pricesExpected[i])
		assert.Equal(t, row.avgPrice, averagePricesExpected[i])
	}
}

/*
	+----------+----------+----------+------------------+
	| order_id | offer_id | amount   | order_dense_rank |
	+----------+----------+----------+------------------+
	| 0        | 0        | 1        |	1				|
	| 0        | 2        | 1        |	1				|
	| 1 	   | 1        | 2        |	2				|
	| 1 	   | 0        | 1        | 	2				|
	| 2 	   | 2        | 3        |  3               |
	+----------+-----------+---------+------------------+
*/

func TestFifthQuery(t *testing.T) {
	ConnectToDB()

	rows, err := db.Query("SELECT\n" +
		"order_id,\n" +
		"offer_id,\n" +
		"amount,\n" +
		"DENSE_RANK() OVER (ORDER BY order_id) AS order_dense_rank \n" +
		"FROM market.offerlists;")
	if err != nil {
		t.Fatal("Couldn't complete query")
	}
	defer func() {
		_ = rows.Close()
	}()

	var result []FifthTestRow
	for rows.Next() {
		var row FifthTestRow
		if err := rows.Scan(&row.orderId, &row.offerId, &row.amount, &row.orderDenseRank); err != nil {
			t.Fatal("Couldn't parse row")
		}
		result = append(result, row)
	}
	if err := rows.Err(); err != nil {
		t.Fatal("Error occurred")
	}

	if len(result) != 5 {
		t.Fatalf("Wrong size: %d, but actual size is 5", len(result))
	}

	ordersIdExpected := [5]int32{0, 0, 1, 1, 2}
	offersIdExpected := [5]int32{0, 2, 1, 0, 2}
	amountsExpected := [5]int32{1, 1, 2, 1, 3}
	ordersDenseRanksExpected := [5]int32{1, 1, 2, 2, 3}
	for i, row := range result {
		assert.Equal(t, row.orderId, ordersIdExpected[i])
		assert.Equal(t, row.offerId, offersIdExpected[i])
		assert.Equal(t, row.amount, amountsExpected[i])
		assert.Equal(t, row.orderDenseRank, ordersDenseRanksExpected[i])
	}
}

/*
	+-----------+-----------+-----------+----------------+---------------+-------------+------------+----------+
	| client_id | firstname | surname   | actual_dt      | orders_amount | prev_amount | orders_sum | prev_sum |
	+-----------+-----------+-----------+----------------+---------------+-------------+------------+----------+
	| 0         | Lev       | Smirnov   |  2023-01-04    | 1             | 0           | 1000		| 0		   |
	| 0         | Lev       | Smirnov   |  2023-07-04    | 2             | 1           | 3000	    | 1000	   |
	| 1 	    | Stas      | Solodkin  |  2023-02-02    | 1             | 0           | 4000       | 0		   |
	| 1         | Stas      | Solodkin  |  2023-05-03    | 3             | 1           | 6000       | 4000	   |
	| 1         | Stas      | Solodkin  |  2023-11-03    | 4             | 3           | 6500       | 6000     |
	+-----------+-----------+-----------+----------------+---------------+-------------+------------+----------+
*/

func TestSixthQuery(t *testing.T) {
	ConnectToDB()

	rows, err := db.Query("SELECT\n" +
		"client_id, firstname, surname, actual_dt, orders_amount,\n" +
		"LAG(orders_amount, 1, 0) OVER (PARTITION BY client_id ORDER BY actual_dt DESC) AS prev_amount,\n" +
		"orders_sum,\n" +
		"LAG(orders_sum, 1, 0) OVER (PARTITION BY client_id ORDER BY actual_dt DESC) AS prev_sum\n" +
		"FROM market.clientshistory;")
	if err != nil {
		t.Fatal("Couldn't complete query")
	}
	defer func() {
		_ = rows.Close()
	}()

	var result []SixthTestRow
	for rows.Next() {
		var row SixthTestRow
		if err := rows.Scan(&row.clientId, &row.firstname, &row.surname, &row.actualDate,
			&row.ordersAmount, &row.prevAmount, &row.ordersSum, &row.prevSum); err != nil {
			t.Fatal("Couldn't parse row")
		}
		result = append(result, row)
	}
	if err := rows.Err(); err != nil {
		t.Fatal("Error occurred")
	}

	if len(result) != 5 {
		t.Fatalf("Wrong size: %d, but actual size is 5", len(result))
	}

	for i := 1; i < len(result); i++ {
		assert.Equal(t, result[i].prevAmount, result[i-1].ordersAmount)
		assert.Equal(t, result[i].prevSum, result[i-1].ordersSum)
	}
}
