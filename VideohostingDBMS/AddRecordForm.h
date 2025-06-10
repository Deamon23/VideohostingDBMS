#pragma once

namespace VideohostingDBMS {
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Windows::Forms;

    public ref class AddRecordForm : public Form {
    public:
        AddRecordForm(List<String^>^ columnNames, Dictionary<String^, List<String^>^>^ dropdownData) {
            InitializeComponent();
            this->columnNames = columnNames;
            this->dropdownData = dropdownData;
            InitializeDynamicControls();
        }

        property List<String^>^ Values {
            List<String^>^ get() { return textBoxValues; }
        }

    private:
        List<String^>^ columnNames;
        Dictionary<String^, List<String^>^>^ dropdownData;
        List<Control^>^ controls = gcnew List<Control^>();
        List<String^>^ textBoxValues = gcnew List<String^>();

        void InitializeDynamicControls() {
            int yPosition = 10;
            for each (String ^ columnName in columnNames) {
                // Label
                Label^ label = gcnew Label();
                label->Text = columnName + ":";
                label->Location = System::Drawing::Point(10, yPosition);
                label->AutoSize = true;
                this->Controls->Add(label);

                // ComboBox или TextBox
                if (dropdownData && dropdownData->ContainsKey(columnName)) {
                    ComboBox^ comboBox = gcnew ComboBox();
                    comboBox->Location = System::Drawing::Point(150, yPosition);
                    comboBox->Width = 200;
                    comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
                    for each (String ^ item in dropdownData[columnName]) {
                        comboBox->Items->Add(item);
                    }
                    comboBox->SelectedIndex = 0;
                    controls->Add(comboBox);
                    this->Controls->Add(comboBox); 
                }
                else if (columnName == "preview_image") {
                    TextBox^ textBox = gcnew TextBox();
                    textBox->Location = System::Drawing::Point(150, yPosition);
                    textBox->Width = 150;
                    textBox->ReadOnly = true;

                    Button^ browseButton = gcnew Button();
                    browseButton->Text = "Обзор...";
                    browseButton->Location = System::Drawing::Point(310, yPosition);
                    browseButton->Tag = textBox;
                    browseButton->Click += gcnew EventHandler(this, &AddRecordForm::OnBrowseClick);
                    controls->Add(browseButton); 
                    controls->Add(textBox);
                    this->Controls->Add(browseButton);
                    this->Controls->Add(textBox); 
                }
                else {
                    TextBox^ textBox = gcnew TextBox();
                    textBox->Location = System::Drawing::Point(150, yPosition);
                    textBox->Width = 200;
                    controls->Add(textBox);
                    this->Controls->Add(textBox); 
                }

                yPosition += 30;
            }

            // Кнопки OK и Cancel
            Button^ okButton = gcnew Button();
            okButton->Text = "OK";
            okButton->Location = System::Drawing::Point(150, yPosition);
            okButton->Click += gcnew EventHandler(this, &AddRecordForm::OnOkClick);
            this->Controls->Add(okButton);

            Button^ cancelButton = gcnew Button();
            cancelButton->Text = "Отмена";
            cancelButton->Location = System::Drawing::Point(250, yPosition);
            cancelButton->Click += gcnew EventHandler(this, &AddRecordForm::OnCancelClick);
            this->Controls->Add(cancelButton);

            this->ClientSize = System::Drawing::Size(400, yPosition + 80);
            this->Text = "Добавить запись";
            this->StartPosition = FormStartPosition::CenterParent;
        }

        void OnBrowseClick(Object^ sender, EventArgs^ e) {
            Button^ button = dynamic_cast<Button^>(sender);
            TextBox^ textBox = dynamic_cast<TextBox^>(button->Tag);
            if (!textBox) return;

            OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
            openFileDialog->Filter = "Image Files (*.jpg, *.png)|*.JPG;*.PNG";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                textBox->Text = openFileDialog->FileName;
            }
        }

        void OnOkClick(Object^ sender, EventArgs^ e) {
            for each(Control ^ control in controls) {
                if (ComboBox^ comboBox = dynamic_cast<ComboBox^>(control)) {
                    textBoxValues->Add(comboBox->SelectedItem->ToString());
                }
                else if (TextBox^ textBox = dynamic_cast<TextBox^>(control)) {
                    textBoxValues->Add(textBox->Text);
                }
            }
            this->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->Close();
        }

        void OnCancelClick(Object^ sender, EventArgs^ e) {
            this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Close();
        }

        void InitializeComponent() {
            this->SuspendLayout();
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(400, 300);
            this->Name = "AddRecordForm";
            this->ResumeLayout(false);
        }
    };
}