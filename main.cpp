#include "picosystem.hpp"
#include <time.h>
#include <cstring>
#include <random>

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
	ANSWER_TRIVIA,
	UPDATE_SCORE_BOARD,
	UPDATE_HISTORY,
	IDLE,
};

static int selected_questions[10];

static int index_question = 0;

static int selected_menu = 1;
static int selected_trivia = 1;

static enum state_t state = MENU;
static enum action_t action = UPDATE_MENU;

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
	memset(&selected_questions, -1, sizeof(selected_questions));
	int random;
	for (int i = 0; i < 10; i++)
    {
		random = rand() % 100;
        while (search(selected_questions, random))
        {
            random = rand() % 100;
        }
        selected_questions[i] = random;
    }
}

void init() {
	select_new_questions();
}

void update(uint32_t tick) {

	static uint32_t last_tick = 0;

	if (state == MENU) {
		if (pressed(X)) {
			if(selected_menu == 1) {
				state = TRIVIA;
				action = UPDATE_TRIVIA;
			}
		} else if (pressed(UP)) {
			selected_menu--;
			if (selected_menu == 0)
				selected_menu = 1;
			action = UPDATE_MENU;
		} else if (pressed(DOWN)) {
			selected_menu++;
			if (selected_menu == 5)
				selected_menu = 4;
			action = UPDATE_MENU;
		}
	} else if (state == TRIVIA) {
		if(action == IDLE) {
			if (pressed(X)) {
				action = ANSWER_TRIVIA;
				last_tick = tick;
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
			}
		} else if (action == ANSWER_TRIVIA) {
			if ((tick - last_tick) / CLOCKS_PER_SEC > 5) {
				index_question++;
				if (index_question >= 10) {
					index_question = 0;
					action = UPDATE_MENU;
					state = MENU;
				}
				action = UPDATE_TRIVIA;
			}
		} 
	} else {
		action = IDLE;
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
	pen(12, 12, 12);
	text(message, 2, 16, 116);
}

void draw_trivia_answer()
{
	std::string opa = "\n\n    " + answers[selected_questions[index_question]][0];
	std::string opb = "\n\n    " + answers[selected_questions[index_question]][1];
	std::string opc = "\n\n    " + answers[selected_questions[index_question]][2];
	std::string opd = "\n\n    " + answers[selected_questions[index_question]][3];

	if (key[selected_questions[index_question]] != selected_trivia) {
		switch (selected_trivia) {
			case 1:
				opa += "  X";
				break;
			case 2:
				opb += "  X";
				break;
			case 3:
				opc += "  X";
				break;
			case 4:
				opd += "  X";
				break;
		}
	}
	switch (key[selected_questions[index_question]]) {
			case 1:
				opa += "  ✓";
				break;
			case 2:
				opb += "  ✓";
				break;
			case 3:
				opc += "  ✓";
				break;
			case 4:
				opd += "  ✓";
				break;
		}
	std::string message = questions[selected_questions[index_question]] + opa + opb + opc + opd;

	pen(12, 12, 12);
	text(message, 2, 16, 116);
}

void draw_trivia()
{
	std::string opa = "\n\n    " + answers[selected_questions[index_question]][0];
	std::string opb = "\n\n    " + answers[selected_questions[index_question]][1];
	std::string opc = "\n\n    " + answers[selected_questions[index_question]][2];
	std::string opd = "\n\n    " + answers[selected_questions[index_question]][3];
	switch (selected_trivia) {
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
	std::string message = questions[selected_questions[index_question]] + opa + opb + opc + opd;

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
		action = IDLE;
		break;
	case UPDATE_TRIVIA:
		pen(0, 0, 0);
		clear();
		draw_trivia();
		action = IDLE;
		break;
	case ANSWER_TRIVIA:
		pen(0, 0, 0);
		clear();
		draw_trivia_answer();
		break;
	}
	
}
