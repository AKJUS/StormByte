#include <StormByte/database/sqlite/sqlite3.hxx>
#include <StormByte/database/sqlite/prepared_stmt.hxx>
#include <StormByte/database/sqlite/row.hxx>
#include <StormByte/database/sqlite/result.hxx>
#include <StormByte/database/sqlite/exception.hxx>
#include <cassert>
#include <memory>
#include <iostream>

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
	
int main() {
    try {
        // Create an in-memory database for testing
        TestDatabase db;

        // 1. Verify that users were inserted correctly
        auto rows = db.get_users();
        assert(rows[0]->Columns() == 2);
        assert(rows[0]->At(0)->Value<std::string>() == "Alice");
        assert(rows[0]->At(1)->Value<std::string>() == "alice@example.com");

		assert(rows[1]->Columns() == 2);
        assert(rows[1]->At(0)->Value<std::string>() == "Bob");
        assert(rows[1]->At(1)->Value<std::string>() == "bob@example.com");

        // 2. Verify that products were inserted correctly
        rows = db.get_products();
        assert(rows[0]->Columns() == 2);
        assert(rows[0]->At(0)->Value<std::string>() == "Laptop");
        assert(rows[0]->At(1)->Value<double>() == 999.99);

		assert(rows[1]->Columns() == 2);
        assert(rows[1]->At(0)->Value<std::string>() == "Mouse");
        assert(rows[1]->At(1)->Value<double>() == 19.99);

        // 3. Verify that orders were inserted correctly
        rows = db.get_orders();
        assert(rows[0]->Columns() == 3);
        assert(rows[0]->At(0)->Value<int>() == 1);
        assert(rows[0]->At(1)->Value<int>() == 1);
        assert(rows[0]->At(2)->Value<int>() == 1);

		assert(rows[1]->Columns() == 3);
        assert(rows[1]->At(0)->Value<int>() == 2);
        assert(rows[1]->At(1)->Value<int>() == 2);
        assert(rows[1]->At(2)->Value<int>() == 2);

        // 4. Verify that the relationship between tables works correctly
        rows = db.get_joined_data();
        assert(rows[0]->Columns() == 3);
        assert(rows[0]->At(0)->Value<std::string>() == "Alice");
        assert(rows[0]->At(1)->Value<std::string>() == "Laptop");
        assert(rows[0]->At(2)->Value<int>() == 1);

		assert(rows[1]->Columns() == 3);
        assert(rows[1]->At(0)->Value<std::string>() == "Bob");
        assert(rows[1]->At(1)->Value<std::string>() == "Mouse");
        assert(rows[1]->At(2)->Value<int>() == 2);

        std::cout << "All tests passed successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << '\n';
		return 1;
    } catch (const StormByte::Database::SQLite::Exception& e) {
		std::cerr << "An exception occurred: " << e.what() << '\n';
		return 1;
	}

    return 0;
}
