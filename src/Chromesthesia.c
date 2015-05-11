#include <pebble.h>
#include "dial.h"
#include "hands.h"
#include "utils.h"

static Window *window;
static Layer *dial_layer, *hands_layer, *date_layer;
static StatusBarLayer *status_layer;
static TextLayer *day_label, *num_label;

static char day_buffer[6] = "", num_buffer[4] = "";

static void date_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(day_buffer, sizeof(day_buffer), "%a", t);
  text_layer_set_text(day_label, day_buffer);

  strftime(num_buffer, sizeof(num_buffer), "%d", t);
  text_layer_set_text(num_label, num_buffer);
}


static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(hands_layer);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  dial_layer = layer_create(bounds);
  layer_set_update_proc(dial_layer, dial_update_layer);
  layer_add_child(window_layer, dial_layer);

  hands_layer = layer_create(bounds);
  layer_set_update_proc(hands_layer, hands_update_layer);
  layer_add_child(dial_layer, hands_layer);

  date_layer = layer_create(GRect(0,153,144,15));
  layer_set_update_proc(date_layer, date_update_proc);
  layer_add_child(window_layer, date_layer);

  day_label = text_layer_create(GRect(46, 0, 27, 15));
  text_layer_set_text(day_label, day_buffer);
  text_layer_set_background_color(day_label, GColorBlack);
#ifdef PBL_COLOR
  text_layer_set_text_color(day_label, GColorDarkGray);
#else
  text_layer_set_text_color(day_label, GColorWhite);
#endif
  text_layer_set_font(day_label, fonts_get_system_font(FONT_KEY_GOTHIC_14));

  layer_add_child(date_layer, text_layer_get_layer(day_label));

  num_label = text_layer_create(GRect(73, 0, 18, 15));
  text_layer_set_text(num_label, num_buffer);
  text_layer_set_background_color(num_label, GColorBlack);
#ifdef PBL_COLOR
  text_layer_set_text_color(num_label, GColorDarkGray);
#else
  text_layer_set_text_color(num_label, GColorWhite);
#endif
  text_layer_set_font(num_label, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));

  layer_add_child(date_layer, text_layer_get_layer(num_label));

  // status bar
  status_layer = status_bar_layer_create();
  layer_add_child(window_layer, status_bar_layer_get_layer(status_layer));
 
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

static void window_unload(Window *window) {
	status_bar_layer_destroy(status_layer);
	text_layer_destroy(day_label);
	text_layer_destroy(num_label);
	layer_destroy(date_layer);
	layer_destroy(hands_layer);
	layer_destroy(dial_layer);
}

static void init(void) {
  window = window_create();
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  dial_init();
  hands_init(bounds);

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_set_background_color(window, GColorBlack);

  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
