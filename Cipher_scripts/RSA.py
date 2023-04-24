import random
import math

def KeyGen():
    p = random.randint(1, 10**4)
    q = random.randint(1, 10**4)
    while not ((is_prime(p)) and (is_prime(q))):
        p = random.randint(1, 10**4)
        q = random.randint(1, 10**4)
    n = p*q
    f_n = (p-1)*(q-1)
    e = f_n

    while(egcd(e, f_n)[0]!=1):
        e = random.randint(1, f_n)
    
    d = modinv(e, f_n)

    public_key = [n, e]
    private_key = [n, d]

    print("Votre clé publique est le couple:", public_key)
    print("Votre clé privée est le couple:", private_key)

    return (public_key, private_key)

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    g, y, x = egcd(b%a,a)
    return (g, x - (b//a) * y, y)

def modinv(a, m):
    g, x = egcd(a, m)[0:2]
    if g != 1:
        raise Exception('No modular inverse')
    return x%m

def is_prime(a):
    if a < 0:
        return False
    else:
        racine = int(math.sqrt(a))
    for i in range(2, racine+1):
        if a%i == 0:
            return False
    return True

def encrypt_rsa(pubkey, message):
    n, e = pubkey
    codes = [ord(c) for c in message if c.isalpha()]
    cipher = []
    for c in codes:
        cipher.append(str(pow(c, e, n)))
    
    return " ".join(cipher)
 
def decrypt_rsa(privkey, cipher):
    n, d = privkey
    codes = [int(c) for c in cipher.split()]
    message = ""
    for code in codes:
        message += chr(pow(code, d, n))
    return message


pkey, prkey = KeyGen()
text1 = encrypt_rsa(pkey, "Babe")
text2 = decrypt_rsa(prkey, text1)

print("Texte chiffré: ",text1,"Texte clair: ",text2)
