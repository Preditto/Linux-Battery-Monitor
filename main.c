#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BATTERY_PATH "/sys/class/power_supply/BAT0"
#define LOW_BATTERY 30
#define HIGH_BATTERY 80
#define CHECK_INTERVAL 30

int read_battery_percentage(void) {
    FILE *f = fopen(BATTERY_PATH "/capacity", "r");
    int percentage;

    if (!f)
        return -1;

    if (fscanf(f, "%d", &percentage) != 1) {
        fclose(f);
        return -1;
    }

    fclose(f);
    return percentage;
}

int read_battery_status(char *status) {
    FILE *f = fopen(BATTERY_PATH "/status", "r");

    if (!f)
        return 0;

    if (fscanf(f, "%31s", status) != 1) {
        fclose(f);
        return 0;
    }

    fclose(f);
    return 1;
}

void send_notification(const char *title, const char *message) {
    char command[512];

    snprintf(
        command,
        sizeof(command),
        "notify-send -u critical \"%s\" \"%s\"",
        title,
        message
    );

    system(command);
}

int main(void) {
    int low_notified = 0;
    int high_notified = 0;

    while (1) {
        int battery = read_battery_percentage();
        char status[32];
        char message[128];

        if (battery < 0 || !read_battery_status(status)) {
            sleep(CHECK_INTERVAL);
            continue;
        }

        /* Low battery warning */
        if (battery <= LOW_BATTERY &&
            strcmp(status, "Charging") != 0 &&
            !low_notified) {

            snprintf(
                message,
                sizeof(message),
                "Battery at %d%%. Plug in charger.",
                battery
            );

            send_notification("Battery Low", message);
            low_notified = 1;
        }

        if (battery > LOW_BATTERY)
            low_notified = 0;

        /* High battery warning */
        if (battery >= HIGH_BATTERY &&
            strcmp(status, "Charging") == 0 &&
            !high_notified) {

            snprintf(
                message,
                sizeof(message),
                "Battery at %d%%. Unplug charger.",
                battery
            );

            send_notification("Battery Charged", message);
            high_notified = 1;
        }

        if (battery < HIGH_BATTERY)
            high_notified = 0;

        sleep(CHECK_INTERVAL);
    }

    return 0;
}
