#pragma once

namespace VideohostingDBMS {
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace System::Windows::Forms;
    using namespace System::IO;

    public ref class AddRecordForm : public Form {
    public:
        AddRecordForm(List<String^>^ columnNames) {
            InitializeComponent();
            this->columnNames = columnNames;
            InitializeDynamicControls();
        }

        property List<String^>^ Values{
            List<String^> ^ get() { return textBoxValues; }
        }

    private:
        List<String^>^ columnNames;
        List<Control^>^ inputControls = gcnew List<Control^>();
        List<String^>^ textBoxValues = gcnew List<String^>();

        void InitializeDynamicControls() {
            int yPosition = 10;
            for each(String ^ columnName in columnNames) {
                Label^ label = gcnew Label();
                label->Text = columnName + ":";
                label->Location = System::Drawing::Point(10, yPosition);
                label->AutoSize = true;
                this->Controls->Add(label);

                if (columnName == "preview_image") {
                    // ������ ��� ������ �����������
                    Button^ imageButton = gcnew Button();
                    imageButton->Text = "������� �����������";
                    imageButton->Location = System::Drawing::Point(150, yPosition);
                    imageButton->Click += gcnew EventHandler(this, &AddRecordForm::OnImageSelect);
                    this->Controls->Add(imageButton);
                    inputControls->Add(imageButton);
                }
                else {
                    // ������� ��������� ����
                    TextBox^ textBox = gcnew TextBox();
                    textBox->Location = System::Drawing::Point(150, yPosition);
                    textBox->Width = 200;
                    this->Controls->Add(textBox);
                    inputControls->Add(textBox);
                }

                yPosition += 30;
            }

            // ������ OK � Cancel
            Button^ okButton = gcnew Button();
            okButton->Text = "OK";
            okButton->Location = System::Drawing::Point(150, yPosition);
            okButton->Click += gcnew EventHandler(this, &AddRecordForm::OnOkClick);
            this->Controls->Add(okButton);

            Button^ cancelButton = gcnew Button();
            cancelButton->Text = "������";
            cancelButton->Location = System::Drawing::Point(250, yPosition);
            cancelButton->Click += gcnew EventHandler(this, &AddRecordForm::OnCancelClick);
            this->Controls->Add(cancelButton);

            this->ClientSize = System::Drawing::Size(400, yPosition + 80);
            this->Text = "�������� ������";
            this->StartPosition = FormStartPosition::CenterParent;
        }

        void OnImageSelect(Object^ sender, EventArgs^ e) {
            OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
            openFileDialog->Filter = "Image Files(*.BMP;*.JPG;*.GIF;*.PNG)|*.BMP;*.JPG;*.GIF;*.PNG";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                String^ filePath = openFileDialog->FileName;
                try {
                    array<Byte>^ imageData = File::ReadAllBytes(filePath);
                    pin_ptr<Byte> pinnedData = &imageData[0];
                    // ����������� � Base64 � ��������� ��� ������������� � executeNonQuery
                    String^ base64 = Convert::ToBase64String(imageData);
                    textBoxValues->Add("BLOB:" + base64); // ������� ��� ���������
                }
                catch (Exception^ ex) {
                    MessageBox::Show("������ ������ �����: " + ex->Message);
                }
            }
        }

        void OnOkClick(Object^ sender, EventArgs^ e) {
            for each(Control ^ control in inputControls) {
                if (control->GetType() == TextBox::typeid) {
                    textBoxValues->Add(safe_cast<TextBox^>(control)->Text);
                }
                else if (control->GetType() == Button::typeid && control->Text == "������� �����������") {
                    // �������� ��� ��������� � OnImageSelect
                    continue;
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
            this->AutoScaleDimensions = Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = Drawing::Size(400, 300);
            this->Name = "AddRecordForm";
            this->ResumeLayout(false);
        }
    };
}