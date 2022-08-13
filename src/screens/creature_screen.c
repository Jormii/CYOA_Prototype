#include "input.h"
#include "base_set_font.h"
#include "combat_engine.h"
#include "screen_buffer.h"
#include "screen_common.h"
#include "window_display.h"
#include "creature_summary.h"
#include "special_characters.h"

// TODO: HANDLING MEMORY. I RUN OUT OF IT!!!

#define CONFIRMATION_PRESSES 3
#define ATTRIBUTE_WINDOW_WIDTH 85
#define ATTRIBUTE_WINDOW_BUFFER_LENGTH 512
#define SKILL_WINDOW_BUFFER_LENGTH 512

const wchar_t *attribute_names[ATTR_COUNT] = {
    L"Fuerza",
    L"Destreza",
    L"Fortaleza",
    L"Agilidad"};

const wchar_t *priority_names[SKILL_PRIORITY_COUNT] = {
    L"Mínima",
    L"Lenta",
    L"Normal",
    L"Alta",
    L"Máxima",
    L"Objeto"};

const wchar_t *skill_type_empty_str = L"";
const wchar_t *skill_type_active_str = L"Activa";
const wchar_t *skill_type_passive_str = L"Pasiva";
const wchar_t *skill_type_condition_str = L"Condición";
const wchar_t **skill_type_names[SKILL_TYPE_COUNT] = {
    &skill_type_passive_str,
    &skill_type_condition_str,
    &skill_type_empty_str,

    &skill_type_active_str,
    &skill_type_active_str,
    &skill_type_active_str,
    &skill_type_active_str};

// TODO: Reset when entering this state
struct
{
    size_t x_presses;
    size_t skill_cursor;
    size_t skill_cursor_local;
    Attribute attribute_cursor;
    attribute_t attribute_change[ATTR_COUNT];
} creature_screen_interface;

Window attribute_windows[ATTR_COUNT];

Window skill_window = {
    .margin = {
        .left = SCREEN_COMMON_VER_DIV_X + SCREEN_EXTRA_MARGIN,
        .right = SCREEN_WIDTH - SCREEN_COMMON_MARGIN,
        .top = SCREEN_COMMON_HOR_DIV_Y + SCREEN_EXTRA_MARGIN,
        .bottom = SCREEN_HEIGHT - SCREEN_COMMON_MARGIN},
    .font = &base_set_font};

Window confirm_window = {
    .margin = {
        .left = SCREEN_COMMON_VER_DIV_X + SCREEN_EXTRA_MARGIN,
        .right = SCREEN_WIDTH - SCREEN_COMMON_MARGIN,
        .top = SCREEN_COMMON_HOR_DIV_Y - SCREEN_EXTRA_MARGIN,
        .bottom = SCREEN_HEIGHT - SCREEN_COMMON_MARGIN},
    .font = &base_set_font};

void creature_screen_handle_input();
void creature_screen_increase_attribute();
void creature_screen_decrease_attribute();
void creature_screen_interface_commit_attributes();

void creature_screen_adjust_skill_cursor();
void creature_screen_print_skills(const Unit *unit);

void creature_screen_initialize()
{
    // Interface
    creature_screen_interface.x_presses = 0;
    creature_screen_interface.skill_cursor = 0;
    creature_screen_interface.skill_cursor_local = 0;
    creature_screen_interface.attribute_cursor = ATTR_STRENGTH;
    for (Attribute attr = 0; attr < ATTR_COUNT; ++attr)
    {
        creature_screen_interface.attribute_change[attr] = 0;
    }
    creature_screen_adjust_skill_cursor();

    // Skill window
    create_text_buffer(SKILL_WINDOW_BUFFER_LENGTH, &(skill_window.buffer));

    // Confirm window
    create_text_buffer(18, &(confirm_window.buffer)); // String: "* Confirmar x x x\0"
    confirm_window.margin.top -= base_set_font.height;

    // Attribute windows
    screen_t left = SCREEN_COMMON_VER_DIV_X + SCREEN_EXTRA_MARGIN;
    for (Attribute attr = 0; attr < ATTR_COUNT; ++attr)
    {
        Window *w = attribute_windows + attr;

        w->font = &base_set_font;
        create_text_buffer(ATTRIBUTE_WINDOW_BUFFER_LENGTH, &(w->buffer));

        w->margin.left = left;
        w->margin.right = left + ATTRIBUTE_WINDOW_WIDTH;
        w->margin.top = SCREEN_COMMON_MARGIN;
        w->margin.bottom = confirm_window.margin.top - SCREEN_COMMON_MARGIN;

        left = w->margin.right + SCREEN_COMMON_MARGIN;
    }
}

