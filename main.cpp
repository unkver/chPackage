#include "installUtils.h"

const std::string version = "0.9.4";
int main(int argc, char* argv[]){
	//isRoot();
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
	Gtk::Label aboutlabel("chPkg version " + version + "\nhttps://github.com/unkver/chPkg");
	
	// Window properties
	window.set_default_size(250,200);
	window.set_title("chPackage");
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
