#pragma once

#include "DatabaseManager.h"
#include "AddRecordForm.h"
#include <string>
#include <msclr/marshal_cppstd.h>
#include <vcclr.h> 
#include <map>

namespace VideohostingDBMS {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MainForm : public System::Windows::Forms::Form {
    public:
        MainForm(void) {
            InitializeComponent();
            dbManager = new DatabaseManager();
        }

    protected:
        ~MainForm() {
            if (components) {
                delete components;
            }
            if (dbManager) {
                delete dbManager;
                dbManager = nullptr;
            }
        }

    private:
        System::Windows::Forms::DataGridView^ dataGridView1;
        System::Windows::Forms::ComboBox^ TablesComboBox;
        System::Windows::Forms::Button^ connectButton;
        System::Windows::Forms::Button^ addButton;
        System::Windows::Forms::Button^ deleteButton;
        System::Windows::Forms::TextBox^ dbPathTextBox;
        System::Windows::Forms::Label^ dbPathLabel;
        System::ComponentModel::Container^ components;

        DatabaseManager* dbManager;

        System::String^ currentTablePK;

        void InitializeComponent(void) {

            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->dbPathLabel = (gcnew System::Windows::Forms::Label());
            this->dbPathTextBox = (gcnew System::Windows::Forms::TextBox());
            this->connectButton = (gcnew System::Windows::Forms::Button());
            this->TablesComboBox = (gcnew System::Windows::Forms::ComboBox());
            this->addButton = (gcnew System::Windows::Forms::Button());
            this->deleteButton = (gcnew System::Windows::Forms::Button());
            this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
            this->SuspendLayout();
            // 
            // dbPathLabel
            // 
            this->dbPathLabel->Location = System::Drawing::Point(10, 15);
            this->dbPathLabel->Name = L"dbPathLabel";
            this->dbPathLabel->Size = System::Drawing::Size(120, 20);
            this->dbPathLabel->TabIndex = 0;
            this->dbPathLabel->Text = L"Путь к базе данных:";
            // 
            // dbPathTextBox
            // 
            this->dbPathTextBox->Location = System::Drawing::Point(130, 12);
            this->dbPathTextBox->Name = L"dbPathTextBox";
            this->dbPathTextBox->Size = System::Drawing::Size(500, 22);
            this->dbPathTextBox->TabIndex = 1;
            this->dbPathTextBox->Text = L"videoDB.db";
            // 
            // connectButton
            // 
            this->connectButton->Location = System::Drawing::Point(640, 10);
            this->connectButton->Name = L"connectButton";
            this->connectButton->Size = System::Drawing::Size(120, 25);
            this->connectButton->TabIndex = 2;
            this->connectButton->Text = L"Подключиться";
            this->connectButton->Click += gcnew System::EventHandler(this, &MainForm::connectButton_Click);
            // 
            // TablesComboBox
            // 
            this->TablesComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->TablesComboBox->Items->AddRange(gcnew cli::array<System::Object^>(5) {
                L"users",
                L"channels",
                L"subscriptions",
                L"videos",
                L"liked_videos",
            });
            this->TablesComboBox->Location = System::Drawing::Point(10, 50);
            this->TablesComboBox->Name = L"TablesComboBox";
            this->TablesComboBox->Size = System::Drawing::Size(250, 24);
            this->TablesComboBox->TabIndex = 3;
            this->TablesComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::TablesComboBox_SelectedIndexChanged);
            // 
            // addButton
            // 
            this->addButton->Enabled = false;
            this->addButton->Location = System::Drawing::Point(400, 48);
            this->addButton->Name = L"addButton";
            this->addButton->Size = System::Drawing::Size(120, 25);
            this->addButton->TabIndex = 5;
            this->addButton->Text = L"Добавить запись";
            this->addButton->Click += gcnew System::EventHandler(this, &MainForm::addButton_Click);
            // 
            // deleteButton
            // 
            this->deleteButton->Enabled = false;
            this->deleteButton->Location = System::Drawing::Point(530, 48);
            this->deleteButton->Name = L"deleteButton";
            this->deleteButton->Size = System::Drawing::Size(120, 25);
            this->deleteButton->TabIndex = 6;
            this->deleteButton->Text = L"Удалить выбранное";
            this->deleteButton->Click += gcnew System::EventHandler(this, &MainForm::deleteButton_Click);
            // 
            // dataGridView1
            // 
            this->dataGridView1->AllowUserToAddRows = false;
            this->dataGridView1->AllowUserToDeleteRows = false;
            this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
            this->dataGridView1->ColumnHeadersHeight = 29;
            this->dataGridView1->Location = System::Drawing::Point(10, 85);
            this->dataGridView1->Name = L"dataGridView1";
            this->dataGridView1->ReadOnly = true;
            this->dataGridView1->RowHeadersWidth = 51;
            this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView1->Size = System::Drawing::Size(760, 465);
            this->dataGridView1->TabIndex = 7;
            this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView1->RowsDefaultCellStyle->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView1->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::EnableResizing;
            this->dataGridView1->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::None;
            // 
            // MainForm
            // 
            this->ClientSize = System::Drawing::Size(782, 553);
            this->Controls->Add(this->dbPathLabel);
            this->Controls->Add(this->dbPathTextBox);
            this->Controls->Add(this->connectButton);
            this->Controls->Add(this->TablesComboBox);
            this->Controls->Add(this->addButton);
            this->Controls->Add(this->deleteButton);
            this->Controls->Add(this->dataGridView1);
            this->Name = L"MainForm";
            this->Padding = System::Windows::Forms::Padding(10);
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"SQLite Database Viewer";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }

