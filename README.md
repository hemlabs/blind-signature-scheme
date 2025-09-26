# C++ Blind Signature Protocol

*A C++ command-line implementation of the RSA Blind Signature cryptographic protocol.*

---

## About The Project

This project is a C++ application that demonstrates the concept of a **blind signature**. This is a cryptographic scheme where a message is signed by an authority (the "Signer") without the authority ever knowing the content of the message.

This is useful in privacy-critical applications, such as digital voting systems, where a voter needs their ballot to be authorized without the authority being able to link the ballot back to the voter.

This implementation includes the core cryptographic functions:
* RSA Key Generation
* Message Blinding and Unblinding
* Signing and Verification

---

## Getting Started

To get a local copy up and running, follow these simple steps.

### Prerequisites

You will need a C++ compiler, such as **g++**, installed on your system.

### Compilation

1.  Clone the repository:
    ```sh
    git clone [https://github.com/your-username/your-repository-name.git](https://github.com/your-username/your-repository-name.git)
    ```
2.  Navigate to the project directory:
    ```sh
    cd your-repository-name
    ```
3.  Compile the source code. The following command will create an executable file named `blind_signature`:
    ```sh
    g++ main.cpp -o blind_signature
    ```
    *(Note: If you included a `Makefile`, you can simply run the `make` command instead.)*

---

## Usage

Once compiled, run the program from your terminal:
```sh
./blind_signature