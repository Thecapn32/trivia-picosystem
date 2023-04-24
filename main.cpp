#include "picosystem.hpp"
#include "rand.h"

#include "questions.hpp"

using namespace picosystem;

enum state_t { 
	MENU,
	TRIVIA,
	SCORE_BOARD,
	HISTORY,
};

enum action_t {
	UPDATE_MENU,
	UPDATE_TRIVIA,
	UPDATE_SCORE_BOARD,
	UPDATE_HISTORY,
	IDLE,
};

static int selected_questions[10];

static int index_question = 0;

static int selected_menu = 1;
static int selected_trivia = 1;

static enum state_t state = MENU;
static enum action_t action = IDLE:

int search(int arr[10], int value)
{
    for (int i = 0; i < 10; i++)
    {
        if (value == arr[i])
            return 1;
    }
    return 0;
}

void select_new_questions()
{
	memset(&select_new_questions, -1, sizeof(select_new_questions));
	int rand;
	for (int i = 0; i < 10; i++)
    {
		rand = get_rand_32() % 100;
        while (search(selected_questions, rand))
        {
            rand = get_rand_32() % 100;
        }
        selected_questions[i] = rand;
    }
}

void init() {
	select_new_questions();
}

void update_menu() {
	if (pressed(UP)) {
		selected_menu--;
		if (selected_menu == 0)
			selected_menu = 1;
		action = UPDATE_MENU;
		break;
	}
	if (pressed(DOWN)) {
		selected_menu++;
		if (selected_menu == 5)
			selected_menu = 4;
		action = UPDATE_MENU;
		break;
	}
	action = IDLE;
}

void update_trivia(uint32_t tick) {
	if (pressed(UP)) {
		selected_trivia--;
		if (selected_trivia == 0)
			selected_trivia = 1;
		action = UPDATE_TRIVIA;
		return;
	}
	if (pressed(DOWN)) {
		selected_trivia++;
		if (selected_trivia == 5)
			selected_trivia = 4;
		action = UPDATE_TRIVIA;
		return;
	}
	action = IDLE;
}

void update(uint32_t tick) {
	uint32_t last_tick = 0;

	switch (state)
	{
	case MENU: {
		if (pressed(X)) {
			if(selected_menu == 1) {
				last_tick = tick;
				state = TRIVIA;
				action = UPDATE_TRIVIA;
				break;
			}
		}
		if (pressed(UP)) {
		selected_menu--;
		if (selected_menu == 0)
			selected_menu = 1;
		action = UPDATE_MENU;
		break;
		}
		if (pressed(DOWN)) {
			selected_menu++;
			if (selected_menu == 5)
				selected_menu = 4;
			action = UPDATE_MENU;
			break;
		}
		action = IDLE;
		break;
	}
	case TRIVIA: {
		if (tick - last_tick > 700) {
			index_question++;
			if (index_question > 10) {
				index_question = 0;
				action = UPDATE_MENU;
				state = MENU;
			} else {
				action = UPDATE_TRIVIA;
			}
		} else {
			if (pressed(X)) {
				index_question++;
				if (index_question > 10) {
					index_question = 0;
					action = UPDATE_MENU;
					state = MENU;
				} else {
					last_tick = tick;
					action = UPDATE_TRIVIA;
				}
			} else if (pressed(UP)) {
				selected_trivia--;
				if (selected_trivia == 0)
					selected_trivia = 1;
				action = UPDATE_TRIVIA;
			} else if (pressed(DOWN)) {
				selected_trivia++;
				if (selected_trivia == 5)
					selected_trivia = 4;
				action = UPDATE_TRIVIA;
			} else {
				action = IDLE;
			}
		}
		break;
	}
	default:
		break;
	}
}

void draw_menu()
{
	std::string item1 = "\n\n    1) Play";
	std::string item2 = "\n\n    2) Score Board";
	std::string item3 = "\n\n    3) History";
	std::string item4 = "\n\n    4) Dummy";
	switch(selected_menu) {
		case 1:
			item1 += "  \\spr001";
			break;
		case 2:
			item2 += "  \\spr001";
			break;
		case 3:
			item3 += "  \\spr001";
			break;
		case 4:
			item4 += "  \\spr001";
			break;
	}
	std::string message = "Welcome" + item1 + item2 + item3 + item4;
}

void draw_trivia()
{
	std::string opa = "\n\n    A) Option A";
	std::string opb = "\n\n    B) Option B";
	std::string opc = "\n\n    C) Option C";
	std::string opd = "\n\n    D) Option D";
	switch (selected_menu) {
		case 1:
			opa += "  \\spr001";
			break;
		case 2:
			opb += "  \\spr001";
			break;
		case 3:
			opc += "  \\spr001";
			break;
		case 4:
			opd += "  \\spr001";
			break;
	}
	std::string message = "What is the question?" + opa + opb + opc + opd;

	pen(12, 12, 12);
	text(message, 2, 16, 116);
}

void draw(uint32_t tick) {
	
	switch (action)
	{
	case UPDATE_MENU:
		pen(0, 0, 0);
		clear();
		draw_menu();
		break;
	case UPDATE_TRIVIA:
		pen(0, 0, 0);
		clear();
		draw_trivia();
		break;
	default:
		action = UPDATE_MENU;
		state = MENU;
		break;
	}
	
}
