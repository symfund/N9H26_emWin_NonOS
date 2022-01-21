# Developing emWin for N9H26 through Visual Studio 2022 Community and GNU Arm Embedded Toolchain on Windows 10

## emWin Visual C++ 2022 Project Solution
https://github.com/symfund/N9H26_emWin_NonOS/tree/master/BSP/SampleCode/emWin/GUIDemo/VC2022


## Build Environment
* Microsoft Visual Studio 2022 Community (Visual C++)
* GNU Arm Embedded Toolchain

  https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
  
  Alter **BuildCmd.bat**
  ```
  set GNU_TOOLS_ARM_EMBEDDED_PATH="C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.10\bin"
  ```
* msys2
  
  https://www.msys2.org/
  
  Install the 'make' tool
  ```
  pacman -Sy base-devel
  ```
  Alter **BuildCmd.bat**
  ```
  set make_tool_provided_by=C:\msys64\usr\bin
  ```



# Disclaimer
The Software is furnished "AS IS", without warranty as to performance or results, and
the entire risk as to performance or results is assumed by YOU. Nuvoton disclaims all
warranties, express, implied or otherwise, with regard to the Software, its use, or
operation, including without limitation any and all warranties of merchantability, fitness
for a particular purpose, and non-infringement of intellectual property rights.

# Important Notice

Nuvoton Products are neither intended nor warranted for usage in systems or equipment,
any malfunction or failure of which may cause loss of human life, bodily injury or severe
property damage. Such applications are deemed, "Insecure Usage".

Insecure usage includes, but is not limited to: equipment for surgical implementation,
atomic energy control instruments, airplane or spaceship instruments, the control or
operation of dynamic, brake or safety systems designed for vehicular use, traffic signal
instruments, all types of safety devices, and other applications intended to support or
sustain life.

All Insecure Usage shall be made at customer's risk, and in the event that third parties
lay claims to Nuvoton as a result of customer's Insecure Usage, customer shall indemnify
the damages and liabilities thus incurred by Nuvoton.

Please note that all data and specifications are subject to change without notice. All the
trademarks of products and companies mentioned in this datasheet belong to their respective
owners.

**Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved**
