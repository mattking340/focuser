# README.md

## 1. Problem

This project is part of a system that automates the process of capturing focused images. It is responsible for coordinating image acquisition and control of supporting hardware to produce the best possible image for further processing.

### Design Challenge Questions

1. How would you organize the software to satisfy the requirements?  
2. What considerations beyond these requirements would you consider important?  
3. What interface(s) would you consider part of the design? Please include high-level descriptions of each interface along with the core definitions.  
4. What are some risks of your chosen design that should be taken into consideration and mitigated?

## 2. Givens

- The system must capture a sharp image by coordinating focus, motion, and illumination.  
- Autofocus is sweep-based, using a motorized actuator.  
- Software runs on a Linux-based Cortex-A processor.  
- Communicates with a Cortex-M controller.  
- Image acquisition and hardware control must be synchronized.  
- The final image is passed to another Linux process.  
- Performance matters: minimize latency and illumination time.  
- Only the Linux-side software is in scope.

## 3. Assumptions

- Protobuf is used to serialize command/response messages over UART.  
- Final image is passed via shared memory (`shm_open`) with optional signaling (e.g. named pipe).  
- Configs are hardcoded now, but structured for git-based provisioning (e.g. TOML + commit SHA).  
- Local calibration may override provisioned settings.  
- Camera is accessed using V4L2 via `/dev/videoX`.  
- Camera MUX is controlled via I2C.  
- Actuator and illumination are controlled via UART.  
- Focus scoring is mocked; ready for CV-based metrics.  
- Error handling is minimal but supported via structured response types.  
- Logging uses `std::cout` as a placeholder for structured telemetry.  
- Device tree and media graph are assumed preconfigured.  
- RESET# GPIO is available to reset the Cortex-M.

## 4. Solution

### Q1  
How would you organize the software to satisfy the requirements?

See `docs/architecture.png` block diagram for high-level structure.

- HALs
  - `CameraHal` / `ActuatorHal` -> abstract base interfaces for pluggable backends
  - `V4l2CameraHal` -> camera config and capture  
  - `UartActuatorHal` -> actuator and illumination control  
  - `Mux` -> camera selection over I2C
- Transports  
  - `Uart` / `I2c` handle framing and bus access
- Protocol  
  - Protobuf-based command/response with typed telemetry
- Orchestration  
  - `ImageCapturer` -> coordinates illumination, mux, and camera  
  - `Focuser` -> runs autofocus (INIT -> SWEEP <-> SCORE -> COMPLETE)
- Test harness  
  - `main()` -> runs an autofocus cycle with mocked scoring
- Support  
  - `config.hpp` -> shared paths, device addresses, and enums (e.g. `CameraType`)  
  - `focus_metric` -> pluggable scoring functions (mocked for now)

### Q2  
What considerations beyond these requirements would you consider important?

- HALs handle hardware, orchestration stays clean  
- Motion, illumination, and capture are timed explicitly  
- Shared memory avoids extra copies during handoff  
- Protobuf supports structured communication and future growth  
- Configs are structured for remote provisioning  
- HALs can be mocked for local or CI testing  
- Cortex-A and Cortex-M boundaries are cleanly separated

### Q3  
What interface(s) would you consider part of the design?

Full definitions and usage details are available in the codebase.

- `V4l2CameraHal` -> Controls camera format, exposure/gain, and frame capture
- `UartActuatorHal` -> Sends Protobuf commands to the Cortex-M, receives typed telemetry
- `Mux` -> Selects camera input via I2C channel
- `ImageCapturer` -> Coordinates mux, illumination, and camera to capture a frame
- `Focuser` -> Runs the autofocus loop and refocuses to the best result
- `Uart` / `I2c` -> Provide raw communication with hardware

### Q4  
What are some risks of your chosen design that should be taken into consideration and mitigated?

- Actuator may become unresponsive -> Reset using GPIO (RESET#)
- Illumination and capture may drift -> Controlled together in `ImageCapturer`
- Focus scoring may be unreliable -> Abstracted and mockable
- Timing may drift or be inconsistent -> All delays are explicit and tunable
- Configs are hardcoded -> Structured for future provisioning
- UART communication may fail -> Framing and parsing are isolated
- Integration boundaries may become complex -> Interfaces are minimal and clearly scoped
