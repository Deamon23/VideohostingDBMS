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
        List<Button^>^ browseButtons = gcnew List<Button^>(); // Для кнопок выбора файла

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

                // TextBox или элементы для preview_image
                if (columnName == "preview_image") {
                    TextBox^ textBox = gcnew TextBox();
                    textBox->Location = System::Drawing::Point(150, yPosition);
                    textBox->Width = 150;
                    textBox->ReadOnly = true;
                    textBoxes->Add(textBox);

                    Button^ browseButton = gcnew Button();
                    browseButton->Text = "Обзор...";
                    browseButton->Location = System::Drawing::Point(310, yPosition);
                    browseButton->Tag = textBox; // Связываем кнопку с TextBox
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

            // Адаптируем размер формы
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
                textBox->Text = openFileDialog->FileName; // Сохраняем путь к файлу
            }
        }

        void OnOkClick(Object^ sender, EventArgs^ e) {
            bool hasEmptyPreview = false;
            for each(TextBox ^ textBox in textBoxes) {
                if (textBox->Text == "") {
                    // Проверка, является ли текущее поле foreign key (translation_id)
                    String^ columnName = labels[textBoxes->IndexOf(textBox)]->Text;
                    if (columnName == "translation_id:") {
                        textBoxValues->Add("NULL"); // Отправляем NULL для foreign key
                    }
                    else {
                        textBoxValues->Add(""); // Для остальных полей — пустая строка
                    }
                }
                else {
                    textBoxValues->Add(textBox->Text);
                }
            }

            if (hasEmptyPreview) {
                MessageBox::Show("Выберите изображение для preview_image!", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
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