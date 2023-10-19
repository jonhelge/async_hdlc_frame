![C/C++ CI](https://github.com/jonhelge/async_hdlc_frame/actions/workflows/main.yml/badge.svg)

# Async HDLC frame Encoder-Decoder

## Introduction

High-Level Data Link Control (HDLC) is a data link layer (Layer 2) protocol for reliable and efficient communication over a synchronous serial link. It's particularly useful for sending data over UART between microcontrollers, ensuring data integrity and allowing for more complex command-response interaction between devices. This repository contains a C implementation of an minimal HDLC encoder and decoder, making it easy to integrate HDLC into embedded projects. 

**Note:** This implementation provides a minimalistic and functional HDLC framing mechanism. However, it's worth noting that it does not cover the full HDLC specification

**Inspiration:** This is more or less an implementation on what is outlined in this stackoverflow post: https://stackoverflow.com/a/815813

## Features

- Simple and lightweight
- Uses CRC-16 for data integrity
- Supports escape characters to avoid frame boundary bytes in the payload
- Compatible with both microcontrollers and standard computers

## How to Compile and Run

1. Clone the repository:

    ```
    git clone https://github.com/jonhelge/async_hdlc_frame.git
    ```

2. Navigate to the project directory:

    ```
    cd async_hdlc_frame
    ```

3. Compile the code:

    ```
    gcc -o hdlc_test hdlc.c main.c
    ```

4. Run the compiled program:

    ```
    ./hdlc_test
    ```

## Expected Output

After running the compiled program, you should see output similar to the following:

```text
Testing MY_COMMAND...
Encoded frame with command 01: 7E 01 06 12 34 7D 5E 7D 5D 56 78 XX XX 7E 
Decoded successfully!
Decoded command: 01
Decoded payload: 12 34 7E 7D 56 78

Testing CONTROL_COMMAND...
Encoded frame with command 02: 7E 02 04 AA BB CC DD XX XX 7E 
Decoded successfully!
Decoded command: 02
Decoded payload: AA BB CC DD
```

**Note:** The `XX XX` in the encoded frame represents the CRC-16, which will vary based on your payload.



