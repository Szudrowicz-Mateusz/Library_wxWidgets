#include "../headers/database.h"

Database::Database()
{
    if(init())
        std::cout<<"Your database is working \n";
    else
        std::cerr<<"Your database don't work \n";
}

bool Database::init()
{
    int rc = sqlite3_open("../database/mydatabase.db", &db);

    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << '\n';
        return false;
    }

const char* createGameTableSQL = "CREATE TABLE IF NOT EXISTS Library ("
                                "Id_book INTEGER PRIMARY KEY AUTOINCREMENT,"
                                "Title TEXT,"
                                "Author TEXT,"
                                "Is_rented BOOL DEFAULT FALSE,"
                                "Graphic TEXT,"
                                "Ind TEXT,"
                                "date DATE"
                                ");";

    rc = sqlite3_exec(db, createGameTableSQL, nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    } 

    return true;
}

void Database::setSearch(std::string _search, choiceOfSearch _choice)
{
    search = _search;
    choice = _choice;
}


std::vector<BookInfo> Database::printSelectedColumns() {
    std::string columnName;

    std::vector<BookInfo> nothing;

    switch (choice) {
        case choiceOfSearch::Book:
            columnName = "Title";
            break;
        case choiceOfSearch::Author:
            columnName = "Author";
            break;
        case choiceOfSearch::Index:
            columnName = "Ind";
            break;
        default:
            std::cerr << "Invalid choice of search." << std::endl;
            return nothing;
    }

    if (search.empty())
        search = '*';

    std::string selectSQL = "SELECT * FROM Library WHERE " + columnName + " LIKE  " + "'" + search + "%'";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, selectSQL.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SELECT query: " << sqlite3_errmsg(db) << std::endl;
        return nothing;
    }

    std::vector<BookInfo> results;


while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    BookInfo book;
    book.id = sqlite3_column_int(stmt, 0);
    book.title = (const char*)(sqlite3_column_text(stmt, 1));
    book.author = (const char*)(sqlite3_column_text(stmt, 2));
    book.isRented = sqlite3_column_int(stmt, 3);
    book.graphic = (const char*)(sqlite3_column_text(stmt, 4));
    book.ind = (const char*)(sqlite3_column_text(stmt, 5));
    book.date = (const char*)(sqlite3_column_text(stmt, 6));

    results.push_back(book);
}


    sqlite3_finalize(stmt);

    return results;
}

void Database::rentBook(int const _id) {
    const char* sql = "UPDATE Library SET Is_rented = 1, date = date('now', '+31 days') WHERE Id_book = ?;"; 

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_bind_int(stmt, 1, _id);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Update failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}
