import hashlib, os, string, re, random, database
import smtplib, ssl, time, getpass


def create_regex():
    print("Choose the type of password:")
    print("1 - Alphanumeric (letters and digits)")
    print("2 - Alphanumeric with special characters")
    print("3 - Custom regular expression")
    choice = input("Enter your choice: ")

    if choice == "1":
        return r"[\w\d]+"
    elif choice == "2":
        return r"[\w\d\W]+"
    elif choice == "3":
        print("Enter a custom regular expression to match the password requirements:")
        regex = input("Regex: ")
        try:
            re.compile(regex)
            return regex
        except re.error:
            print("Invalid regular expression. Using the default value.")
            return None
    else:
        print("Invalid choice. Using the default value.")
        return None

def gen_password(length,regex = None):
    regex = create_regex()
    chars = string.ascii_letters + string.digits + string.punctuation
    if regex:
        chars = ''.join(re.findall(regex, chars))
    
    password = ''.join(random.choice(chars) for i in range(length))
    
    return password

def store_password(user, mdp, service):
    print("Let's store a new password for ", user)
    database.new_passwd(user, service, mdp)
    
def logged_menu(user):
    while True:
        print("\n**********************Logged Menu*********************")
        print(f"\nWhat would you like to do dear {user}?")
        print("\t1 - Generate a new password")
        print("\t2 - Store a new password")
        print("\t3 - List stored passwords")
        print("\t4 - Quit")

        choice = input("Enter your choice: ")

        if choice == "1":
            length = int(input("Enter the desired length of the password: "))
            password = gen_password(length)
            print("Generated Password:", password)
            c = int(input("Do you wanna store this password ? \n1 - Yes\n2 - No"))
            if(c==1):
                s = input("Enter a service name for your new password: ")
                store_password(user, password, s)
            else:
                print("Done")

        elif choice == "2":
            service = input("Enter the service or website name: ")
            password = getpass.getpass("Enter the password: ")
            store_password(user, password, service)

        elif choice == "3":
            passwords = database.list_passwd(user)
            if passwords:
                print("Stored Passwords:")
                for (password, service) in passwords:
                    print(f"Service: {service}, Password: {password}")
            else:
                print("No stored passwords found.")

        elif choice == "4":
            print("Logging out...")
            break

        else:
            print("Invalid choice. Please try again.")

def login(addr=None):
    if (addr is None):
        addr = input("Enter the mail address associated to your account: ")
    if database.checkmail(addr):
        print("A login code will be sent to you. Use it within 5 mins")
        logincode, timestamp = send_mail(addr)

    else:
        print("Enter a valid email or register")
        return
    
    usercode = getpass.getpass("Enter the code sent to your mail: ")
    if(usercode == logincode):
        if (time.time()<timestamp + 300):
            user = database.checkmail(addr)
            print("Logged succesfully")
            logged_menu(user)
        
        else:
            print("This code is expired")
            return False
    else:
        print("Wrong code, Try again")
        return False
    
def register():
    mail = input("Enter your email address: ")
    name = input("How should i call you: ")
    while(database.checkmail(mail)==True):
        mail = input("Use another mail address: ")
    database.new_user(name, mail)
    login(mail)
    
def send_mail(r_mail):
    smtp_server = "smtp.gmail.com"
    port = 587  # For starttls
    sender_email = "erdhal.jerry@gmail.com"
    receiver_email = r_mail
    code = ''.join(str(random.randint(0, 9)) for _ in range(6))
    message = f"""\
    Subject: no@reply

    This message is sent from Python.
    
    This is your login code: {code}
    
    The code will soon be unusable"""

    timestamp = time.time()

    # Try to log in to the server and send the email
    try:
        with smtplib.SMTP(smtp_server, port) as server:
            server.ehlo()  # Can be omitted
            server.starttls()  # Secure the connection
            server.ehlo()  # Can be omitted
            server.login(sender_email, "csmubsgwrdhosknf")
            server.sendmail(sender_email, receiver_email, message)
    except Exception as e:
        # Print any error messages to stdout
        print(e)

    return code, timestamp
