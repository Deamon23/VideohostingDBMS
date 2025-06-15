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
            this->dbPathLabel->Text = L"���� � ���� ������:";
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
            this->connectButton->Text = L"������������";
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
            this->addButton->Text = L"�������� ������";
            this->addButton->Click += gcnew System::EventHandler(this, &MainForm::addButton_Click);
            // 
            // deleteButton
            // 
            this->deleteButton->Enabled = false;
            this->deleteButton->Location = System::Drawing::Point(530, 48);
            this->deleteButton->Name = L"deleteButton";
            this->deleteButton->Size = System::Drawing::Size(120, 25);
            this->deleteButton->TabIndex = 6;
            this->deleteButton->Text = L"������� ���������";
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
            }
            catch (Exception^ ex) {
                MessageBox::Show("������: " + ex->Message, "������",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void addButton_Click(System::Object^ sender, System::EventArgs^ e) {
            try {
                String^ tableName = TablesComboBox->SelectedItem->ToString();
                std::string table = msclr::interop::marshal_as<std::string>(tableName);

                // �������� ���������� � ��������
                std::string columnsQuery = "PRAGMA table_info(" + table + ");";
                auto columnRows = dbManager->executeQuery(columnsQuery);
                std::vector<std::string> columnNames;
                for (const auto& row : columnRows) {
                    if (row.size() >= 2) {
                        columnNames.push_back(row[1]);
                    }
                }

                if (columnNames.empty()) {
                    MessageBox::Show("�� ������� �������� ���������� � �������� �������.", "������");
                    return;
                }

                // ���������� �������� � ���� ������ ��� ComboBox
                std::vector<std::string> filteredColumns;
                Dictionary<String^, List<String^>^>^ dropdownData = gcnew Dictionary<String^, List<String^>^>();

                if (table == "users") {
                    for (const auto& col : columnNames) {
                        if (col == "username") filteredColumns.push_back(col);
                    }
                }
                else if (table == "channels") {
                    for (const auto& col : columnNames) {
                        if (col == "channel_name" || col == "creator_id") filteredColumns.push_back(col);
                    }
                    auto userIds = dbManager->getColumnValues("users", "user_id");
                    List<String^>^ userList = gcnew List<String^>();
                    for (const auto& id : userIds) {
                        userList->Add(gcnew String(id.c_str()));
                    }
                    dropdownData["creator_id"] = userList;
                }
                else if (table == "subscriptions") {
                    filteredColumns.push_back("channel_name");
                    filteredColumns.push_back("username");

                    auto channelNames = dbManager->getChannelNames();
                    auto usernames = dbManager->getUsernames();

                    List<String^>^ channelList = gcnew List<String^>();
                    for (const auto& name : channelNames) {
                        channelList->Add(gcnew String(name.c_str()));
                    }
                    dropdownData["channel_name"] = channelList;

                    List<String^>^ userList = gcnew List<String^>();
                    for (const auto& name : usernames) {
                        userList->Add(gcnew String(name.c_str()));
                    }
                    dropdownData["username"] = userList;
                }
                else if (table == "videos") {
                    for (const auto& col : columnNames) {
                        if (col == "video_name" || col == "preview_image" || col == "number_of_views" || col == "channel_id") {
                            filteredColumns.push_back(col);
                        }
                    }
                    auto channelIds = dbManager->getColumnValues("channels", "channel_id");
                    List<String^>^ channelList = gcnew List<String^>();
                    for (const auto& id : channelIds) {
                        channelList->Add(gcnew String(id.c_str()));
                    }
                    dropdownData["channel_id"] = channelList;
                }
                else if (table == "liked_videos") {
                    filteredColumns.push_back("video_name");
                    filteredColumns.push_back("username");

                    auto videoNames = dbManager->getVideoNames();
                    auto usernames = dbManager->getUsernames();

                    List<String^>^ videoList = gcnew List<String^>();
                    for (const auto& name : videoNames) {
                        videoList->Add(gcnew String(name.c_str()));
                    }
                    dropdownData["video_name"] = videoList;

                    List<String^>^ userList = gcnew List<String^>();
                    for (const auto& name : usernames) {
                        userList->Add(gcnew String(name.c_str()));
                    }
                    dropdownData["username"] = userList;
                }
                else {
                    filteredColumns = columnNames;
                }

                if (filteredColumns.empty()) {
                    MessageBox::Show("��� ��������� �������� ��� ����� ������.", "������");
                    return;
                }

                List<String^>^ columnNamesList = gcnew List<String^>();
                for (const auto& name : filteredColumns) {
                    columnNamesList->Add(gcnew String(name.c_str()));
                }

                AddRecordForm^ addForm = gcnew AddRecordForm(columnNamesList, dropdownData);
                if (addForm->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                    List<String^>^ values = addForm->Values;
                    std::vector<std::string> params;
                    for each (String ^ value in values) {
                        params.push_back(msclr::interop::marshal_as<std::string>(value));
                    }

                    std::string columnsPart = "";
                    std::string valuesPart = "";
                    std::string sql;

                    if (table == "subscriptions") {
                        // �������������� channel_name � username � channel_id � user_id
                        std::string channelName = params[0];
                        std::string username = params[1];

                        sql = "INSERT INTO subscriptions (channel_id, user_id) "
                            "SELECT c.channel_id, u.user_id "
                            "FROM channels c, users u "
                            "WHERE c.channel_name = ? AND u.username = ?";
                    }
                    else if (table == "liked_videos") {
                        // �������������� video_name � username � video_id � user_id
                        std::string videoName = params[0];
                        std::string username = params[1];

                        sql = "INSERT INTO liked_videos (video_id, user_id) "
                            "SELECT v.video_id, u.user_id "
                            "FROM videos v, users u "
                            "WHERE v.video_name = ? AND u.username = ?";
                    }
                    else {
                        for (size_t i = 0; i < filteredColumns.size(); ++i) {
                            if (i > 0) {
                                columnsPart += ", ";
                                valuesPart += ", ";
                            }
                            columnsPart += filteredColumns[i];
                            valuesPart += "?";
                        }
                        sql = "INSERT INTO " + table + " (" + columnsPart + ") VALUES (" + valuesPart + ")";
                    }

                    if (!dbManager->executeNonQuery(sql, params)) {
                        MessageBox::Show("������ ���������� ������!", "������");
                    }
                    else {
                        MessageBox::Show("������ ��������� �������!", "�����");
                        TablesComboBox_SelectedIndexChanged(nullptr, nullptr); // �������� ������
                    }
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("������: " + ex->Message, "������",
                    MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        System::Void deleteButton_Click(System::Object^ sender, System::EventArgs^ e) {
            try {
                if (dataGridView1->SelectedRows->Count == 0) {
                    MessageBox::Show("�������� ������ ��� ��������", "��������������");
                    return;
                }

                if (String::IsNullOrEmpty(currentTablePK)) {
                    MessageBox::Show("�� ������� ���������� ��������� ���� �������!", "������");
                    return;
                }

                if (MessageBox::Show("�� �������, ��� ������ ������� ��������� ������?",
                    "�������������", MessageBoxButtons::YesNo) == System::Windows::Forms::DialogResult::No) {
                    return;
                }

                // �������� ������� � ��������� �����
                String^ tableName = TablesComboBox->SelectedItem->ToString();
                std::string table = msclr::interop::marshal_as<std::string>(tableName);
                array<String^>^ pkNames = currentTablePK->Split(',');

                // �������� �������� ��������� ������ �� ��������� ������
                DataGridViewRow^ selectedRow = dataGridView1->SelectedRows[0];
                std::string whereClause;
                std::vector<std::string> params;

                for (int i = 0; i < pkNames->Length; ++i) {
                    String^ pkName = pkNames[i]->Trim();
                    Object^ pkValue = selectedRow->Cells[pkName]->Value;

                    if (pkValue == nullptr || pkValue->ToString()->Length == 0) {
                        MessageBox::Show("�� ������� �������� �������� ���������� �����!", "������");
                        return;
                    }

                    std::string colName = msclr::interop::marshal_as<std::string>(pkName);
                    std::string valueStr = msclr::interop::marshal_as<std::string>(pkValue->ToString());

                    if (i > 0) whereClause += " AND ";
                    whereClause += colName + " = ?";
                    params.push_back(valueStr);
                }

                std::string sql = "DELETE FROM " + table + " WHERE " + whereClause + ";";

                if (!dbManager->executeNonQuery(sql, params)) {
                    MessageBox::Show("������ �������� ������!", "������");
                }
                else {
                    MessageBox::Show("������ ������� �������!", "�����");
                    TablesComboBox_SelectedIndexChanged(nullptr, nullptr); // �������� ������
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("������: " + ex->Message, "������");
            }
        }

        System::Void TablesComboBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
            try {
                if (!dbManager->isOpen()) {
                    MessageBox::Show("���� ������ �� ����������!", "������",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                    return;
                }

                std::string tableName = msclr::interop::marshal_as<std::string>(TablesComboBox->SelectedItem->ToString());

                // �������� ���������� � ��������� �����
                std::string pkQuery = "PRAGMA table_info(" + tableName + ");";
                auto rows = dbManager->executeQuery(pkQuery);
                std::vector<std::string> pkColumns;

                for (const auto& row : rows) {
                    if (row.size() >= 6 && row[5] == "1") { // ������� 'pk' ����� 1
                        pkColumns.push_back(row[1]); // ��� �������
                    }
                }

                // ����� �������� ��������� ������
                if (tableName == "subscriptions") {
                    pkColumns = { "channel_id", "user_id" };
                }
                else if (tableName == "liked_videos") {
                    pkColumns = { "video_id", "user_id" };
                }

                // ��������� ��� ���������� ����� (��� ����������)
                currentTablePK = "";
                for (size_t i = 0; i < pkColumns.size(); ++i) {
                    if (i > 0) currentTablePK += ",";
                    currentTablePK += gcnew String(pkColumns[i].c_str());
                }

                // ��������� ������ � DataGridView
                if (dbManager->fillDataGridViewFromTable(dataGridView1, tableName)) {
                    // ������� ���������
                }
                else {
                    MessageBox::Show("�� ������� ��������� ������ �� �������!", "������");
                }
            }
            catch (Exception^ ex) {
                MessageBox::Show("������: " + ex->Message, "������");
            }
        }
    };
}