        System::Void connectButton_Click(System::Object^ sender, System::EventArgs^ e) {
            try {
                std::string dbPath = msclr::interop::marshal_as<std::string>(dbPathTextBox->Text);

                if (dbManager->connect(dbPath)) {

                    TablesComboBox->SelectedItem = L"users";
                    addButton->Enabled = true;
                    deleteButton->Enabled = true;

                }
                else {
                    MessageBox::Show("Не удалось подключиться к базе данных!", "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void addButton_Click(System::Object^ sender, System::EventArgs^ e) {
            try {
                String^ tableName = TablesComboBox->SelectedItem->ToString();
                std::string table = msclr::interop::marshal_as<std::string>(tableName);

                // Получить информацию о столбцах
                std::string columnsQuery = "PRAGMA table_info(" + table + ");";
                auto columnRows = dbManager->executeQuery(columnsQuery);
                std::vector<std::string> columnNames;
                for (const auto& row : columnRows) {
                    if (row.size() >= 2) {
                        columnNames.push_back(row[1]);
                    }
                }

                if (columnNames.empty()) {
                    MessageBox::Show("Не удалось получить информацию о столбцах таблицы.", "Ошибка");
                    return;
                }

                // Фильтрация столбцов и сбор данных для ComboBox
                std::vector<std::string> filteredColumns;
                Dictionary<String^, List<String^>^>^ dropdownData = gcnew Dictionary<String^, List<String^>^>();

                if (table == "users") {
                    // Только username вводится вручную
                    for (const auto& col : columnNames) {
                        if (col == "username") filteredColumns.push_back(col);
                    }
                }
                else if (table == "channels") {
                    // channel_name и creator_id (выбор из существующих user_id)
                    for (const auto& col : columnNames) {
                        if (col == "channel_name" || col == "creator_id") filteredColumns.push_back(col);
                    }
                    // Получаем список user_id для ComboBox
                    auto userIds = dbManager->getColumnValues("users", "user_id");
                    List<String^>^ userList = gcnew List<String^>();
                    for (const auto& id : userIds) {
                        userList->Add(gcnew String(id.c_str()));
                    }
                    dropdownData["creator_id"] = userList;
                }
                else if (table == "subscriptions") {
                    // Выбор из существующих channel_id и user_id
                    for (const auto& col : columnNames) {
                        if (col == "channel_id" || col == "user_id") filteredColumns.push_back(col);
                    }
                    // Получаем channel_id и user_id
                    auto channelIds = dbManager->getColumnValues("channels", "channel_id");
                    auto userIds = dbManager->getColumnValues("users", "user_id");
                    List<String^>^ channelList = gcnew List<String^>();
                    List<String^>^ userList = gcnew List<String^>();
                    for (const auto& id : channelIds) channelList->Add(gcnew String(id.c_str()));
                    for (const auto& id : userIds) userList->Add(gcnew String(id.c_str()));
                    dropdownData["channel_id"] = channelList;
                    dropdownData["user_id"] = userList;
                }
                else if (table == "videos") {
                    // video_name, preview_image, number_of_views, channel_id (выбор из существующих)
                    for (const auto& col : columnNames) {
                        if (col == "video_name" || col == "preview_image" || col == "number_of_views" || col == "channel_id") {
                            filteredColumns.push_back(col);
                        }
                    }
                    // Получаем channel_id
                    auto channelIds = dbManager->getColumnValues("channels", "channel_id");
                    List<String^>^ channelList = gcnew List<String^>();
                    for (const auto& id : channelIds) channelList->Add(gcnew String(id.c_str()));
                    dropdownData["channel_id"] = channelList;
                }
                else if (table == "liked_videos") {
                    // Выбор из существующих video_id и user_id
                    for (const auto& col : columnNames) {
                        if (col == "video_id" || col == "user_id") filteredColumns.push_back(col);
                    }
                    // Получаем video_id и user_id
                    auto videoIds = dbManager->getColumnValues("videos", "video_id");
                    auto userIds = dbManager->getColumnValues("users", "user_id");
                    List<String^>^ videoList = gcnew List<String^>();
                    List<String^>^ userList = gcnew List<String^>();
                    for (const auto& id : videoIds) videoList->Add(gcnew String(id.c_str()));
                    for (const auto& id : userIds) userList->Add(gcnew String(id.c_str()));
                    dropdownData["video_id"] = videoList;
                    dropdownData["user_id"] = userList;
                }
                else {
                    // Для других таблиц используем все столбцы
                    filteredColumns = columnNames;
                }

                if (filteredColumns.empty()) {
                    MessageBox::Show("Нет доступных столбцов для ввода данных.", "Ошибка");
                    return;
                }

                // Преобразовать в управляемый список
                List<String^>^ columnNamesList = gcnew List<String^>();
                for (const auto& name : filteredColumns) {
                    columnNamesList->Add(gcnew String(name.c_str()));
                }

                // Показать форму для ввода данных
                AddRecordForm^ addForm = gcnew AddRecordForm(columnNamesList, dropdownData);
                if (addForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    List<String^>^ values = addForm->Values;
                    std::vector<std::string> params;
                    for each (String ^ value in values) {
                        params.push_back(msclr::interop::marshal_as<std::string>(value));
                    }

                    // Сформировать SQL-запрос
                    std::string columnsPart = "";
                    std::string valuesPart = "";
                    for (size_t i = 0; i < filteredColumns.size(); ++i) {
                        if (i > 0) {
                            columnsPart += ", ";
                            valuesPart += ", ";
                        }
                        columnsPart += filteredColumns[i];
                        valuesPart += "?";
                    }

                    std::string sql = "INSERT INTO " + table + " (" + columnsPart + ") VALUES (" + valuesPart + ")";
                    if (!dbManager->executeNonQuery(sql, params)) {
                        MessageBox::Show("Ошибка добавления записи!", "Ошибка");
                    }
                    else {
                        MessageBox::Show("Запись добавлена успешно!", "Успех");
                        TablesComboBox_SelectedIndexChanged(nullptr, nullptr); // Обновить данные
                    }
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка: " + ex->Message, "Ошибка",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void deleteButton_Click(System::Object^ sender, System::EventArgs^ e) {
            try {
                if (dataGridView1->SelectedRows->Count == 0) {
                    MessageBox::Show("Выберите запись для удаления", "Предупреждение");
                    return;
                }

                if (String::IsNullOrEmpty(currentTablePK)) {
                    MessageBox::Show("Не удалось определить первичный ключ таблицы!", "Ошибка");
                    return;
                }

                if (MessageBox::Show("Вы уверены, что хотите удалить выбранную запись?",
                    "Подтверждение", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::No) {
                    return;
                }

                DataGridViewRow^ selectedRow = dataGridView1->SelectedRows[0];
                Object^ pkValue = selectedRow->Cells[currentTablePK]->Value;

                if (pkValue == nullptr) {
                    MessageBox::Show("Не удалось получить значение первичного ключа!", "Ошибка");
                    return;
                }

                std::string table = msclr::interop::marshal_as<std::string>(TablesComboBox->SelectedItem->ToString());
                std::string valueStr = msclr::interop::marshal_as<std::string>(pkValue->ToString());

                std::string sql = "DELETE FROM " + table + " WHERE " +
                    msclr::interop::marshal_as<std::string>(currentTablePK->ToString()) + " = ?";
                std::vector<std::string> params = { valueStr };

                if (!dbManager->executeNonQuery(sql, params)) {
                    MessageBox::Show("Ошибка удаления записи!", "Ошибка");
                }
                else {
                    MessageBox::Show("Запись удалена успешно!", "Успех");
                    TablesComboBox_SelectedIndexChanged(nullptr, nullptr); // Обновить данные
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка: " + ex->Message, "Ошибка");
            }
        }

        System::Void TablesComboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            try {
                if (!dbManager->isOpen()) {
                    MessageBox::Show("База данных не подключена!", "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return;
                }

                std::string viewName = msclr::interop::marshal_as<std::string>(TablesComboBox->SelectedItem->ToString());

                // Получить информацию о первичном ключе
                std::string pkQuery = "PRAGMA table_info(" + viewName + ");";
                std::vector<std::vector<std::string>> rows = dbManager->executeQuery(pkQuery);

                // Определить имя первичного ключа
                std::string pkName;
                for (const auto& row : rows) {
                    if (row.size() >= 6 && row[5] == "1") { // Столбец 'pk' равен 1
                        pkName = row[1]; // Имя столбца
                        break;
                    }
                }

                currentTablePK = pkName.empty() ? "" : gcnew String(pkName.c_str());

                // Загрузить данные в DataGridView
                if (dbManager->fillDataGridViewFromTable(dataGridView1, viewName)) {
                    // Успешно загружено
                }
                else {
                    MessageBox::Show("Не удалось загрузить данные из таблицы!", "Ошибка");
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("Ошибка: " + ex->Message, "Ошибка");
            }
        }
    };
}