void creature_screen_update()
{
    Team *team = &(combat_engine.players_team.team);
    const Unit *unit = team_get_unit(team, creature_summary.creature_index);
    if (unit != NULL)
    {
        creature_screen_print_skills(unit);

        for (Attribute attr = 0; attr < ATTR_COUNT; ++attr)
        {
            display_window(attribute_windows + attr);
        }
        display_window(&skill_window);
        display_window(&confirm_window);
        creature_summary_update();
        screen_common_update();
    }

    creature_screen_handle_input();
}

void creature_screen_handle_input()
{
    bool_t other_pressed = FALSE; // TODO: Ugly
    bool_t square_bttn = input_button_held(BUTTON_SQUARE);
    if (input_button_pressed(BUTTON_CROSS))
    {
        creature_screen_interface.x_presses += 1;
        if (creature_screen_interface.x_presses == CONFIRMATION_PRESSES)
        {
            creature_screen_interface.x_presses = 0;
            creature_screen_interface_commit_attributes();
        }
    }
    else if (input_button_pressed(BUTTON_UP))
    {
        other_pressed = TRUE;
        if (square_bttn)
        {
            creature_screen_increase_attribute();
        }
        else if (creature_screen_interface.skill_cursor_local != 0)
        {
            creature_screen_interface.skill_cursor_local -= 1;
            creature_screen_adjust_skill_cursor();
        }
    }
    else if (input_button_pressed(BUTTON_DOWN))
    {
        other_pressed = TRUE;
        if (square_bttn)
        {
            creature_screen_decrease_attribute();
        }
        else
        {
            creature_screen_interface.skill_cursor_local += 1;
            creature_screen_adjust_skill_cursor();
        }
    }
    else if (input_button_pressed(BUTTON_LEFT))
    {
        other_pressed = TRUE;
        if (creature_screen_interface.attribute_cursor != ATTR_STRENGTH)
        {
            creature_screen_interface.attribute_cursor -= 1;
            creature_screen_adjust_skill_cursor();
        }
    }
    else if (input_button_pressed(BUTTON_RIGHT))
    {
        other_pressed = TRUE;
        if (creature_screen_interface.attribute_cursor != ATTR_AGILITY)
        {
            creature_screen_interface.attribute_cursor += 1;
            creature_screen_adjust_skill_cursor();
        }
    }

    if (other_pressed)
    {
        creature_screen_interface.x_presses = 0;
    }
}

void creature_screen_increase_attribute()
{
    Team *team = &(combat_engine.players_team.team);
    const Unit *unit = team_get_unit(team, creature_summary.creature_index);
    if (unit == NULL)
    {
        return;
    }

    if (creature_summary.attr_points_available != 0)
    {
        attribute_t *attr = creature_screen_interface.attribute_change + creature_screen_interface.attribute_cursor;
        *attr += 1;
        creature_summary.attr_points_available -= 1;
    }
}

void creature_screen_decrease_attribute()
{
    attribute_t *attr = creature_screen_interface.attribute_change + creature_screen_interface.attribute_cursor;
    if ((*attr) != 0)
    {
        *attr -= 1;
        creature_summary.attr_points_available += 1;
    }
}

void creature_screen_interface_commit_attributes()
{
    // TODO: Lots of repetition
    Team *team = &(combat_engine.players_team.team);
    Unit *unit = team_get_unit(team, creature_summary.creature_index);
    if (unit == NULL)
    {
        return;
    }

    for (Attribute attr = 0; attr < ATTR_COUNT; ++attr)
    {
        unit->attributes[attr] += creature_screen_interface.attribute_change[attr];
        creature_screen_interface.attribute_change[attr] = 0;
    }
}

