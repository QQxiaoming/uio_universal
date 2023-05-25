[![CI](https://github.com/QQxiaoming/uio_universal/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/QQxiaoming/uio_universal/actions/workflows/ci.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/qqxiaoming/uio_universal/badge)](https://www.codefactor.io/repository/github/qqxiaoming/uio_universal)
[![License](https://img.shields.io/github/license/qqxiaoming/uio_universal.svg?colorB=f48041&style=flat-square)](https://github.com/QQxiaoming/uio_universal)

# uio_universal

[English](./README.md) | 简体中文

## 介绍

这是一个使用uio的通用设备驱动，加载驱动后，会产生一个/dev/uioX设备，用户可以通过用户态应用程序编写相应的设备驱动。

## 安装

```shell
make
make load
```

## 卸载

```shell
make unload
```
