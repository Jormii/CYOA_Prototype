#include "base_set_font.h"
#include "combat_engine.h"
#include "screen_buffer.h"
#include "screen_common.h"
#include "window_display.h"
#include "creature_summary.h"

#define CREATURE_SUMMARY_BUFFER_LENGTH 128

void creature_summary_initialize()
{
    Window *w = &(creature_summary.display_window);
    w->font = &base_set_font;
    w->margin.left = SCREEN_COMMON_MARGIN;
    w->margin.right = SCREEN_COMMON_VER_DIV_X - SCREEN_EXTRA_MARGIN;
    w->margin.top = SCREEN_COMMON_MARGIN;
    w->margin.bottom = SCREEN_HEIGHT - SCREEN_COMMON_MARGIN;
    create_text_buffer(CREATURE_SUMMARY_BUFFER_LENGTH, &(w->buffer));
}

void creature_summary_update()
{
    Team *team = &(combat_engine.players_team.team);
    const Unit *unit = team_get_unit(team, creature_summary.creature_index);
    if (unit == NULL)
    {
        return;
    }

    TextBuffer *buffer = &(creature_summary.display_window.buffer);
    tb_clear(buffer, NULL);

    tb_printf(buffer, 0x00FFFFFF, L"%ls\n\n", unit->species->name);
    tb_printf(buffer, 0x00FFFFFF, L"SALUD:       %u / %u\n", unit->hp, unit_calculate_stat(unit, STAT_HEALTH));
    tb_printf(buffer, 0x00FFFFFF, L"RESISTENCIA: %u / %u\n\n", unit->stamina, unit_calculate_stat(unit, STAT_STAMINA));

    tb_printf(buffer, 0x00FFFFFF, L"DAÑO:      %u\n", unit_calculate_stat(unit, STAT_DAMAGE));
    tb_printf(buffer, 0x00FFFFFF, L"DEFENSA:   %u\n", unit_calculate_stat(unit, STAT_ENDURANCE));
    tb_printf(buffer, 0x00FFFFFF, L"VELOCIDAD: %u\n", unit_calculate_stat(unit, STAT_SPEED));

    display_window(&(creature_summary.display_window));
}