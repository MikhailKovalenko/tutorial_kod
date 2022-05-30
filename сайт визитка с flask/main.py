from flask import Flask, render_template
import dataset

app = Flask(__name__, static_folder='static')


@app.route('/')
def index1():
    personal_qualities = db2.query("SELECT * FROM personal_qualities")
    list_personal_qualities = list(personal_qualities)
    professional_qualities = db1.query("SELECT * FROM professional_qualities")
    list_professional_qualities = list(professional_qualities)
    return render_template('syte.html', text2=list_personal_qualities, text1=list_professional_qualities)

db2 = dataset.connect('sqlite:///db/personal_qualities.db')
db1 = dataset.connect('sqlite:///db/professional_qualities.db')

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=80, debug=True, threaded=False)
