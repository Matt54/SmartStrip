#LED-control

from flask import Flask, request
import smbus

bus = smbus.SMBus(1)
address=0x08

app = Flask(__name__) #create the Flask app

@app.route('/CONTROL_LEDS', methods=['GET', 'POST']) #allow both GET and POST r$
def formexample():
        if request.method == 'POST': #this block is only entered when the form $
                timing = request.form.get('timing')
                mod = request.form.get('mod')
                program_number = request.form.get('program_number')
                if(timing != ""):
                		write(0,timing)
            	if(mod != ""):
        				write(1,mod)
    			if(program_number != ""):
        				write(2,program_number)

		return '''<form method="POST">
                        Timing: <input type="number" name="timing"><br>
                        Mod: <input type="number" name="mod"><br>
                        Program Number: <input type="number" name="program_number"><br>
                        <input type="submit" value="Submit"><br>
                 </form>'''

def write(register,value):
        bus.write_byte_data(address,register,value)
        return -1

if __name__ == "__main__":
        app.run(host='0.0.0.0',port=2525,debug=True) #run app in debug mode on port 2525
