#include <file/file.h++>
#include <iostream>
#include <print/print.h++>

using namespace std;

int main() {
    try {
        vik::print("Starting the program");

        string input_file, output_file;

        cout << "Enter input file path: ";
        getline(cin, input_file);
        vik::print("Got input file path as --> " + input_file);

        cout << "Enter output file path: ";
        getline(cin, output_file);
        vik::print("Got output file path as --> " + output_file);

        USBdrive_flasher USB_flasher(output_file, input_file);
        size_t size_flashed = USB_flasher.initiate_flashing();

        cout << "Flashed " << size_flashed/1024 << " KB of data" << std::endl;
    } catch (const exception &e) {
        cout << "[CAUGHT EXCEPTION] " << e.what() << endl;
    }
}