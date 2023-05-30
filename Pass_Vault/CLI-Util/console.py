import vault_func as v
import database as db
def main_menu():
    db.db_init()
    print("**********************Welcome to your Password Manager App!!*********************")
    print("\nFirst time here ?\n\n\n")
    choice = int(input("\t 1-Yes... Let me Register\n\t 2-Already been here... Let me Login\n\t 0-Quit\n"))
    while(choice!=1 and choice!=2 and choice!=0):
        print("\n\n\tI need a valid input")
        choice = int(input("\t 1-Yes... Let me Register\n\t 2-Already been here... Let me Login\n\t 0-Quit\n"))

    if (choice==1):
        v.register()
    elif (choice==2):
        v.login()
    else:
        print("GoodBye!!")


main_menu()