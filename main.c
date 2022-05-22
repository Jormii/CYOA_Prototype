#include <pspkernel.h>
#include <pspdisplay.h>

#include "ui.h"
#include "log.h"
#include "types.h"
#include "callbacks.h"
#include "vm_manager.h"
#include "all_species.h"
#include "text_buffer.h"
#include "vm_callbacks.h"
#include "combat_engine.h"
#include "screen_buffer.h"
#include "damage_calculator.h"
#include "special_condition.h"

#define LOG_ERROR(format, ...)                                                        \
    {                                                                                 \
        tb_printf(&(print_window.buffer), RGB(255, 122, 122), format, ##__VA_ARGS__); \
    }

PSP_MODULE_INFO("Prototype", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

bool_t found_error;

void initialize();
void game_loop();
void deinitialize();

// TODO: Remove
size_t id = 0;
void init_unit(Unit *unit, char *name, Species *species)
{
    unit->id = id++;
    unit->name = name;
    unit->species = species;
    unit->skillset = skillset_init(species->skillset_template);
    unit->hp = 10;
    unit->stamina = 20;
    for (size_t attr = 0; attr < ATTR_COUNT; ++attr)
    {
        unit->attributes[attr] = 1;
    }
    unit->attributes[ATTR_AGILITY] = unit->id;
    dynamic_list_init(&(unit->special_conditions), 1,
                      DYNAMIC_LIST_INCREMENT_DOUBLE, sizeof(SpecialCondition));
}

int main()
{
    initialize();
    while (running())
    {
        game_loop();
    }
    deinitialize();

    return 0;
}

void initialize()
{
    found_error = FALSE;

    // System and interface
    setup_callbacks();

    sb_initialize();
    ui_initialize();

    // CYOA's virtual machine
    VMInitialization vm_init = {
        .max_options = 2,
        .max_stack_size = 2,
        .binaries_dir = "umd0:/assets/binaries/",
        .global_vars_path = "umd0:/assets/binaries/global.bin",

        .call_cb = call_cb,
        .print_cb = print_cb,
        .print_option_cb = print_option_cb,
        .read_cb = read_file_cb,
        .save_program_cb = save_program_cb,
        .save_global_vars_cb = save_global_vars_cb};
    if (!vm_manager_initialize(&vm_init))
    {
        found_error = TRUE;
        LOG_ERROR(L"Couldn't initialize VirtualMachine manager\n");
    }

    if (sizeof(size_t) != sizeof(uint32_t))
    {
        // TODO: This is cyoa's engine's business. Move to its right place later
        found_error = TRUE;
        LOG_ERROR(L"Pointer size should be 4 bytes. sizeof(size_t) <- %lu", sizeof(size_t));
    }

    // Combat engine
    ce_init();
    damage_calculator_init();

    bird_init();

    init_unit(combat_engine.players_team.units, "Ally 1", &bird_species);
    init_unit(combat_engine.players_team.units + 1, "Ally 2", &bird_species);
    init_unit(combat_engine.enemy_team.units, "Enemy 1", &bird_species);
    init_unit(combat_engine.enemy_team.units + 1, "Enemy 2", &bird_species);

    // End
    if (!found_error)
    {
#if 0
        vm_manager_load_program("Escena.bin");
        vm_execute();
        vm_display_options();
#else
        ce_start_combat();
#endif
    }
}

void game_loop()
{
    ui_update();
    sb_swap_buffers();
    sceDisplayWaitVblankStart();
}

void deinitialize()
{
    LOG_CLOSE();
    sceKernelExitGame();
}