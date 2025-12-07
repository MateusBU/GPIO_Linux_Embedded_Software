# GPIO Linux Embedded Example (libgpiod)

This project demonstrates how to control GPIOs in an Embedded Linux environment using **libgpiod v2**.  
It reads a button input and mirrors its state to an LED output.

---

## 📁 Source Code Structure

| File | Description |
|------|-------------|
| **bsp.c** | Board Support Package: initializes GPIO chip, configures input/output lines, sets and gets pin values. |
| **app.c** | Application logic: reads the input button and updates the output LED. |
| **main.c** | Program entry point: initializes GPIO subsystem and runs the main loop. |

---

## 🔌 Hardware Behavior

- **Input**: A button connected to the system GPIO (with pull-up enabled).
- **Output**: A yellow LED that turns ON/OFF based on button state.

The logic flow is:
1. Read button value (`bsp_GetInputValue()` in `app.c`)
2. Set LED value (`bsp_SetOutputValue()` in `app.c`)
3. Apply outputs to hardware (`bsp_OutputsHandler()` in `main.c`)

---

## 🧰 Dependencies

This project uses:
- **libgpiod** (version 2.x)

Make sure libgpiod is installed on your target Linux system:
```bash
sudo apt install libgpiod-dev
