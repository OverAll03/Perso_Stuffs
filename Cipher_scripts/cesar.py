

word=input("Enter a string: ")
word=word.lower()
l=len(word)
c=list(range(l))
pas=int(input("Enter a number: "))
choice=0

while (choice!=1 and choice!=2):
    choice=int(input("1- Cipher forward\n2- Cipher Backward\n\n"))
    if (choice==1):
        print("Cipher your text forward")
        for i in range (l):
            if (word[i]!=' '):
                if (ord(word[i])+pas)<(ord('z')+1):
                    c[i]=chr(ord(word[i])+pas)
                else:
                    c[i]=chr(96+pas-(ord('z')-ord(word[i])))
            else:
                c[i]=' '

    elif (choice==2):
        print("Cipher your text backward")
        for i in range (l):
            if (word[i]!=' '):
                if (ord(word[i])-pas)>(ord('a')-1):
                    c[i]=chr(ord(word[i])-pas)
                else:
                    c[i]=chr(123-pas-(ord('a')-ord(word[i])))
            else:
                c[i]=' '

    else:
        continue


print(''.join(c))