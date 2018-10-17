import os, json, sys




def main():
    if len(sys.argv) != 2:
        print (len(sys.argv))
        print "Only one argument supported "
        exit(1)
        return

    device_id = int(sys.argv[1])

    with open("./devices.json") as f:
        devices = json.loads(f.read())
        print "Executing command: " + devices[device_id]["command"] + " on device " + str(device_id)

        os.system(devices[device_id]["command"])

main()