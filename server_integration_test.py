import subprocess
import os.path

if not os.path.exists("server"):
    subprocess.check_call("make server", stdout=subprocess.PIPE, shell=True)

run_server_command = "./server example_config"
server_proc = subprocess.Popen(run_server_command, stdout=subprocess.PIPE, shell=True)

run_curl_command = "curl -i localhost:8080"
curl_proc = subprocess.Popen(run_curl_command, stdout=subprocess.PIPE, shell=True)
response = curl_proc.stdout.read().decode('utf-8')

expected_response = """HTTP/1.1 200 OK
Content-Type: text/plain

GET / HTTP/1.1\r
User-Agent: curl/7.35.0\r
Host: localhost:8080\r
Accept: */*\r\n\r\n"""

if response != expected_response:
	print("Incorrect response")
else:
	print("Correct response")
