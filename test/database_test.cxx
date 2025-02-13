#include <StormByte/database/sqlite/sqlite3.hxx>
#include <StormByte/database/sqlite/prepared_stmt.hxx>
#include <StormByte/database/sqlite/row.hxx>
#include <StormByte/database/sqlite/result.hxx>
#include <StormByte/database/sqlite/exception.hxx>
#include <StormByte/system/system.hxx>
#include <memory>
#include <iostream>

#include "test_handlers.h"

using namespace StormByte::Database::SQLite;

class TestDatabase : public SQLite3 {
	public:
		TestDatabase() : SQLite3() {
			init_database();
		}
	
		void post_init_action() noexcept override {
			// Create tables
			silent_query("CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, email TEXT NOT NULL UNIQUE);");
			silent_query("CREATE TABLE products (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, price REAL NOT NULL);");
			silent_query("CREATE TABLE orders (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, product_id INTEGER, quantity INTEGER NOT NULL, FOREIGN KEY (user_id) REFERENCES users(id), FOREIGN KEY (product_id) REFERENCES products(id));");
	
			// Insert data
			silent_query("INSERT INTO users (name, email) VALUES ('Alice', 'alice@example.com');");
			silent_query("INSERT INTO users (name, email) VALUES ('Bob', 'bob@example.com');");
			silent_query("INSERT INTO products (name, price) VALUES ('Laptop', 999.99);");
			silent_query("INSERT INTO products (name, price) VALUES ('Mouse', 19.99);");
			silent_query("INSERT INTO orders (user_id, product_id, quantity) VALUES (1, 1, 1);");
			silent_query("INSERT INTO orders (user_id, product_id, quantity) VALUES (2, 2, 2);");
		}

		std::vector<std::shared_ptr<Row>> get_users() {
			auto stmt = prepare_sentence("select_users", "SELECT name, email FROM users;");
			std::vector<std::shared_ptr<Row>> rows;
			while (std::shared_ptr<Row> r = stmt->Step())
				rows.push_back(r);
			return rows;
		}
	
		std::vector<std::shared_ptr<Row>> get_products() {
			auto stmt = prepare_sentence("select_products", "SELECT name, price FROM products;");
			std::vector<std::shared_ptr<Row>> rows;
			while (std::shared_ptr<Row> r = stmt->Step())
				rows.push_back(r);
			return rows;
		}
	
		std::vector<std::shared_ptr<Row>> get_orders() {
			auto stmt = prepare_sentence("select_orders", "SELECT user_id, product_id, quantity FROM orders;");
			std::vector<std::shared_ptr<Row>> rows;
			while (std::shared_ptr<Row> r = stmt->Step())
				rows.push_back(r);
			return rows;
		}
	
		std::vector<std::shared_ptr<Row>> get_joined_data() {
			auto stmt = prepare_sentence("select_join", "SELECT users.name, products.name, orders.quantity FROM orders JOIN users ON orders.user_id = users.id JOIN products ON orders.product_id = products.id;");
			std::vector<std::shared_ptr<Row>> rows;
			while (std::shared_ptr<Row> r = stmt->Step())
				rows.push_back(r);
			return rows;
		}
};

// Create an in-memory database for testing (global)
TestDatabase db;

int verify_inserted_users() {
	// 1. Verify that users were inserted correctly
	auto rows = db.get_users();
	ASSERT_EQUAL("verify_inserted_users", 2, rows[0]->Columns());
	ASSERT_EQUAL("verify_inserted_users", "Alice", rows[0]->At(0)->Value<std::string>());
	ASSERT_EQUAL("verify_inserted_users", "alice@example.com", rows[0]->At(1)->Value<std::string>());

	ASSERT_EQUAL("verify_inserted_users", 2, rows[1]->Columns());
    ASSERT_EQUAL("verify_inserted_users", "Bob", rows[1]->At(0)->Value<std::string>());
    ASSERT_EQUAL("verify_inserted_users", "bob@example.com", rows[1]->At(1)->Value<std::string>());

	return 0;
}

int verify_inserted_products() {
	// 2. Verify that products were inserted correctly
	auto rows = db.get_products();
	ASSERT_EQUAL("verify_inserted_products", 2, rows[0]->Columns());
	ASSERT_EQUAL("verify_inserted_products", "Laptop", rows[0]->At(0)->Value<std::string>());
	ASSERT_EQUAL("verify_inserted_products", 999.99, rows[0]->At(1)->Value<double>());

	ASSERT_EQUAL("verify_inserted_products", 2, rows[1]->Columns());
	ASSERT_EQUAL("verify_inserted_products", "Mouse", rows[1]->At(0)->Value<std::string>());
	ASSERT_EQUAL("verify_inserted_products", 19.99, rows[1]->At(1)->Value<double>());

	return 0;
}

int verify_inserted_orders() {
	// 3. Verify that orders were inserted correctly
	auto rows = db.get_orders();
	ASSERT_EQUAL("verify_inserted_orders", 3, rows[0]->Columns());
	ASSERT_EQUAL("verify_inserted_orders", 1, rows[0]->At(0)->Value<int>());
	ASSERT_EQUAL("verify_inserted_orders", 1, rows[0]->At(1)->Value<int>());
	ASSERT_EQUAL("verify_inserted_orders", 1, rows[0]->At(2)->Value<int>());

	ASSERT_EQUAL("verify_inserted_orders", 3, rows[1]->Columns());
	ASSERT_EQUAL("verify_inserted_orders", 2, rows[1]->At(0)->Value<int>());
	ASSERT_EQUAL("verify_inserted_orders", 2, rows[1]->At(1)->Value<int>());
	ASSERT_EQUAL("verify_inserted_orders", 2, rows[1]->At(2)->Value<int>());

	return 0;
}

int verify_relationships() {
	// 4. Verify that the relationship between tables works correctly
	auto rows = db.get_joined_data();
	ASSERT_EQUAL("verify_relationships", 3, rows[0]->Columns());
	ASSERT_EQUAL("verify_relationships", "Alice", rows[0]->At(0)->Value<std::string>());
	ASSERT_EQUAL("verify_relationships", "Laptop", rows[0]->At(1)->Value<std::string>());
	ASSERT_EQUAL("verify_relationships", 1, rows[0]->At(2)->Value<int>());

	ASSERT_EQUAL("verify_relationships", 3, rows[1]->Columns());
	ASSERT_EQUAL("verify_relationships", "Bob", rows[1]->At(0)->Value<std::string>());
	ASSERT_EQUAL("verify_relationships", "Mouse", rows[1]->At(1)->Value<std::string>());
	ASSERT_EQUAL("verify_relationships", 2, rows[1]->At(2)->Value<int>());

	return 0;
}

int main() {
    int result = 0;
    try {
		result += verify_inserted_users();
		result += verify_inserted_products();
		result += verify_inserted_orders();
		result += verify_relationships();
        std::cout << "All tests passed successfully.\n";
    } catch (...) {
        result++;
	}

    return result;
}
