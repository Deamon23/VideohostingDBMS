#include "DatabaseManager.h"
#include <iostream>
#include <sys/stat.h>

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
        std::cerr << "Ошибка подключения: файл '" << dbPath
            << "' не найден в указанном каталоге" << std::endl;
        isConnected = false;
        return false;
    }
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "Ошибка подключения: "
            << sqlite3_errmsg(db) << std::endl;
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
        std::cerr << "База данных не подключена" << std::endl;
        return resultData;
    }

    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "Ошибка при подготовке запроса: " << sqlite3_errmsg(db) << std::endl;
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
        std::cerr << "Ошибка при выполнении запроса: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return resultData;
}

bool DatabaseManager::executeNonQuery(const std::string& query, const std::vector<std::string>& params) {
    if (!isConnected) {
        std::cerr << "База данных не подключена" << std::endl;
        return false;
    }
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        const std::string& param = params[i];
        if (param == "NULL") {
            sqlite3_bind_null(stmt, i + 1);
        }
        else if (param.rfind("BLOB:", 0) == 0) { // Обработка BLOB
            std::string base64Data = param.substr(5); // Удаляем префикс
            std::string decodedData;
            try {
                array<Byte>^ decodedBytes = Convert::FromBase64String(gcnew String(base64Data.c_str()));
                pin_ptr<Byte> pinnedData = &decodedBytes[0];
                decodedData.assign(reinterpret_cast<const char*>(pinnedData), decodedBytes->Length);
            }
            catch (...) {
                std::cerr << "Ошибка декодирования Base64" << std::endl;
                sqlite3_finalize(stmt);
                return false;
            }
            sqlite3_bind_blob(stmt, i + 1, decodedData.data(), decodedData.size(), SQLITE_STATIC);
        }
        else { // Обработка чисел и текста
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

    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        std::cerr << "Ошибка выполнения запроса: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<std::string> DatabaseManager::getTableImageColumns(const std::string& tableName) {
    std::vector<std::string> imageColumns;
    std::string query = "PRAGMA table_info(" + tableName + ");";
    auto result = executeQuery(query);
    for (const auto& row : result) {
        // row[1] - имя столбца, row[2] - тип
        if (row.size() >= 3 && row[2] == "BLOB") {
            imageColumns.push_back(row[1]);
        }
    }
    return imageColumns;
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

    // Создаем новую DataTable
    System::Data::DataTable^ table = gcnew System::Data::DataTable();

    // Получаем информацию о столбцах таблицы
    std::string columnsQuery = "PRAGMA table_info(" + tableName + ");";
    std::vector<std::string> columnNames;

    auto columnRows = executeQuery(columnsQuery);
    for (const auto& row : columnRows) {
        if (row.size() >= 2) {
            columnNames.push_back(row[1]);
        }
    }

    // Добавляем столбцы в DataTable
    for (const auto& columnName : columnNames) {
        if (columnName == "preview_image") {
            // Настройка столбца для изображений
            System::Windows::Forms::DataGridViewImageColumn^ imageColumn =
                gcnew System::Windows::Forms::DataGridViewImageColumn();
            imageColumn->Name = gcnew String(columnName.c_str());
            imageColumn->HeaderText = gcnew String(columnName.c_str());
            imageColumn->DefaultCellStyle->BackColor = System::Drawing::Color::LightGray;
            imageColumn->DefaultCellStyle->Alignment =
                System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            imageColumn->AutoSizeMode =
                System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
            imageColumn->ImageLayout =
                System::Windows::Forms::DataGridViewImageCellLayout::Stretch; // <--- Ключевое изменение
            dataGridView->Columns->Add(imageColumn);
        }
        else {
            // Обычные столбцы
            System::Windows::Forms::DataGridViewTextBoxColumn^ textColumn =
                gcnew System::Windows::Forms::DataGridViewTextBoxColumn();
            textColumn->Name = gcnew String(columnName.c_str());
            textColumn->HeaderText = gcnew String(columnName.c_str());
            textColumn->AutoSizeMode =
                System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
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
            if (columnNames[i] == "preview_image") {
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
                    catch (Exception^ ex) {
                        Console::WriteLine("Ошибка загрузки изображения: " + ex->Message);
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