void creature_screen_adjust_skill_cursor()
{
    Team *team = &(combat_engine.players_team.team);
    const Unit *unit = team_get_unit(team, creature_summary.creature_index);
    if (unit == NULL)
    {
        return;
    }

    size_t *global = &(creature_screen_interface.skill_cursor);
    size_t *local = &(creature_screen_interface.skill_cursor_local);
    const SkillSetMetadata *skillset_meta = unit->species->skillset_template;

    bool_t found = FALSE;
    size_t local_target = *local;
    while (!found)
    {
        *local = 0;
        *global = 0;

        while (!found && (*global) < skillset_meta->n_skills)
        {
            const SkillMetadata *skill_meta = skillset_meta->skills_metadata[*global];
            if (skill_meta->attribute == creature_screen_interface.attribute_cursor)
            {
                if ((*local) == local_target)
                {
                    found = TRUE;
                }
                else
                {
                    *local += 1;
                }
            }

            *global += !found;
        }

        if (!found)
        {
            if (local_target == 0)
            {
                // No skills for the given attribute. Force break to avoid overflow
                break;
            }
            else
            {
                local_target -= 1;
            }
        }
    }
}

void creature_screen_print_skills(const Unit *unit)
{
    // Print column headers
    for (Attribute attr = 0; attr < ATTR_COUNT; ++attr)
    {
        TextBuffer *buffer = &(attribute_windows[attr].buffer);
        rgb_t color = (attr == creature_screen_interface.attribute_cursor) ? 0x0000FFFF : 0x00FFFFFF;

        tb_clear(buffer, NULL);
        tb_printf(buffer, color, L"%ls : %u",
                  attribute_names[attr], unit->attributes[attr]);

        attribute_t modification = creature_screen_interface.attribute_change[attr];
        if (modification == 0)
        {
            tb_print(buffer, color, L" (0)\n");
        }
        else
        {
            tb_printf(buffer, color, L" (+%u)\n", modification);
        }
    }

    // Print skill in their respective columns
    const SkillSetMetadata *skillset_meta = unit->species->skillset_template;
    for (size_t i = 0; i < skillset_meta->n_skills; ++i)
    {
        const SkillMetadata *skill_meta = skillset_meta->skills_metadata[i];
        TextBuffer *buffer = &(attribute_windows[skill_meta->attribute].buffer);

        bool_t over_cursor = i == creature_screen_interface.skill_cursor;
        if (unit_knows_skill(unit, skill_meta))
        {
            rgb_t color = (over_cursor) ? 0x0000FFFF : 0x00FFFFFF;
            tb_printf(buffer, color, L" - %ls\n", skill_meta->name);
        }
        else
        {
            rgb_t color = (over_cursor) ? 0x00008888 : 0x00888888;
            tb_print(buffer, color, L" - ?????\n");
        }
    }

    // Print skill under cursor
    tb_clear(&(skill_window.buffer), NULL);
    if (creature_screen_interface.skill_cursor != skillset_meta->n_skills)
    {
        TextBuffer *buffer = &(skill_window.buffer);
        const SkillMetadata *skill_meta = skillset_meta->skills_metadata[creature_screen_interface.skill_cursor];

        bool_t knows_skill = unit_knows_skill(unit, skill_meta);
        if (knows_skill)
        {
            tb_print(buffer, 0x0000FFFF, skill_meta->name);
        }
        else
        {
            tb_print(buffer, 0x0000FFFF, L" - ?????\n");
        }

        // TODO: Print requirements

        if (knows_skill)
        {
            tb_printf(buffer, 0x00FFFFFF, L"\n- Coste: %u | Tipo: %ls | Prioridad: %ls\n\n",
                      skill_meta->cost, *skill_type_names[skill_meta->type], priority_names[skill_meta->priority]);
            tb_print(buffer, 0x00FFFFFF, skill_meta->description);
        }
    }

    // Print confirmation
    tb_clear(&(confirm_window.buffer), NULL);
    tb_print(&(confirm_window.buffer), 0x00FFFFFF, L"* Confirmar");
    size_t count = creature_screen_interface.x_presses;
    for (size_t i = 0; i < CONFIRMATION_PRESSES; ++i)
    {
        rgb_t color = (i >= count) ? 0x00FFFFFF : 0x0000FFFF;
        tb_printf(&(confirm_window.buffer), color, L" %lc", SPECIAL_CHARACTER_CROSS);
    }
}
