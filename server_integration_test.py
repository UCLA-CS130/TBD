import subprocess
import os
import signal

# Build server binary if it doesnt already exist
if not os.path.exists("server"):
    subprocess.check_call("make server", stdout=subprocess.PIPE, shell=True)

# Spawn a shell process to run the server
run_server_command = "./server example_config"
server_proc = subprocess.Popen(run_server_command, stdout=subprocess.PIPE, shell=True, preexec_fn=os.setsid)

# Spawn a shell process to curl to server at port 8080
run_curl_command = "curl -i localhost:8080"
curl_proc = subprocess.Popen(run_curl_command, stdout=subprocess.PIPE, shell=True)
response = curl_proc.stdout.read().decode('utf-8')

expected_response = """HTTP/1.1 200 OK
Content-Type: text/plain

GET / HTTP/1.1\r
User-Agent: curl/7.35.0\r
Host: localhost:8080\r
Accept: */*\r\n\r\n"""

# Terminate server process before exiting
os.killpg(os.getpgid(server_proc.pid), signal.SIGTERM)

if response != expected_response:
    print("Server replied with an incorrect response.")
    exit(1)
else:
    print("Server replied with the correct response. Successful test.")
    exit(0)
