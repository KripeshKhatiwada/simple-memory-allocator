# Simple Memory Allocator

This project is a basic memory allocator written in C++. It simulates `malloc`-like behavior using a memory pool and free list.

## 🧪 Features
- Custom memory allocation and deallocation
- Free list management
- Block size tracking

## 🧰 Technologies
- C++
- C-style casting

## 🚀 Usage
1. Compile: `g++ main.cpp -o allocator`
2. Run: `./allocator`

## 📁 File
- `main.cpp`: contains allocator logic and usage test

## 🧠 How It Works
A fixed-size array simulates memory. Blocks are tracked using a `Block` struct and a linked free list.

## 🖼️ Diagram
![Memory layout](layout.png) <!-- optional -->
