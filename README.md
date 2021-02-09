# FreeRGB

![issues](https://img.shields.io/github/issues/milan338/ArduRGB?style=flat-square)
![size](https://img.shields.io/github/repo-size/milan338/ArduRGB?style=flat-square)
![license](https://img.shields.io/github/license/milan338/ArduRGB?style=flat-square)

ArduRGB is a fully-featured, lightweight, highly customisable LED controller using the Arduino framework.

## Navigation

- [Wiki](https://github.com/milan338/ArduRGB/wiki)
- [Navigation](#navigation)
- [Features](#features)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Downloading](#downloading)
  - [Updating](#updating)
  - [Uploading](#uploading)
- [Usage](#usage)
- [Creating addons](#creating-addons)
- [Contributing](#contributing)
- [ToDo](#todo)
- [License](#license)

## Features

- **Customisable**: The easy to use addon system enables completely custom lighting effect without adding any code outside the effect definition - integration is handled automatically during build. You get access to all features of the FastLED library.
- **Powerful**: All physical and virtual LED strips are controlled independently of each other, and effects on each strip are updated together; all animations are updated on all strips at the same time.
- **Lightweight**: Using the least amount of resource possible while not compromising on features or performance is a big part of ArduRGB. You can use it on anything from an Arduino Nano to an ESP-32.
- **Scalable**: Whether you have 1 LED strip or 10 LED strips connected to your device, ArduRGB lets you control it. You can even assign virtual LED strips to your physical LED strips. Microcontrollers with more SRAM and faster CPUs will allow you to control more LED strips with more advanced animations.
- **Expandable**: Control your lighting through serial or MQTT or any other type of communicaiton. If it's not already supported, integrating a new communication protocol is incredibly simple.
- **Cross-Platform**: Most microcontrollers that have support for the Arduino framework and FastLED library should work with ArduRGB. If configurations don't already exists for a microcontroller, the modular configuration format makes it easy to add support.

## Installation

### Prerequisites

#### Software

- **PlatformIO Core** >= 5.1.0

#### Hardware

*These are mostly recommendations for usable performance.*
*See the [wiki](https://github.com/milan338/ArduRGB/wiki) for more information.*

- **MCU** = ATmega328 equivalent or better
- **SRAM** >= 2 KB
- **EEPROM / Flash** >= 512 B

### Downloading

To download, all you need to do is clone the repo.
```
git clone https://github.com/milan338/ArduRGB.git
```

### Updating

If you've already cloned the repo and wish to update, simply run
```
git pull origin master
```

### Uploading

Before uploading, remember to run the project's `build` task to generate user config.

After this, setup the configuration to match your build environment.
You can use the additional project tasks for additional configuration options.

If using OTA uploads, remember to run the project's `Flash OTA` through serial task on first upload.

Once ready, run the project's `upload` task.

Detailed information can be found in the [wiki](https://github.com/milan338/ArduRGB/wiki).

## Usage

To use ArduRGB, you'll need some external software to communicate with your device and control it.

[FreeRGB](https://github.com/milan338/FreeRGB) has support for ArduRGB out of the box.

Visit the [wiki](https://github.com/milan338/ArduRGB/wiki) for more information.

## Creating Addons

Addons can come in the form of:
- Custom effects
- Communication interfaces

Please see the wiki for more information about the types of addons that can be created,
and how to create and integrate them.

## Contributing

Before contributing, please ensure you've read the [code of conduct](CODE_OF_CONDUCT.md).

If you'd like to contribute to the project, please see the [guidelines](CONTRIBUTING.md).

## ToDo

- [ ] Support for MQTT communication
- [ ] Web API
- [ ] Update notifications
- [ ] CI pipelines

## License

[GNU General Public License (version 3)](LICENSE)
