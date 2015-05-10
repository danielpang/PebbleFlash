#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

// Turns the flash on once the watchapp is opened.
static bool flash = true;

// Turns on the flash is the flash is currently off, or turns off the flash
// if the flash is currently on by pressing the select button of the pebble
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (flash == false){
    light_enable(true);
    flash = true;
    text_layer_set_text(text_layer, "Flash: ON");
  } else {
    light_enable(false);
    flash = false;
    text_layer_set_text(text_layer, "Flash: OFF");
  }
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text(text_layer, "Pebble Flash");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  light_enable(true);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

// Initializes the app by creating the main window
static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

// Destroys the app by deallocating the memory 
// and by turning off the flash
static void deinit(void) {
  light_enable(false);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
