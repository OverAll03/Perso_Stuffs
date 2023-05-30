import sqlite3

def db_init():
    # Connexion à la base de données
    conn = sqlite3.connect('pass.db')
    cursor = conn.cursor()

    # Création de la table des utilisateurs
    cursor.execute('''CREATE TABLE IF NOT EXISTS users
                    (id INTEGER PRIMARY KEY AUTOINCREMENT,
                    username TEXT NOT NULL,
                    email TEXT NOT NULL)''')

    # Création de la table des mots de passe
    cursor.execute('''CREATE TABLE IF NOT EXISTS passwords
                    (id INTEGER PRIMARY KEY AUTOINCREMENT,
                    user_id INTEGER NOT NULL,
                    service TEXT NOT NULL,
                    password TEXT NOT NULL,
                    FOREIGN KEY(user_id) REFERENCES users(id))''')
    
    # Fermeture de la connexion à la base de données
    conn.close()

def get_user_id(username):
    # Se connecter à la base de données
    conn = sqlite3.connect('pass.db')
    c = conn.cursor()

    # Exécuter la requête SELECT
    c.execute("SELECT id FROM users WHERE username=?", (username,))
    result = c.fetchone()

    # Fermer la connexion
    conn.close()

    # Vérifier si l'utilisateur existe et retourner l'ID
    if result:
        return result[0]
    else:
        return None


def new_user(username, mail):
    # Connexion à la base de données
    conn = sqlite3.connect('pass.db')
    cursor = conn.cursor()
    cursor.execute('''INSERT INTO users (username, email) VALUES(? , ? )''', (username, mail))
    conn.commit()
    conn.close()
    print("User", username,"succesfully created")

def new_passwd(username, service, passwd):
    # Connexion à la base de données
    conn = sqlite3.connect('pass.db')
    cursor = conn.cursor()
    userid = get_user_id(username)
    # Ajouter le nouveau mot de passe
    cursor.execute("INSERT INTO passwords (service, password, user_id) VALUES (?, ?, ?)", (service, passwd, userid))
    conn.commit()
    conn.close()

def list_passwd(username):
    conn = sqlite3.connect('pass.db')
    cursor = conn.cursor()
    userid = get_user_id(username)
    cursor.execute("SELECT password, service FROM passwords WHERE id=?", (userid,))
    result = cursor.fetchall()
    conn.commit()
    conn.close()
    return result

def checkmail(addr):
    conn = sqlite3.connect('pass.db')
    c = conn.cursor()

    c.execute("SELECT username FROM users WHERE email=?", (addr,))
    result = c.fetchall()
    conn.close()

    if not result:
        print("No user associated with this email")
        return False
    else:
        print("Welcome back, dear", result[0][0])
        return result[0][0]
    