# Y86-64 Simulator

A Y86-64 processor simulator written in JavaScript (Node.js) and C++11.

![Y86-64 Simulator](./assets/screenshot.png)

Y86-64 Simulator is a well-designed coursework project attached to Chap. 4 of *Computer Systems: A Programmer's Perspective, Third Edition*. For more details, please refer to *[CS:APP3e Guide to Y86-64 Processor Simulators](http://csapp.cs.cmu.edu/3e/simguide.pdf)*.

## Table of Contents

- [Y86-64 Simulator](#y86-64-simulator)
  - [Table of Contents](#table-of-contents)
  - [Getting Started](#getting-started)
    - [0. Prerequisites](#0-prerequisites)
      - [0.1 Back-end](#01-back-end)
      - [0.2 Front-end](#02-front-end)
    - [1. Installation](#1-installation)
      - [1.1 Back-end](#11-back-end)
        - [1.1.1 Windows](#111-windows)
        - [1.1.2 Linux](#112-linux)
      - [1.2 Front-end](#12-front-end)
    - [2. Usage](#2-usage)
    - [3. Clean](#3-clean)
      - [3.1 Back-end](#31-back-end)
        - [3.1.1 Windows](#311-windows)
        - [3.1.2 Linux](#312-linux)
      - [3.2 Front-end](#32-front-end)
    - [4. Test](#4-test)
  - [TODO](#todo)
  - [Contributors](#contributors)
  - [License](#license)

## Getting Started

### 0. Prerequisites

To set up the environment, you need to have the following dependencies installed.

#### 0.1 Back-end

- [CMake](https://cmake.org/download) 2.8 or above
- [GNU make](https://www.gnu.org/software/make) 4.0 or above
- [GCC](https://gcc.gnu.org/releases.html) 4.8 or above

For Windows, try [MinGW-w64](http://mingw-w64.org/doku.php/download).

#### 0.2 Front-end

- [MDUI](https://www.mdui.org)
- [Node.js](https://nodejs.org/en/download) 12 or above

### 1. Installation

#### 1.1 Back-end

##### 1.1.1 Windows

```bash
cd lib
./scripts/install.ps1
```

##### 1.1.2 Linux

```bash
cd lib
bash ./scripts/install.sh
```

#### 1.2 Front-end

```bash
cd app
npm install
npm run build
```

### 2. Usage

```bash
cd app
node server
```

Then check the website at <http://localhost:7273>.

To deploy and run the project on a server continuously, try [pm2](https://www.npmjs.com/package/pm2).

To use a command line version, see the document [here](./lib/README.md).

### 3. Clean

#### 3.1 Back-end

##### 3.1.1 Windows

```bash
cd lib
./scripts/clean.ps1
```

##### 3.1.2 Linux

```bash
cd lib
bash ./scripts/clean.sh
```

#### 3.2 Front-end

```bash
cd app
rm -rf dist/*
```

### 4. Test

Test files are located in `./test/object`. Currently only `.yo` files are accepted.

Click the 'Upload' button to get started.

## TODO

- [ ] Implement a memory status section
- [ ] Compose a detailed document

## Contributors

- [**Hakula Chen**](https://github.com/hakula139)<[i@hakula.xyz](mailto:i@hakula.xyz)> - Fudan University
- [**Jiaqi Dai**](https://github.com/jqdai) - Fudan University

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](./LICENSE) file for details.
