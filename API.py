from flask import Flask, jsonify, request
import os
import pickle
import signal
import numbers
import time
import numpy as np
import subprocess

app = Flask(__name__)
PORT = 8080
data = dict()
model = dict()


def deserialization():
    data=dict()
    model=dict()
    if (os.path.exists("./data.efrei")):
        file=open("./data.efrei", 'rb')
        data = pickle.loads(file.read())
        file.close()
    if (os.path.exists("./model.efrei")):
        file=open("./model.efrei", 'rb')
        model = pickle.loads(file.read())
        file.close()
    print("Data and models imported")
    return (data,model)



def serialization(a, b):
    pickle.dump(data, open('./data.efrei', 'wb'))
    pickle.dump(model, open('./model.efrei', 'wb'))
    print("Data and models exported")
    exit(0)


@app.route('/record', methods=['POST', 'GET'])
def record():
    # record timestamp
    timestamp = int(time.time() * 1000)
    # building error answer
    answer = {
        'message': 'Bad Request',
        'timestamp': timestamp,
        'status_code': 400
    }
    # Get the json request
    content = request.get_json(silent=True)
    # Checking if everything is fine with the json
    # Check that the json exist, that the json follows the standard (sensor name, data  and data is numeric or list)
    if content is not None \
            and 'sensor' in content.keys() \
            and 'data' in content.keys() \
            and (
            isinstance(content['data'], numbers.Number)
            or (isinstance(content['data'], list)
                and len(content['data']) > 0
                and isinstance(content['data'][0], numbers.Number))):
        # Get the ip inorder to find the computer sensor
        ip = request.environ.get('HTTP_X_REAL_IP', request.remote_addr)
        # If it's the first occurence from the computer, then it records it
        if (ip not in data.keys()):
            data.update({ip: dict()})
        val = content['data']
        # If it's the first data sent by the sensor then it records or updates it
        if (content['sensor'] not in data.get(ip).keys()):
            data.get(ip).update({content['sensor']: np.array(val)})
        else:
            data[ip][content['sensor']] = np.append(data[ip][content['sensor']], val, axis=0)
        # Builds the succes answer and reminds the user of the ip that the server used to record
        answer = {
            'message': 'Created',
            'ip': ip,
            'timestamp': timestamp,
            'status_code': 201
        }
    return jsonify(answer)
'''
{
    IP1:{
            CAPTEUR1:[VALEURS CAPTE DU CAPTEUR 1],
            CAPTEUR2:[VALEURS CAPTE DU CAPTEUR 2],
            CAPTEUR3:[VALEURS CAPTE DU CAPTEUR 3],
    }
    IP2:{
            CAPTEUR1:[VALEURS CAPTE DU CAPTEUR 1],
            CAPTEUR2:[VALEURS CAPTE DU CAPTEUR 2],
            CAPTEUR3:[VALEURS CAPTE DU CAPTEUR 3],
    }
}
'''

