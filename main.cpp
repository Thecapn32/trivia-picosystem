#include "picosystem.hpp"

using namespace picosystem;

int selected_menu = 1;

void init() {
}

void update(uint32_t tick) {
	if (pressed(DOWN)) {
		selected_menu--;
		if (selected_menu == 0)
			selected_menu = 1;
	}

	if (pressed(UP)) {
		selected_menu++;
		if (selected_menu == 5)
			selected_menu = 4;
	}
}

void draw(uint32_t tick) {
	pen(0, 0, 0);
	clear();
	std::string opa = "\n    A) Option A";
	std::string opb = "\n    B) Option B";
	std::string opc = "\n    C) Option C";
	std::string opd = "\n    D) Option D";
	switch (selected_menu) {
		case 1:
			opa += "\\spr001";
			break;
		case 2:
			opb += "\\spr001";
			break;
		case 3:
			opc += "\\spr001";
			break;
		case 4:
			opd += "\\spr001";
			break;
	}
	std::string message = "selected Option \n" + opa + opb + opc + opd; 

	pen(12, 12, 12);
	text(message, 2, 16, 116);
}

