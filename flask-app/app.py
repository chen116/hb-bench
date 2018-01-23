from flask import Flask, render_template,jsonify
from multiprocessing import Value
from subprocess import call
import subprocess
import random
#import time
counter=Value('i',0)
app = Flask(__name__)

# list of cat images
images = [
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr05/15/9/anigif_enhanced-buzz-26388-1381844103-11.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr01/15/9/anigif_enhanced-buzz-31540-1381844535-8.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr05/15/9/anigif_enhanced-buzz-26390-1381844163-18.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr06/15/10/anigif_enhanced-buzz-1376-1381846217-0.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr03/15/9/anigif_enhanced-buzz-3391-1381844336-26.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr06/15/10/anigif_enhanced-buzz-29111-1381845968-0.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr03/15/9/anigif_enhanced-buzz-3409-1381844582-13.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr02/15/9/anigif_enhanced-buzz-19667-1381844937-10.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr05/15/9/anigif_enhanced-buzz-26358-1381845043-13.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr06/15/9/anigif_enhanced-buzz-18774-1381844645-6.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr06/15/9/anigif_enhanced-buzz-25158-1381844793-0.gif",
    "http://ak-hdl.buzzfed.com/static/2013-10/enhanced/webdr03/15/10/anigif_enhanced-buzz-11980-1381846269-1.gif"
]
#global cnt=0

@app.route('/')
def index():
    with counter.get_lock():
        counter.value +=1
    url = random.choice(images)
#    proc = subprocess.Popen(['./dijkstra_small','input.dat'], stdout=subprocess.PIPE)
#    proc = subprocess.Popen(['./basicmath_large'], stdout=subprocess.PIPE)
#    proc = subprocess.Popen(['./cyclictest', '-p99','-n' ,'-D' ,'2' ,'-h30', '-q'], stdout=subprocess.PIPE)
#    proc = subprocess.Popen(['sudo','./run.sh','-t','all','-l','1'], stdout=subprocess.PIPE,stderr=subprocess.PIPE,cwd='./ltp/testcases/realtime')
#    proc = subprocess.Popen(['./run.sh','-t','func','-l','1'], stdout=subprocess.PIPE,stderr=subprocess.PIPE,cwd='./ltp/testcases/realtime',universal_newlines=True)
#    proc = subprocess.Popen(['sysbench','--test=cpu','--cpu-max-prime=2000','run'], stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True)
    proc = subprocess.Popen(['./susan/vic','./susan/input_large.pgm','vicout','-s','-3'], stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=False)
#    proc = subprocess.Popen(['./runme_xeon64'], stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True,cwd='./linpack')
#    proc = subprocess.Popen(['./runme_xeon64'], stdout=subprocess.PIPE,stderr=subprocess.PIPE,cwd='./linpack',universal_newlines=False)
   # return render_template('index.html', url=url, st=proc.stdout.read(),er=proc.stderr.read(),cntt=counter.value)
     #v=proc.stdout.read().split('\n')
    return render_template('index2.html',st=proc.stdout.read(),er=proc.stderr.read(),cntt=counter.value)




if __name__ == "__main__":
    call(["ls"])
#    proc = subprocess.Popen(['./cyclictest -p99 -n -D 2 -h 30 -q'], stdout=subprocess.PIPE)

    app.run(host="0.0.0.0",processes=1)
