import random
import json
import sys
from itertools import combinations

# character set: space + A-Z (27 characters total,  5 bits each)
ALPHABET = ' ABCDEFGHIJKLMNOPQRSTUVWXYZ'


def matrix_multiply_mod2(matrix_a, matrix_b):
    rows_a = len(matrix_a)
    cols_b = len(matrix_b[0])
    cols_a = len(matrix_a[0])

    result = []
    for i in range(rows_a):
        row = []
        for j in range(cols_b):
            element = sum(matrix_a[i][k] * matrix_b[k][j]
                          for k in range(cols_a)) % 2
            row.append(element)
        result.append(row)

    return result


def text_to_bits(text):
    text = text.upper()

    # validation
    for char in text:
        if char not in ALPHABET:
            raise ValueError(f"invalid character '{
                             char}'. Use only: {ALPHABET}")

    # convert to 5-bit binary string
    bits = []
    for char in text:
        position = ALPHABET.index(char)
        binary_string = f'{position:05b}'
        for bit in binary_string:
            bits.append(int(bit))

    return bits


def encrypt_mceliece(plaintext, public_key_file='public.json', output_file='ciphertext.json'):
    # load public key
    with open(public_key_file, 'r') as f:
        public_key = json.load(f)

    G_pub = public_key['G_pub']
    t = public_key['t']

    k = len(G_pub)        # number of rows = message block size
    n = len(G_pub[0])     # number of columns = codeword length

    print(f"Public key parameters: k={k} (message bits), n={
          n} (codeword bits), t={t} (errors)")

    # plaintext to bits
    message_bits = text_to_bits(plaintext)
    print(f"Message: '{plaintext}' -> {len(message_bits)} bits")

    # encrypt each k-bit block
    ciphertext_blocks = []

    for block_start in range(0, len(message_bits), k):
        message_block = message_bits[block_start:block_start + k]

        # pad with zeros if it's shorter than k
        padding_needed = k - len(message_block)
        if padding_needed > 0:
            message_block = message_block + [0] * padding_needed

        # encode using public generator matrix
        # message_block = 1 x k
        # G_pub = k x n
        # result = 1 x n
        codeword = matrix_multiply_mod2([message_block], G_pub)[0]

        # add t random errors (flip t random bits)
        error_positions = random.sample(range(n), t)
        for position in error_positions:
            codeword[position] ^= 1  # xor

        ciphertext_blocks.append(codeword)

    # save to file
    with open(output_file, 'w') as f:
        json.dump(ciphertext_blocks, f)

    print(f"Encrypted {len(plaintext)} characters -> {output_file}")
    print(f"Created {len(ciphertext_blocks)} ciphertext blocks")


# main execution
if __name__ == "__main__":
    if len(sys.argv) > 1:
        plaintext = sys.argv[1]
    else:
        plaintext = input("Plaintext to encrypt: ")

    encrypt_mceliece(plaintext)
