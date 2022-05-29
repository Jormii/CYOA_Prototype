#include "ui.h"
#include "team.h"
#include "input.h"
#include "skillset.h"
#include "combat_engine.h"
#include "gs_combat_engine.h"

void ask_for_skill_update();

void combat_engine_state_ask_for_skill()
{
    ask_for_skill_update();
    display_skills();
    display_combat_state();
}

void ask_for_skill_update()
{
    tb_clear(&(print_window.buffer), NULL);
    tb_clear(&(commands_window.buffer), NULL);

    // Handle input
    const Unit *unit = team_get_unit(&(combat_engine.players_team), gs_ask_for_skill_buffer.current);
    if (input_button_pressed(BUTTON_UP) && gs_ask_for_skill_buffer.skill_index != 0)
    {
        gs_ask_for_skill_buffer.skill_index -= 1;
    }
    else if (input_button_pressed(BUTTON_DOWN) && (gs_ask_for_skill_buffer.skill_index + 1) < unit->skillset->n_actives)
    {
        gs_ask_for_skill_buffer.skill_index += 1;
    }

    if (input_button_pressed(BUTTON_CROSS))
    {
        gs_ask_for_skill_buffer.target_slot = 0;
        gs_state_cb = combat_engine_state_ask_for_skills_target;
    }
    else if (input_button_pressed(BUTTON_CIRCLE))
    {
        if (gs_ask_for_skill_buffer.current != 0)
        {
            // TODO: Remove command from queue
            gs_ask_for_skill_buffer.current -= 1;
            gs_ask_for_skill_buffer.skill_index = 0;
        }
    }
}