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
        List<Button^>^ browseButtons = gcnew List<Button^>(); // ��� ������ ������ �����

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

                // TextBox ��� �������� ��� preview_image
                if (columnName == "preview_image") {
                    TextBox^ textBox = gcnew TextBox();
                    textBox->Location = System::Drawing::Point(150, yPosition);
                    textBox->Width = 150;
                    textBox->ReadOnly = true;
                    textBoxes->Add(textBox);

                    Button^ browseButton = gcnew Button();
                    browseButton->Text = "�����...";
                    browseButton->Location = System::Drawing::Point(310, yPosition);
                    browseButton->Tag = textBox; // ��������� ������ � TextBox
                    browseButton->Click += gcnew EventHandler(this, &AddRecordForm::OnBrowseClick);
                    browseButtons->Add(browseButton);
                    this->Controls->Add(browseButton);

                    this->Controls->Add(textBox);
                }
                else {
                    TextBox^ textBox = gcnew TextBox();
                    textBox->Location = System::Drawing::Point(150, yPosition);
                    textBox->Width = 200;
                    textBoxes->Add(textBox);
                    this->Controls->Add(textBox);
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

            // ���������� ������ �����
            this->ClientSize = System::Drawing::Size(400, yPosition + 80);
            this->Text = "�������� ������";
            this->StartPosition = FormStartPosition::CenterParent;
        }

        void OnBrowseClick(Object^ sender, EventArgs^ e) {
            Button^ button = dynamic_cast<Button^>(sender);
            TextBox^ textBox = dynamic_cast<TextBox^>(button->Tag);
            if (!textBox) return;

            OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
            openFileDialog->Filter = "Image Files (*.jpg, *.png)|*.JPG;*.PNG";
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                textBox->Text = openFileDialog->FileName; // ��������� ���� � �����
            }
        }

        void OnOkClick(Object^ sender, EventArgs^ e) {
            bool hasEmptyPreview = false;
            for each(TextBox ^ textBox in textBoxes) {
                if (textBox->Text == "") {
                    // ��������, �������� �� ������� ���� foreign key (translation_id)
                    String^ columnName = labels[textBoxes->IndexOf(textBox)]->Text;
                    if (columnName == "translation_id:") {
                        textBoxValues->Add("NULL"); // ���������� NULL ��� foreign key
                    }
                    else {
                        textBoxValues->Add(""); // ��� ��������� ����� � ������ ������
                    }
                }
                else {
                    textBoxValues->Add(textBox->Text);
                }
            }

            if (hasEmptyPreview) {
                MessageBox::Show("�������� ����������� ��� preview_image!", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
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