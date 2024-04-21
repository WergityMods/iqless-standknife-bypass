from flask import Flask

app = Flask(__name__)

@app.route('/standknife/dvc')
def ban_check():
    return 'false'

if __name__ == '__main__':
    app.run(port=80, debug=True)