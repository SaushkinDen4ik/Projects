import sqlite3

connection1 = sqlite3.connect('database.db')
with open('schema.sql') as f:
    connection1.executescript(f.read())

connection2 = sqlite3.connect('feedback.db')
with open('tablefeedback.sql') as f:
    connection2.executescript(f.read())

cur = connection1.cursor()

cur.execute("INSERT INTO posts (title, content) VALUES (?, ?)",
            ('For examples', 'It is your first note. You can edit it')
            )

connection1.commit()
connection1.close()