@app.route('/train', methods=['POST', 'GET'])
def train():
    '''
    Input should be like the following:
    {
        'modelName': model name,

        'parameter':{
                        IP1:{
                                sensor1:[ARRAYOFINDEX]
                                sensor2:[ARRAYOFINDEX]
                        }
                        IP2:{
                                sensor1:[ARRAYOFINDEX]
                                sensor2:[ARRAYOFINDEX]
                        }
                        ...
        }
        result:{
                        IP:{
                            sensor:[ARRAYOFINDEX]
                        }
        }
        numberOfIteration:10000
    }
    :return:
    '''
    # record timestamp
    timestamp = int(time.time() * 1000)
    # building error answer
    answer = {
        'message': 'Bad Request',
        'timestamp': timestamp,
        'status_code': 400
    }
    # Get the json request
    content = request.get_json(silent=True)
    # Checking if everything is fine with the json
    # Check that the json exist, that the json follows the standard (parameters and result which are lists and name which is string)
    if content is not None \
            and 'modelName' in content.keys() \
            and 'parameters' in content.keys() \
            and 'result' in content.keys() \
            and 'numberOfIteration' in content.keys() \
            and 'modelType' in content.keys()    \
            and isinstance(content["modelName"], unicode) \
            and isinstance(content["parameters"], dict) \
            and isinstance(content["result"], dict) \
            and isinstance(content["numberOfIteration"], numbers.Number)\
            and isinstance(content["modelType"], numbers.Number):
        Xcsvbuildup = None
        Ycsvbuildup = None
        # IP
        for i in content['parameters'].keys():
            if (i in data.keys()
                    and isinstance(content["parameters"][i], dict)):
                # Sensors
                for j in content['parameters'][i].keys():
                    if (j in data[i].keys()
                            and isinstance(content['parameters'][i][j], list)
                            and len(content['parameters'][i][j]) > 0
                            and isinstance(content['parameters'][i][j][0], numbers.Number)
                            and min(content['parameters'][i][j]) >= 0
                            and max(content['parameters'][i][j]) < len(data[i][j])):
                        arr = data[i][j][content['parameters'][i][j]]
                        if Xcsvbuildup is None:
                            Xcsvbuildup = np.array([arr])
                        elif len(arr) is len(Xcsvbuildup[0]):
                            Xcsvbuildup = np.append(Xcsvbuildup, [arr], axis=0)
        for i in content['result'].keys():
            if (i in data.keys()
                    and isinstance(content["result"][i], dict)):
                # Sensors
                for j in content['result'][i].keys():
                    if (j in data[i].keys()
                            and isinstance(content['result'][i][j], list)
                            and len(content['result'][i][j]) > 0
                            and isinstance(content['result'][i][j][0], numbers.Number)
                            and min(content['result'][i][j]) >= 0
                            and max(content['result'][i][j]) < len(data[i][j])):
                        Ycsvbuildup = np.array([data[i][j][content['result'][i][j]]])

            model.update({content['modelName']:{
                'modelType':content["modelType"],
                'nbParam':len(Xcsvbuildup)
            }})
            np.savetxt(content['modelName'] + ".X", Xcsvbuildup.transpose(), delimiter=";")
            np.savetxt(content['modelName'] + ".Y", Ycsvbuildup.transpose(), delimiter=";")
            # ADD code execution
            process = subprocess.Popen(["./APIMLProject", "0", str(content['modelType']), content['modelName'], str(content['numberOfIteration'])],
                                       stdout=subprocess.PIPE)
            answer = {
                'message': 'Training process launched',
                'timestamp': timestamp,
                'status_code': 201
            }
    return answer


@app.route('/predict', methods=['POST', 'GET'])
def predict():
    '''
        Input should be like the following:
        {
            'modelName': model name,
            'parameters':[[POINT1],[POINT2],...]
        }
        :return:
        '''
    # record timestamp
    timestamp = int(time.time() * 1000)
    # building error answer
    answer = {
        'message': 'Bad Request',
        'timestamp': timestamp,
        'status_code': 400
    }
    # Get the json request
    content = request.get_json(silent=True)
    # Checking if everything is fine with the json
    # Check that the json exist, that the json follows the standard (parameters and result which are lists and name which is string)
    if content is not None \
            and 'modelName' in content.keys() \
            and 'parameters' in content.keys() \
            and isinstance(content["modelName"], unicode) \
            and isinstance(content["parameters"], list)\
            and len(content["parameters"])>0\
            and isinstance(content["parameters"][0], list)\
            and len(content["parameters"][0])==model[content["modelName"]]["nbParam"]:
        arr = np.array(content["parameters"])
        np.savetxt(content['modelName'] + ".query", arr, delimiter=";")
        # ADD code execution
        process = subprocess.Popen(["./APIMLProject", "1", content['modelName']], stdout=subprocess.PIPE)
        outs, errs = process.communicate()
        #TODO TROUVER LE CHANNEL DE COMMUNICATION RETOUR
        answer = {
            'message': list(map(float,outs.split("\n"))),
            'timestamp': timestamp,
            'status_code': 201
        }
    return answer


signal.signal(signal.SIGTERM, serialization)
signal.signal(signal.SIGINT, serialization)
if __name__ == "__main__":
    data,model=deserialization()
    app.run(host="0.0.0.0", port=PORT)

