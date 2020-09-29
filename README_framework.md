# STM32 Framework

VSCODE STM32 framework for CubeMX & HAL.

Features automatic build and flash after code change.

## Dependencies

(\* are required for automatic build & flash feature)

NODE\*
NPM\*
GNU Arm Embedded Toolchain / mingw
Make for Windows
STM32CubeMX
STM32 ST-LINK Utility

For debugging:
OpenOCD (included precompiled binaries work fine, source:http://www.freddiechopin.info/en/download/category/4-openocd)
Cortex-Debug (vscode addon)

## Installation

1. Install:

   NODE\*,
   NPM\*,
   mingw gcc,
   Make for windows,
   STM32 ST-LINK Utility,

   and add all of bin folders to your PATH environment.

   System restart may be needed for these to apply.

2. Install STM32CubeMX and edit /stm.ioc project file (Makefile must be selected as Toolchain/IDE).

3. Run "npm install" in project root folder\*.

4. It may be needed to define mcu (ex. "STM32F103xB") in .vscode/c_cpp_properties.json for VSCode IntelliSense to work properly
   (It is added in makefile what messes up IntelliSenses path)

5. For debugging: adjust paths in .vscode/launch.json

## Usage

1. a) run "npm run watch" command in project root folder for automatic build and flash\*.

1. b) run "make & flash"

1. edit /stm.ioc to setup CubeMX and regenerate project config files -> change flash.bat hex file path accordingly to project name

1. use npm run serial for com port terminal
