#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Data;

class DatabaseManager {
private:
    sqlite3* db;
    bool isConnected;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect(const std::string& dbPath);
    void disconnect();
    bool isOpen() const { return isConnected; }

    bool fillDataGridViewFromTable(System::Windows::Forms::DataGridView^ dataGridView, const std::string& tableName);
    std::vector<std::vector<std::string>> executeQuery(const std::string& query);
    bool executeNonQuery(const std::string& query, const std::vector<std::string>& params = {});
    //std::vector<std::string> DatabaseManager::getColumnValues(const std::string& tableName, const std::string& columnName);
    std::vector<std::string> DatabaseManager::getColumnValues(const std::string& tableName, const std::string& columnName, const std::string& filterColumn = "");
    std::vector<std::string> getChannelNames();
    std::vector<std::string> getUsernames();
    std::vector<std::string> getVideoNames();
};