#include "installUtils.h"
#include <gtkmm.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <regex>
const std::string v = "0.9.3"; // Current version
int main(int argc, char* argv[]){
	isRoot();
	Gtk::Main kit(argc, argv);
	// Init GUI elements
	Gtk::Window window;
	Gtk::Window popwindow;
	Gtk::Window aboutwindow;
	Gtk::Box vbox(Gtk::ORIENTATION_VERTICAL, 10);
	Gtk::Entry entry;
	Gtk::Button ibutton("Install!");
	Gtk::Button rbutton("Remove!");
	Gtk::Button abutton("About!");
	Gtk::Label label("Input your packages here!");
	Gtk::Label poplabel("");
	Gtk::Label aboutlabel("		🧀 chPackage version " + v + " 🧀\nhttps://github.com/ndrewCheese/chPackage");
	
	// Window properties
	window.set_default_size(250,200);
	window.set_title("chPackage 🧀");
	window.set_position(Gtk::WIN_POS_CENTER);
	window.set_resizable(false);
	popwindow.set_default_size(100,100);
	popwindow.set_title("Package status");
	popwindow.set_position(Gtk::WIN_POS_CENTER);
	popwindow.set_resizable(false);
	aboutwindow.set_default_size(300,50);
	aboutwindow.set_title("About chPackage");
	aboutwindow.set_position(Gtk::WIN_POS_CENTER);
	aboutwindow.set_resizable(false);
	
	// Label handling
    label.set_halign(Gtk::ALIGN_CENTER);
    label.set_margin_top(15);
    vbox.pack_start(label, Gtk::PACK_SHRINK);
    poplabel.set_halign(Gtk::ALIGN_CENTER);
    
    
    // Entry fields
	entry.set_placeholder_text("Enter package name...");
	entry.set_size_request(200, -1);
	entry.set_halign(Gtk::ALIGN_CENTER);
	vbox.pack_start(entry, Gtk::PACK_SHRINK);
    
	// Buttons
	ibutton.signal_clicked().connect(
		sigc::bind(sigc::ptr_fun(&ibuttonClicked), &entry, &popwindow, &poplabel)
	);
	rbutton.signal_clicked().connect(
		sigc::bind(sigc::ptr_fun(&rbuttonClicked), &entry, &popwindow, &poplabel)
	);
	abutton.signal_clicked().connect(
		sigc::bind(sigc::ptr_fun(&abuttonClicked), &aboutwindow)
	);
	ibutton.set_size_request(150, 50);
	ibutton.set_halign(Gtk::ALIGN_CENTER);
	rbutton.set_size_request(150, 50);
	rbutton.set_halign(Gtk::ALIGN_CENTER);
	abutton.set_size_request(150, 20);
	abutton.set_halign(Gtk::ALIGN_CENTER);
	vbox.pack_start(ibutton, Gtk::PACK_SHRINK);
	vbox.pack_start(rbutton, Gtk::PACK_SHRINK);
	vbox.pack_start(abutton, Gtk::PACK_SHRINK);
	
	// Pack everything into windows & show em
	window.add(vbox);
	popwindow.add(poplabel);
	aboutwindow.add(aboutlabel);
	vbox.show_all();
	Gtk::Main::run(window);
	return 0;
}
void ibuttonClicked(Gtk::Entry* entry, Gtk::Window* popwindow, Gtk::Label* poplabel){ 
	std::string text = entry->get_text();
	std::string name = checkDistro();
	system("> install_log.txt");
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
	system("> install_log.txt");
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
		install = "sudo apt-get install -y " + text;
	}
	else if (name == "Arch Linux" || name == "CachyOS" || name == "endeavourOS"){
		install = "sudo pacman -S --noconfirm " + text;
	}
	else {
		std::cout << "Unsupported Linux distribution." << std::endl;
		exit(1);
	}
	install += " 2> install_log.txt";
	std::cout << install << std::endl;
	system(install.c_str());
}
void removePkg(std::string name, std::string text){
	std::string remove;
	if (name == "Debian GNU/Linux" || name == "Ubuntu" || name == "Linux Mint"){
		remove = "sudo apt-get remove -y " + text;
	}
	else if (name == "Arch Linux" || name == "CachyOS" || name == "endeavourOS"){
		remove = "sudo pacman -R --noconfirm " + text;
	}
	else {
		std::cout << "Unsupported Linux distribution." << std::endl;
		exit(1);
	}
	remove += " 2> install_log.txt";
	std::cout << remove << std::endl;
	system(remove.c_str());
}
bool isFileEmpty(std::ifstream& file) {
    return file.peek() == std::ifstream::traits_type::eof();
}
void readErr(Gtk::Label* poplabel){
	std::string s;
	std::ifstream f("install_log.txt");
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
