# Embedded Design Challenge

## Problem

You are tasked with designing a portion of an overall system that is responsible for taking a
series of images using an automated microscope. The portion you're responsible for includes
the firmware and hardware drivers (be them kernel or userspace) which control the overall
image acquisition sequence.

The microscope is required to automatically find the best focus by capturing a series of images
while moving the focus actuator while varying several camera settings. The BLDC motor is used
to cycle through different focus positions. These images are then evaluated by an algorithm to
determine the best focus and therefore the motor setting that achieves that and can reproduce
the highest quality image.

The whole process is time sensitive: illuminators/light sources cannot be left on longer than
necessary, image throughput (how many captures per unit time) is important but not at video
speed. The total time taken for a sequence of move-focus-image should be as short as can be
managed and minimized.

The images are to be pre-processed and then delivered to another process running on Linux on
the Cortex-A processor alongside your driver. This handoff should be carefully managed to allow
for minimal resource burden, copies, and thus high throughput.

## Electronics

Electronics are organized as follows:

- **ARM Cortex‑A**
  - Connected to **MIPI‑CSI mux** via:
    - MIPI‑CSI (image data)
    - I²C (control)
  - **MIPI‑CSI mux** connects to:
    - Image sensor 1
    - Image sensor 2
- **ARM Cortex‑M**
  - Communicates with ARM Cortex‑A via:
    - High‑speed serial
    - Shared active‑low reset signal (RESET#)
  - Connected to:
    - Focus voice coil actuator
      - Control
      - Feedback
    - Illuminators
      - On/off
      - Current feedback
    - BLDC motor drive
      - Control
      - Feedback

*See `electronics_diagram.png` for block diagram.*

## Questions

1. How would you organize the software to satisfy the requirements?

2. What considerations beyond these requirements would you consider important?

3. What interface(s) would you consider part of the design? Please include high-level
descriptions of each interface along with the core definitions?

4. What are some risks of your chosen design that should be taken into consideration and
mitigated?

*Feel free to make any other assumptions that you think are necessary - just state what they are.*

*If you have any questions, feel free to ask.*