#include <gtkmm.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <regex>
void isRoot();                           // We need root for installing packages
std::string checkDistro();               // Use the correct package manager
bool isFileEmpty(std::ifstream& file);   // For readErr();
void readErr(Gtk::Label* poplabel);      // Get error from install_log.txt
void removePkg(std::string name, std::string text); // Remove said packages
void installPkg(std::string name, std::string text); // Install said packages
void abuttonClicked(Gtk::Window* aboutwindow); // For the about button
void rbuttonClicked(Gtk::Entry* entry, Gtk::Window* popwindow, Gtk::Label* poplabel); // For the remove button
void ibuttonClicked(Gtk::Entry* entry, Gtk::Window* popwindow, Gtk::Label* poplabel); // For the install button

void ibuttonClicked(Gtk::Entry* entry, Gtk::Window* popwindow, Gtk::Label* poplabel){ 
	std::string text = entry->get_text();
	std::string name = checkDistro();
	system("> /tmp/install_log.txt");
	popwindow->resize(200,100); // Pkg names resize the window to fit content but don't resize it back
	if (std::all_of(text.begin(), text.end(), ::isspace)){
		poplabel->set_text("No package name entered!");
		popwindow->show_all();
		return;
	} else { 
		installPkg(name, text);
		readErr(poplabel);
		popwindow->show_all();
	}
}
void rbuttonClicked(Gtk::Entry* entry, Gtk::Window* popwindow, Gtk::Label* poplabel){
	std::string text = entry->get_text();
	std::string name = checkDistro();
	system("> /tmp/install_log.txt");
	popwindow->resize(200,100); // Pkg names resize the window to fit content but don't resize it back
	if (std::all_of(text.begin(), text.end(), ::isspace)){
		poplabel->set_text("No package name entered!");
		popwindow->show_all();
		return;
	} else { 
		removePkg(name, text);
		readErr(poplabel);
		popwindow->show_all();
	}
}
void abuttonClicked(Gtk::Window* aboutwindow){
	aboutwindow->show_all();
}
void installPkg(std::string name, std::string text){
	std::string install;
	if (name == "Debian GNU/Linux" || name == "Ubuntu" || name == "Linux Mint"){
		install = "pkexec /usr/bin/apt-get install -y " + text;
	}
	else if (name == "Arch Linux" || name == "CachyOS" || name == "endeavourOS" || name == "Omarchy"){
		install = "pkexec /usr/bin/pacman -S --noconfirm --color never " + text;
	}
	else {
		std::cout << "Unsupported Linux distribution." << std::endl;
		exit(1);
	}
	install += " 2> /tmp/install_log.txt";
	std::cout << install << std::endl;
	system(install.c_str());
}
void removePkg(std::string name, std::string text){
	std::string remove;
	if (name == "Debian GNU/Linux" || name == "Ubuntu" || name == "Linux Mint"){
		remove = "pkexec /usr/bin/apt-get remove -y " + text;
	}
	else if (name == "Arch Linux" || name == "CachyOS" || name == "endeavourOS" || name == "Omarchy"){
		remove = "pkexec /usr/bin/pacman -R --noconfirm --color never " + text;
	}
	else {
		std::cout << "Unsupported Linux distribution." << std::endl;
		exit(1);
	}
	remove += " 2> /tmp/install_log.txt";
	std::cout << remove << std::endl;

	system(remove.c_str());
}
bool isFileEmpty(std::ifstream& file) {
    return file.peek() == std::ifstream::traits_type::eof();
}
void readErr(Gtk::Label* poplabel){
	std::string s;
	std::ifstream f("/tmp/install_log.txt");
	if (!f.is_open()) {
        std::cerr << "Error opening the file!";
        exit(1);
    }
    if (isFileEmpty(f)){
		poplabel->set_text("Success!");
	} else {
		getline(f, s);
		poplabel->set_text(s);
	}
	system("rm /tmp/install_log.txt");
}
std::string checkDistro(){
	std::ifstream stream("/etc/os-release");
    std::string line;
    std::regex nameRegex("^NAME=\"(.*?)\"$");
    std::smatch match;
    std::string name;
    while (std::getline(stream, line)) {
        if (std::regex_search(line, match, nameRegex)) {
            name = match[1].str();
            break;
        }
    }
	return name;
}
void isRoot(){
	if (getuid()){
		 std::cout << "Please run as Root. Exiting." << std::endl;
		 exit(1);
	}
}
