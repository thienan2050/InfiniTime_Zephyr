#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
LOG_MODULE_REGISTER(app);

static uint32_t count;

static void lv_btn_click_callback(lv_event_t *e)
{
    ARG_UNUSED(e);

    count = 0;
}

static int set_backlight_on(void)
{
#define BACKLIGHT_HIGH_NODE DT_ALIAS(backlighthigh)

    const struct gpio_dt_spec bl_high = GPIO_DT_SPEC_GET(BACKLIGHT_HIGH_NODE, gpios);
    gpio_pin_configure_dt(&bl_high, GPIO_OUTPUT_ACTIVE); // Tắt P0.23
    gpio_pin_set_dt(&bl_high, 1);
    return 1;
}

int main(void)
{
    LOG_INF("Hello, PineTime!\n");
    char count_str[11] = {0};
    const struct device *display_dev;
    lv_obj_t *hello_world_label;
    lv_obj_t *count_label;

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(display_dev))
    {
        LOG_ERR("Device not ready, aborting");
        return 0;
    }

    if (set_backlight_on() < 0)
    {
        LOG_ERR("Backlight failure: aborting");
        return 0;
    }

    
    if (IS_ENABLED(CONFIG_LV_Z_POINTER_KSCAN) || IS_ENABLED(CONFIG_LV_Z_POINTER_INPUT))
    {
        lv_obj_t *hello_world_button;

        hello_world_button = lv_btn_create(lv_scr_act());
        lv_obj_align(hello_world_button, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(hello_world_button, lv_btn_click_callback, LV_EVENT_CLICKED,
                            NULL);
        hello_world_label = lv_label_create(hello_world_button);
    }
    else
    {
        hello_world_label = lv_label_create(lv_scr_act());
    }

    lv_label_set_text(hello_world_label, "Di Mat (Di Max)");
    lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

    count_label = lv_label_create(lv_scr_act());
    lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_task_handler();
    display_blanking_off(display_dev);

    while (1)
    {
        if ((count % 100) == 0U)
        {
            sprintf(count_str, "%d", count / 100U);
            lv_label_set_text(count_label, count_str);
        }
        lv_task_handler();
        ++count;
        k_sleep(K_MSEC(10));
    }
    return 0;
}