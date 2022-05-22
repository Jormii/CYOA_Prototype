#include "ui.h"
#include "species.h"
#include "skillset.h"
#include "combat_engine.h"

void print_combat_state();
void print_team(Team *team);
void ask_for_target_single_not_self(UnitSource *target_out);

void ce_on_event(CombatEventSource *source)
{
    CombatEvent event = source->event;
    switch (event)
    {
    case EVENT_START_OF_TURN:
        tb_clear(&(print_window.buffer), NULL);
        tb_print(&(print_window.buffer), RGB(255, 255, 255), L"\nEVENT :: Start of turn\n");
        print_combat_state();
        break;
    case EVENT_END_OF_TURN:
        tb_print(&(print_window.buffer), RGB(255, 255, 255), L"\nEVENT :: End of turn\n");
        break;
    default:
        break;
    }
}

ActiveSkill *ce_ask_for_skill(const Unit *unit)
{
    const SkillSet *skillset = unit->skillset;

    tb_printf(&(print_window.buffer), 0xFFFFFFFF, L"- %s's skills:\n", unit->name);
    for (size_t i = 0; i < skillset->n_actives; ++i)
    {
        const ActiveSkill *skill = skillset->actives + i;
        tb_printf(&(print_window.buffer), 0xFFFFFFFF, L"-- %s :: Cost = %u\n",
                  skill->active_info->skill_info.name,
                  skill->active_info->skill_info.cost);
    }

    return skillset->actives;
}

void ce_ask_for_skills_target(const ActiveSkill *skill, UnitSource *target_out)
{
    ActiveSkillTarget target = skill->active_info->target;
    switch (target)
    {
    case ACTIVE_TARGET_SINGLE_NOT_SELF:
        ask_for_target_single_not_self(target_out);
        break;
    default:
        tb_printf(&(print_window.buffer), 0xFFFFFFFF, L"Can't handle skill whose target is %u\n", target);
        break;
    }
}

void print_combat_state()
{
    tb_print(&(print_window.buffer), 0xFFFFFFFF, L"- Your team -\n");
    print_team(&(combat_engine.players_team));

    tb_print(&(print_window.buffer), 0xFFFFFFFF, L"- Enemy team -\n");
    print_team(&(combat_engine.enemy_team));
}

void print_team(Team *team)
{
    for (slot_t slot = 0; slot < MAX_UNITS_IN_COMBAT; ++slot)
    {
        const Unit *unit = team_get_unit(team, slot);
        if (unit != NULL)
        {
            tb_printf(&(print_window.buffer), 0xFFFFFFFF,
                      L"%s (%lu / %s) :: HP=%u, STA=%u\n",
                      unit->name, unit->id, unit->species->name,
                      unit->hp, unit->stamina);
        }
    }
}

void ask_for_target_single_not_self(UnitSource *target_out)
{
    target_out->slot = 0;
    target_out->team = &(combat_engine.enemy_team);
    target_out->unit_id = team_get_unit(target_out->team, target_out->slot)->id;
}