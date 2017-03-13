import subprocess
import os
import signal
from threading import Timer

def close_server():
    os.killpg(os.getpgid(server_proc.pid), signal.SIGTERM)
    print '\nTerminating server. Server failed to respond to simultaneous requests.'
    exit(1)

# Spawn a shell process to run the server
run_server_command = "./server path_config"
server_proc = subprocess.Popen(run_server_command, stdout=subprocess.PIPE, shell=True, preexec_fn=os.setsid)

# Set a timer to close hanging server
timer = Timer(5, close_server);
timer.start()

# Spawn a shell process to telnet to server to act as a hanging http request
telnet_request_command = "telnet localhost 8080"
telnet_request_proc = subprocess.Popen(telnet_request_command, stdout=subprocess.PIPE, shell=True)

# Echo handler test
# Spawn a shell process to curl to echo handler endpoint
echo_request_command = "curl --compressed -i localhost:8080/foo"
echo_request_proc = subprocess.Popen(echo_request_command, stdout=subprocess.PIPE, shell=True)
echo_response = echo_request_proc.stdout.read().decode('utf-8')

expected_echo_response = """HTTP/1.1 200 OK\r
Content-Type: text/plain\r
Content-Encoding: gzip\r
\r
GET /foo HTTP/1.1\r
User-Agent: curl/7.35.0\r
Host: localhost:8080\r
Accept: */*\r
Accept-Encoding: deflate, gzip\r\n\r\n"""

# Terminate server process before exiting
os.killpg(os.getpgid(server_proc.pid), signal.SIGTERM)

if echo_response != expected_echo_response:
    print("ERROR: Echo handler replied with an incorrect response.")
    exit(1)
else:
    print("SUCCESS: Echo handler replied with the correct response. Multithreading successful!")
    timer.cancel()
    exit(0)