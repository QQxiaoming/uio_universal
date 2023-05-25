[![CI](https://github.com/QQxiaoming/uio_universal/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/QQxiaoming/uio_universal/actions/workflows/ci.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/qqxiaoming/uio_universal/badge)](https://www.codefactor.io/repository/github/qqxiaoming/uio_universal)
[![License](https://img.shields.io/github/license/qqxiaoming/uio_universal.svg?colorB=f48041&style=flat-square)](https://github.com/QQxiaoming/uio_universal)

# uio_universal

English | [简体中文](./README_zh_CN.md)

## Introduction

This is a universal device driver using uio. After loading the driver, a /dev/uioX device will be generated. Users can write corresponding device drivers through user mode applications.

## Install

```shell
make
make load
```

## Uninstall

```shell
make unload
```
