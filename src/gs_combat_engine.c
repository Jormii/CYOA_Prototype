#include "ui.h"
#include "skillset.h"
#include "combat_engine.h"
#include "gs_combat_engine.h"

void gs_combat_engine_start()
{
    ce_start_combat();

    // Change state
    gs_ask_for_skill_buffer.current = 0;
    gs_ask_for_skill_buffer.skill_index = 0;
    gs_state_cb = combat_engine_state_ask_for_skill;
}

void combat_engine_state_start_of_turn()
{
    ce_engine_broadcast(EVENT_START_OF_TURN);

    // Change state
    gs_ask_for_skill_buffer.current = 0;
    gs_ask_for_skill_buffer.skill_index = 0;
    gs_state_cb = combat_engine_state_ask_for_skill;
}

void combat_engine_state_execute()
{
    ce_sort_command_queue();
    ce_execute_command_queue();
    ce_update_combat_state();
    ce_engine_broadcast(EVENT_END_OF_TURN);

    // Change state
    if (combat_engine.in_combat)
    {
        gs_state_cb = combat_engine_state_start_of_turn;
    }
}

void display_skills()
{
    const Unit *unit = team_get_unit(&(combat_engine.players_team), gs_ask_for_skill_buffer.current);

    size_t n = unit->skillset->n_actives;
    const ActiveSkill *actives = unit->skillset->actives;
    const ActiveSkillInfo *info = actives[gs_ask_for_skill_buffer.skill_index].active_info;

    rgb_t color = 0xFFFFFFFF;
    if (!info->can_be_used_cb(actives + 1, unit))
    {
        color = RGB(122, 122, 122);
    }

    const char *name = info->skill_info.name;
    tb_printf(&(commands_window.buffer), color, L"(%u / %u) %s\n",
              gs_ask_for_skill_buffer.skill_index + 1, n, name);
}

void display_combat_state()
{
    tb_printf(&(print_window.buffer), 0xFFFFFFFF, L"- Your units -\n");
    for (slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const Unit *unit = team_get_unit(&(combat_engine.players_team), slot);
        if (unit == NULL)
        {
            continue;
        }

        rgb_t color = 0xFFFFFFFF;
        if (slot == gs_ask_for_skill_buffer.current)
        {
            color = RGB(255, 255, 0);
        }

        tb_printf(&(print_window.buffer), color, L"- %s :: HP: %u, STA: %u\n",
                  unit->name, unit->hp, unit->stamina);
    }

    tb_printf(&(print_window.buffer), 0xFFFFFFFF, L"\n- Enemy units -\n");
    for (slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const Unit *unit = team_get_unit(&(combat_engine.enemy_team), slot);
        if (unit == NULL)
        {
            continue;
        }

        tb_printf(&(print_window.buffer), 0xFFFFFFFF, L"- %s :: HP: %u, STA: %u\n",
                  unit->name, unit->hp, unit->stamina);
    }
}