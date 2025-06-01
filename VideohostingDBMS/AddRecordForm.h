#pragma once

namespace VideohostingDBMS {
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Windows::Forms;

    public ref class AddRecordForm : public Form {
    public:
        AddRecordForm(List<String^>^ columnNames) {
            InitializeComponent();
            this->columnNames = columnNames;
            InitializeDynamicControls();
        }

        property List<String^>^ Values {
            List<String^>^ get() { return textBoxValues; }
        }

    private:
        List<String^>^ columnNames;
        List<TextBox^>^ textBoxes = gcnew List<TextBox^>();
        List<Label^>^ labels = gcnew List<Label^>();
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
                labels->Add(label);

                // TextBox
                TextBox^ textBox = gcnew TextBox();
                textBox->Location = System::Drawing::Point(150, yPosition);
                textBox->Width = 200;
                this->Controls->Add(textBox);
                textBoxes->Add(textBox);

                yPosition += 30;
            }

            // Button OK
            Button^ okButton = gcnew Button();
            okButton->Text = "OK";
            okButton->Location = System::Drawing::Point(150, yPosition);
            okButton->Click += gcnew EventHandler(this, &AddRecordForm::OnOkClick);
            this->Controls->Add(okButton);

            // Button Cancel
            Button^ cancelButton = gcnew Button();
            cancelButton->Text = "Отмена";
            cancelButton->Location = System::Drawing::Point(250, yPosition);
            cancelButton->Click += gcnew EventHandler(this, &AddRecordForm::OnCancelClick);
            this->Controls->Add(cancelButton);

            // Adjust form size
            this->ClientSize = System::Drawing::Size(400, yPosition + 80);
            this->Text = "Добавить запись";
            this->StartPosition = FormStartPosition::CenterParent;
        }

        void OnOkClick(Object^ sender, EventArgs^ e) {
            for each (TextBox ^ textBox in textBoxes) {
                textBoxValues->Add(textBox->Text);
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
            // 
            // AddRecordForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(400, 300);
            this->Name = "AddRecordForm";
            this->ResumeLayout(false);
        }
    };
}