# Y86-64 Simulator UI

An interface for Y86-64 Simulator written in JavaScript (Node.js).


## Table of Contents

- [Y86-64 Simulator UI](#y86-64-simulator-ui)
  - [Table of Contents](#table-of-contents)
  - [Getting Started](#getting-started)
    - [0. Prerequisites](#0-prerequisites)
    - [1. Building](#1-building)
    - [2. Usage](#2-usage)
    - [3. Clean](#3-clean)
  - [Contributors](#contributors)
  - [License](#license)


## Getting Started

To set up the environment, you need to have the following dependencies installed.

### 0. Prerequisites

- [MDUI](https://www.mdui.org)
- [Node.js](https://nodejs.org/en/download) 12 or above

### 1. Building

```bash
npm install
npm run build
```

### 2. Usage

```bash
node server
```

Then check the website at <http://localhost:7273>.

To deploy and run the project on a server continuously, try [pm2](https://www.npmjs.com/package/pm2).

### 3. Clean

```bash
cd app
rm -rf dist/*
```

## Contributors

- [**Hakula Chen**](https://github.com/hakula139)<[i@hakula.xyz](mailto:i@hakula.xyz)> - Fudan University

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](../LICENSE) file for details.
