def mod26(det):
    if det>0:
        return det%26

    else:
        while det<0:
            det = det+26

        return det

def pgcd(a, b):
    if a == 0:
        return (b, 0, 1)
    g, y, x = pgcd(b%a,a)
    return (g, x - (b//a) * y, y)

def chooseVal():
    print("Entrez les 4 éléments de votre matrice(clé de chiffrement)")
    x = int(input("Valeur 1-1:"))
    y = int(input("Valeur 1-2:"))
    z = int(input("Valeur 2-1:"))
    t = int(input("Valeur 2-2:"))

    print("Voici votre matrice: \n", x, " ", y,"\n", z," ",t, "\n")
    N = [[x,y],[z,t]]
    return N

def chooseMat():
    M = chooseVal()
    det = mod26((M[0][0]*M[1][1]) - (M[1][0]*M[0][1]))

    while (det==0):
        M = chooseVal()

    print("La matrice entrée est inversible \n")

    if (pgcd(det, 26)[0])==1:
        print("La matrice M peut être utilisée comme clé d'un cryptosystème de Hill")
        return M
    else:
        print("La matrice ne peut être utilisée comme clé de chiffrement")
        chooseMat()

def modinv(a, m):
    g, x = pgcd(a, m)[0:2]
    if g != 1:
        raise Exception('No modular inverse')
    return x%m

def invMat(M):
    invdet = modinv(mod26((M[0][0]*M[1][1]) - (M[1][0]*M[0][1])), 26)
    D = [mod26( M[1][1] * invdet), mod26( -(M[0][1]) * invdet)],[ mod26(-M[1][0] * invdet), mod26( M[0][0] * invdet)]
    return D

def hill(M, message):
    texte = message
    if len(texte) % 2 != 0:
        texte += "A"

    blocs = [texte[i:i+2] for i in range(0, len(texte), 2)]
    matrice = []
    for bloc in blocs:
        colonne = []
        for lettre in bloc:
            colonne.append(ord(lettre) - 65)
        matrice.append(colonne)

    enc = [0,0]
    ciphertext = []
    for col in matrice:
        enc[0] = (M[0][0]*col[0]+M[0][1]*col[1])%26
        enc[1] = (M[1][0]*col[0]+M[1][1]*col[1])%26
        ciphertext.append(enc.copy())

    ascii_text = ""
    for col in ciphertext:
        for num in col:
            ascii_text += chr(num % 26 + 65)

    print(ascii_text)
    