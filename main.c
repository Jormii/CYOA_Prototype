#include <pspkernel.h>

#include "ui.h"
#include "log.h"
#include "input.h"
#include "callbacks.h"
#include "game_loop.h"
#include "all_skills.h"
#include "cyoa_interface.h"
#include "combat_interface.h"

PSP_MODULE_INFO("Prototype", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

// Extern declarations dedicated to initialize each component
extern State game_state_story;
extern Window story_messages_window;
extern Window story_choices_window;

extern Window combat_state_window;
extern Window combat_log_window;
extern Window combat_commands_window;

extern State game_state_combat; // TODO: Remove these

void initialize();
void deinitialize();

int main()
{
    initialize();
    game_loop_spawn(&game_state_combat);
    deinitialize();

    return 0;
}

void initialize()
{
    log_fd = -1; // TODO: Somewhere else

    // System and interface
    setup_callbacks();

    input_init();
    ui_initialize();

    // Initialize CYOA engine
    uint16_t max_options = 10; // TODO: Read from somewhere
    uint8_t max_stack_size = 10;
    cyoa_interface_initialize(0, max_options, max_stack_size,
                              &(story_messages_window.buffer), &(story_choices_window.buffer));

    // Initialize combat engine
    combat_interface_initialize(&(combat_state_window.buffer), &(combat_log_window.buffer),
                                &(combat_commands_window.buffer));

    // Other
    skills_buffer = &(combat_log_window.buffer);
}

void deinitialize()
{
    LOG_CLOSE();
    sceKernelExitGame();
}