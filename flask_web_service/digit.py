import sqlite3
from flask import Flask, render_template, request, url_for, flash, redirect


def get_db_connection():
    conn = sqlite3.connect('database.db')
    conn.row_factory = sqlite3.Row
    return conn


def get_note(note_id):
    conn = get_db_connection()
    note = conn.execute('SELECT * FROM posts WHERE id = ?',
                        (note_id,)).fetchone()
    conn.close()
    return note


app = Flask(__name__)
app.config['SECRET_KEY'] = 'your secret key'  # зачем нужен


@app.route('/')
def index():
    conn = get_db_connection()
    posts = conn.execute('SELECT * FROM posts').fetchall()
    conn.close()
    return render_template('index.html', posts=posts)


@app.route('/<int:post_id>')
def post(post_id):
    post = get_note(post_id)
    return render_template('post.html', post=post)


@app.route('/create', methods=('GET', 'POST'))
def create():
    if request.method == 'POST':
        title = request.form['title']
        content = request.form['content']

        if not title:
            flash('Title is required!')
        else:
            conn = get_db_connection()
            conn.execute('INSERT INTO posts (title, content) VALUES (?, ?)',
                         (title, content))
            conn.commit()
            conn.close()
            return redirect(url_for('index'))

    return render_template('create.html')


@app.route('/about')
def about():
    return render_template('about.html')


@app.route('/<int:id>/edit', methods=('GET', 'POST'))
def edit(id):
    post = get_note(id)

    if request.method == 'POST':
        title = request.form['title']
        content = request.form['content']

        if not title:
            flash('Title is required!')
        else:
            conn = get_db_connection()
            conn.execute('UPDATE posts SET title = ?, content = ?'
                         ' WHERE id = ?',
                         (title, content, id))
            conn.commit()
            conn.close()
            return redirect(url_for('index'))

    return render_template('edit.html', post=post)


@app.route('/<int:id>/delete')
def delete(id):
    post = get_note(id)
    conn = get_db_connection()
    conn.execute('DELETE FROM posts WHERE id = ?', (id,))
    posts = conn.execute('SELECT * FROM posts').fetchall()
    conn.commit()
    conn.close()
    flash('"{}" was successfully deleted!'.format(post['title']))
    return render_template('index.html', posts=posts)


@app.route('/removeall')
def remove_all():
    conn = get_db_connection()
    conn.execute('DELETE FROM posts')
    posts = conn.execute('SELECT * FROM posts').fetchall()
    conn.commit()
    conn.close()
    return render_template('index.html', posts=posts)


@app.route('/feedback', methods=('GET', 'POST'))
def feedback():
    if request.method == 'POST':
        email = request.form['title']
        feedback_text = request.form['content']
        if not email:
            flash('Email address is required!')
        if not feedback_text:
            flash('Text is required!')
        else:
            conn = sqlite3.connect('feedback.db')
            conn.row_factory = sqlite3.Row
            conn.execute('INSERT INTO feedback (email, feedback_text) VALUES (?, ?)',
                         (email, feedback_text))
            conn.commit()
            conn.close()
            flash('Thanks for your feedback! It is important to us.')
            return redirect(url_for('index'))

    return render_template('feedback.html')
