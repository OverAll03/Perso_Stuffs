import HillFunctions

M = HillFunctions.chooseMat()
D = HillFunctions.invMat(M)

while True:
    print("Que voulez-vous faire ?")
    print("1 - Chiffrer un message")
    print("2 - Déchiffrer un message")
    print("3 - Quitter")
    choix = input("Entrez votre choix (1, 2 ou 3) : ")
    
    if choix == "1":
        print("Vous avez choisi de chiffrer un message.")
        texte1 = input("Entrez le message à chiffrer: \n").upper()
        HillFunctions.hill(M,texte1)

    elif choix == "2":
        print("Vous avez choisi de déchiffrer un message.")
        texte2 = input("Entrez le message à déchiffrer: \n").upper()
        while (len(texte2)%2)!=0:
            print("Entrez un texte de longueur paire...")
            texte2 = input("Entrez le message à déchiffrer: ").upper()
        HillFunctions.hill(D, texte2)

    elif choix == "3":
        print("GoodBye!!!!")
        break

    else:
        print("Choix invalide. Veuillez entrer 1, 2 ou 3.")
