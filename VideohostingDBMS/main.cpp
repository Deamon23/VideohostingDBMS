#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace VideohostingDBMS;

int main(array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    VideohostingDBMS::MainForm form;
    Application::Run(%form);
}