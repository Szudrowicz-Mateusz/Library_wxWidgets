#pragma once

#include <sqlite3.h>

#include <string>
#include <iostream>
#include <vector>

struct BookInfo {
    int id;
    std::string author;
    std::string title;
    bool isRented;
    std::string graphic;
    std::string ind;
    std::string date;
};

enum choiceOfSearch{
    Book,Author,Index
};

class Database{
private:
    sqlite3* db;

    std::string search;
    choiceOfSearch choice;
public:

    // Construtors
    Database();
    ~Database() = default;

    // Functions
    bool init();

    void setSearch(std::string _search, choiceOfSearch _choice);
    void rentBook(int const _id);

    // Function to return an vector of Books that are valid to search
    std::vector<BookInfo> printSelectedColumns();


};