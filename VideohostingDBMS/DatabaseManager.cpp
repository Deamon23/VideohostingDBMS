#include "DatabaseManager.h"
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <vector>


DatabaseManager::DatabaseManager() : db(nullptr), isConnected(false) {}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool fileExists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

bool DatabaseManager::connect(const std::string& dbPath) {
    disconnect();
    if (!fileExists(dbPath)) {
        MessageBox::Show(gcnew String(("Ошибка подключения: файл '" + dbPath + "' не найден в указанном каталоге").c_str()), "Ошибка",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        isConnected = false;
        return false;
    }
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        MessageBox::Show(gcnew String(("Ошибка подключения: " + std::string(sqlite3_errmsg(db))).c_str()), "Ошибка",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        sqlite3_close(db);
        db = nullptr;
        isConnected = false;
        return false;
    }
    isConnected = true;
    return true;
}

void DatabaseManager::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
    isConnected = false;
}

std::vector<std::vector<std::string>> DatabaseManager::executeQuery(const std::string& query) {
    std::vector<std::vector<std::string>> resultData;
    if (!isConnected) {
        MessageBox::Show("База данных не подключена", "Ошибка",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        return resultData;
    }
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        MessageBox::Show(gcnew String(("Ошибка при подготовке запроса: " + std::string(sqlite3_errmsg(db))).c_str()), "Ошибка",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        return resultData;
    }
    int columnCount = sqlite3_column_count(stmt);
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < columnCount; ++i) {
            switch (sqlite3_column_type(stmt, i)) {
            case SQLITE_INTEGER:
                row.push_back(std::to_string(sqlite3_column_int(stmt, i)));
                break;
            case SQLITE_FLOAT:
                row.push_back(std::to_string(sqlite3_column_double(stmt, i)));
                break;
            case SQLITE_TEXT:
                row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
                break;
            case SQLITE_NULL:
                row.push_back("NULL");
                break;
            default:
                row.push_back("?");
            }
        }
        resultData.push_back(row);
    }
    if (result != SQLITE_DONE) {
        MessageBox::Show(gcnew String(("Ошибка при выполнении запроса: " + std::string(sqlite3_errmsg(db))).c_str()), "Ошибка",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
    sqlite3_finalize(stmt);
    return resultData;
}

std::vector<unsigned char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) return {};
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> buffer(size);
    if (file.read(reinterpret_cast<char*>(buffer.data()), size))
        return buffer;
    return {};
}

