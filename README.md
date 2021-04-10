# HSB-Electrocardiogram
A project for the Bremen City University of Applied Sciences C++ course. Core functionality includes parsing of a unsigned 16bit DAC electrocardiogram signal file,
analyzing the signal and either showing basic information in a commandline or in a Qt5 GUI. Additionally the signal can be analyzed for potential heartbeats using
simple signal processing algorithms.

## Language & Toolchain
- C++17
- Visual Studio 2019 16.8
- Qt5.15.2

## Command Line Arguments
| Parameter                | Modes   | Optional | Argument     | Default | Description                                         |
|--------------------------|---------|----------|--------------|---------|-----------------------------------------------------|
| --no-gui                 | -       | Yes      | -            | -       | Hides GUI                                           |
| --input-file             | Console | No       | String       | -       | Path to raw signal file                             |
| --output-file            | Console | Yes      | String       | -       | Pfad to output ECG file                             |
| --smoothing-window-size  | Both    | Yes      | int          | 10      | Window size in both directions of the current value |
| --sampling-interval      | Both    | Yes      | unsigned int | 5       | Interval value of the signal in milliseconds        |
| --signalrange-millivolts | Both    | Yes      | int          | 4       | Signalrange(min-max) in millivolts                  |
| --signalrange-max        | Both    | Yes      | int          | 65535   | Max value of the raw signal                         |
| --heartbeat-tail-length  | Both    | Yes      | int          | 25      | Windowsize of heartbeats to search for              |
