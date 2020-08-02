from flask import Flask #Memanggil library Flask

app = Flask(__name__)

@app.route('/')
@app.route('/index')
def index():
    return "Hello, World"