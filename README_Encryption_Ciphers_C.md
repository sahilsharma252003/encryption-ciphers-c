# 🔐 Encryption Algorithms in C: Stream & Block Ciphers with IVs and Custom Hashing

This project demonstrates the core principles of symmetric encryption by implementing stream and block cipher algorithms entirely in C. It includes secure pseudorandom number generation, Feistel-based block encryption, and stream cipher variants with and without initialization vectors (IVs).

---

## 🔐 Implemented Ciphers

### 🔸 Stream Ciphers

1. **scrypt.c**
   - Classic stream cipher using LCG (Linear Congruential Generator)
   - XORs each byte with the next keystream byte
   - Stateless encryption

2. **vcrypt.c**
   - Stream cipher with a fixed **Initialization Vector (IV)**
   - XORs hashed key with IV to derive the seed
   - Encrypts/decrypts content and appends/prepends IV

### 🔸 Block Cipher

3. **feistel.c**
   - 8-round Feistel network with 64-bit blocks (2 × 32-bit halves)
   - Uses a basic SDBM hash for key generation
   - Processes files in blocks; pads last block with zeros
   - Reversible encryption and decryption via round-wise XORing

---

## 🧪 Key Features

- **Custom SDBM Hash** for key derivation
- LCG-based pseudorandom generation
- IV-handling for secure non-repetitive streams
- Full file processing with error handling
- Makefile provided for easy compilation

---

## 📂 Files Overview

| File          | Description                                  |
|---------------|----------------------------------------------|
| `scrypt.c`    | Basic stream cipher                          |
| `vcrypt.c`    | Stream cipher with IV support                |
| `feistel.c`   | Block cipher using Feistel network           |
| `makefile`    | Build script to compile all three programs   |

---

## 🛠 Compilation

```bash
make all
```

This will generate:
- `scrypt`
- `vcrypt`
- `feistel`

---

## ▶️ How to Use

### 🔐 Stream Encryption (scrypt)

```bash
./scrypt yourpassword input.txt encrypted.txt
./scrypt yourpassword encrypted.txt decrypted.txt
```

### 🔐 Stream Encryption with IV (vcrypt)

```bash
./vcrypt -e yourpassword input.txt encrypted.txt
./vcrypt -d yourpassword encrypted.txt decrypted.txt
```

### 🔐 Block Encryption (Feistel)

```bash
./feistel -e yourpassword input.txt encrypted.txt
./feistel -d yourpassword encrypted.txt decrypted.txt
```

---

## 📚 Concepts Demonstrated

- Symmetric Key Cryptography
- Feistel Networks
- Initialization Vectors (IVs)
- Linear Congruential Generator (LCG)
- XOR Encryption
- File I/O and Block Padding in C

---

## 🙌 Authors

Sahil Sharma — March 2025  
For academic and educational demonstration of cryptographic concepts

---

## 📜 License

Open source for learning and research purposes.