bool DatabaseManager::executeNonQuery(const std::string& query, const std::vector<std::string>& params) {
    if (!isConnected) {
        MessageBox::Show("База данных не подключена", "Ошибка",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        return false;
    }
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        MessageBox::Show(gcnew String(("Ошибка подготовки запроса: " + std::string(sqlite3_errmsg(db))).c_str()), "Ошибка",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        return false;
    }
    // Привязка параметров
    for (size_t i = 0; i < params.size(); ++i) {
        const std::string& param = params[i];
        if (param == "NULL") {
            sqlite3_bind_null(stmt, i + 1);
        }
        else {
            // Проверяем, является ли параметр путем к файлу
            if (fileExists(param)) {
                auto blobData = readFile(param);
                if (!blobData.empty()) {
                    sqlite3_bind_blob(stmt, i + 1, blobData.data(), blobData.size(), SQLITE_TRANSIENT);
                }
                else {
                    sqlite3_bind_null(stmt, i + 1);
                }
            }
            else {
                char* end;
                long longVal = strtol(param.c_str(), &end, 10);
                if (*end == '\0') {
                    sqlite3_bind_int(stmt, i + 1, static_cast<int>(longVal));
                }
                else {
                    double doubleVal = std::strtod(param.c_str(), &end);
                    if (*end == '\0') {
                        sqlite3_bind_double(stmt, i + 1, doubleVal);
                    }
                    else {
                        sqlite3_bind_text(stmt, i + 1, param.c_str(), -1, SQLITE_STATIC);
                    }
                }
            }
        }
    }
    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        MessageBox::Show(gcnew String(("Ошибка выполнения запроса: " + std::string(sqlite3_errmsg(db))).c_str()), "Ошибка",
            MessageBoxButtons::OK, MessageBoxIcon::Error);
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::fillDataGridViewFromTable(System::Windows::Forms::DataGridView^ dataGridView,
    const std::string& tableName) {
    if (!isConnected) {
        std::cerr << "База данных не подключена" << std::endl;
        return false;
    }

    // Очистка предыдущих данных
    dataGridView->DataSource = nullptr;
    dataGridView->Columns->Clear();

    // Получаем информацию о столбцах
    std::string columnsQuery = "PRAGMA table_info(" + tableName + ");";
    std::vector<std::string> columnNames;
    std::vector<std::string> displayedColumnNames;
    auto columnRows = executeQuery(columnsQuery);
    for (const auto& row : columnRows) {
        if (row.size() >= 2) {
            std::string columnName = row[1];
            columnNames.push_back(columnName);

            // Исключаем первичные ключи из отображения
            if ((tableName == "users" && columnName == "user_id") ||
                (tableName == "channels" && columnName == "channel_id") ||
                (tableName == "videos" && columnName == "video_id")) {
                continue; // Пропускаем отображение
            }
            displayedColumnNames.push_back(columnName);
        }
    }

    // Добавляем все столбцы (включая первичный ключ как скрытый)
    for (const auto& columnName : columnNames) {
        if (columnName == "preview_image") {
            System::Windows::Forms::DataGridViewImageColumn^ imageColumn =
                gcnew System::Windows::Forms::DataGridViewImageColumn();
            imageColumn->Name = gcnew String(columnName.c_str());
            imageColumn->HeaderText = gcnew String(columnName.c_str());
            imageColumn->ImageLayout = System::Windows::Forms::DataGridViewImageCellLayout::Stretch;
            dataGridView->Columns->Add(imageColumn);
        }
        else {
            System::Windows::Forms::DataGridViewTextBoxColumn^ textColumn =
                gcnew System::Windows::Forms::DataGridViewTextBoxColumn();
            textColumn->Name = gcnew String(columnName.c_str());
            textColumn->HeaderText = gcnew String(columnName.c_str());
            textColumn->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;

            // Скрываем только первичные ключи
            bool isHidden = false;
            if ((tableName == "users" && columnName == "user_id") ||
                (tableName == "channels" && columnName == "channel_id") ||
                (tableName == "videos" && columnName == "video_id")) {
                textColumn->Visible = false;
                isHidden = true;
            }

            dataGridView->Columns->Add(textColumn);
        }
    }

    // Выполняем SQL-запрос для получения данных
    std::string dataQuery = "SELECT * FROM " + tableName + ";";

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, dataQuery.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    int columnCount = sqlite3_column_count(stmt);
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        System::Windows::Forms::DataGridViewRow^ row =
            gcnew System::Windows::Forms::DataGridViewRow();
        row->CreateCells(dataGridView);

        for (int i = 0; i < columnCount; ++i) {
            std::string currentColumnName = columnNames[i];

            if (currentColumnName == "preview_image") {
                const void* blobData = sqlite3_column_blob(stmt, i);
                int blobSize = sqlite3_column_bytes(stmt, i);
                if (blobData && blobSize > 0) {
                    array<Byte>^ managedData = gcnew array<Byte>(blobSize);
                    {
                        pin_ptr<Byte> pinnedData = &managedData[0];
                        std::memcpy(pinnedData, blobData, blobSize);
                    }
                    System::IO::MemoryStream^ ms = gcnew System::IO::MemoryStream(managedData);
                    try {
                        System::Drawing::Image^ image = System::Drawing::Image::FromStream(ms);
                        row->Cells[i]->Value = image;
                    }
                    catch (...) {
                        row->Cells[i]->Value = nullptr;
                    }
                }
                else {
                    row->Cells[i]->Value = nullptr;
                }
            }
            else {
                switch (sqlite3_column_type(stmt, i)) {
                case SQLITE_INTEGER:
                    row->Cells[i]->Value = sqlite3_column_int(stmt, i);
                    break;
                case SQLITE_FLOAT:
                    row->Cells[i]->Value = sqlite3_column_double(stmt, i);
                    break;
                case SQLITE_TEXT:
                    row->Cells[i]->Value = gcnew String(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
                    break;
                case SQLITE_NULL:
                    row->Cells[i]->Value = nullptr;
                    break;
                default:
                    row->Cells[i]->Value = "?";
                }
            }
        }

        dataGridView->Rows->Add(row);
    }

    sqlite3_finalize(stmt);
    return true;
}

std::vector<std::string> DatabaseManager::getColumnValues(const std::string& tableName, const std::string& columnName) {
    std::vector<std::string> values;
    if (!isConnected) return values;
    std::string query = "SELECT " + columnName + " FROM " + tableName + " ORDER BY " + columnName + " ASC;";
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) return values;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        values.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);
    return values;
}