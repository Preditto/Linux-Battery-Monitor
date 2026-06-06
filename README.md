# Battery Notifier

A lightweight battery monitoring utility written in C for Linux systems. The program continuously checks battery charge levels and sends desktop notifications when the battery becomes too low or reaches a user-defined upper charging limit.

The goal is simple: help users maintain healthier charging habits without manually checking battery levels throughout the day.

## Features

- Monitors battery percentage directly through the Linux SysFS power interface.
- Sends a critical notification when the battery falls to 30% or below while discharging.
- Sends a notification when the battery reaches 80% or above while charging.
- Prevents notification spam using internal state tracking.
- Runs with minimal CPU and memory usage.
- No external libraries required beyond standard Linux notification support.
- Simple, portable, and easy to modify.

## How It Works

The program periodically reads battery information from:

```text
/sys/class/power_supply/BAT0/
```

Every 30 seconds it:

1. Reads the current battery percentage from `capacity`.
2. Reads the battery charging state from `status`.
3. Checks whether the configured thresholds have been reached.
4. Displays a desktop notification if action is required.
5. Remembers previously sent alerts to avoid repeated notifications.

This approach uses a lightweight polling mechanism and relies on information provided directly by the Linux kernel.

## Prerequisites

- Linux operating system
- GCC or Clang compiler
- `notify-send` (provided by libnotify)

On Debian/Ubuntu-based systems:

```bash
sudo apt install libnotify-bin
```

## Installation

Clone the repository:

```bash
git clone <repository-url>
cd battery-notifier
```

Compile the source code:

```bash
gcc -o battery_notifier main.c
```

(Optional) Install system-wide:

```bash
sudo mv battery_notifier /usr/local/bin/
```

## Usage

Run directly from the terminal:

```bash
./battery_notifier
```

Run in the background:

```bash
./battery_notifier &
```

For long-term use, consider adding the executable to your desktop environment's startup applications or creating a user-level systemd service.

## Example Notifications

Low battery alert:

```text
Battery Low
Battery at 28%. Plug in charger.
```

Charging limit reached:

```text
Battery Charged
Battery at 80%. Unplug charger.
```

## Configuration

The program currently uses compile-time configuration through macros defined near the top of `main.c`.

```c
#define BATTERY_PATH "/sys/class/power_supply/BAT0"
#define LOW_BATTERY 30
#define HIGH_BATTERY 80
#define CHECK_INTERVAL 30
```

### Description

| Macro | Purpose |
|---------|---------|
| BATTERY_PATH | Battery information directory |
| LOW_BATTERY | Low battery notification threshold |
| HIGH_BATTERY | Upper charging notification threshold |
| CHECK_INTERVAL | Time between battery checks (seconds) |

If your system uses a different battery identifier (for example `BAT1`), update `BATTERY_PATH` before compiling.

## Technical Overview

The application follows a polling-based monitoring model.

Battery information is obtained through the Linux SysFS interface:

```text
/sys/class/power_supply/BAT0/capacity
/sys/class/power_supply/BAT0/status
```

To prevent repeated notifications, the program maintains two internal state variables:

```c
low_notified
high_notified
```

These flags act as a simple finite-state mechanism, ensuring that alerts are only shown once until battery conditions change.

## Project Structure

```text
.
├── main.c
└── README.md
```

## Future Improvements

- Support for configuration files
- Command-line argument support
- Custom notification thresholds
- Multiple battery support
- Automatic systemd service installation
- Sound-based alerts

## License

This project is released under the MIT License.
